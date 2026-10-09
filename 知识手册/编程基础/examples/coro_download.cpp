// 协程下载 URL 示例：使用 co_await 挂起，await_ready 检查 socket 是否可读
// 编译: g++ -std=c++20 -pthread coro_download.cpp -o coro_download
// 运行: ./coro_download http://example.com/
#include <coroutine>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <utility>      // std::exchange
#include <memory>       // std::unique_ptr
#include <semaphore>    // std::counting_semaphore
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

// ---------- 极简 Socket 包装（演示用，真实项目用专业网络库） ----------
class Socket {
    int fd_ = -1;
public:
    Socket() = default;
    explicit Socket(int fd) : fd_(fd) {}
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& o) noexcept : fd_(o.fd_) { o.fd_ = -1; }
    Socket& operator=(Socket&& o) noexcept {
        if (this != &o) { ::close(fd_); fd_ = o.fd_; o.fd_ = -1; }
        return *this;
    }
    ~Socket() { if (fd_ >= 0) ::close(fd_); }
    int get() const { return fd_; }
};

// ---------- 可读性等待器：await_ready 轮询 fd 是否可读 ----------
struct ReadableAwaiter {
    int fd_;
    // 第1问：已经可读？→ 无需挂起，直接执行 recv
    bool await_ready() noexcept {
        if (fd_ < 0) return false;
        struct pollfd pfd{fd_, POLLIN, 0};
        int r = ::poll(&pfd, 1, 0);            // 非阻塞探测，超时 0
        return r > 0 && (pfd.revents & POLLIN);
    }
    // 第2问：未就绪 → 挂起协程，另起线程阻塞等可读，就绪后 resume
    void await_suspend(std::coroutine_handle<> h) const noexcept {
        std::thread([fd = fd_, h]() {
            struct pollfd pfd{fd, POLLIN, 0};
            ::poll(&pfd, 1, -1);               // 阻塞等待可读
            h.resume();                        // 唤醒协程
        }).detach();
    }
    // 第3问：恢复后返回值（无）
    void await_resume() noexcept {}
};

// ---------- 基于 co_await 的读取（直接在协程内联使用） ----------
// 说明: 若想封装成 async_recv(fd, buf, n) 形式，其返回类型也必须是协程返回类型，
//       例：CoroResult<ssize_t> async_recv(...) { co_await ...; co_return ::recv(...); }
//       为简洁起见，这里直接在 download_url 里 co_await。

// ---------- DNS 解析 + 连接（阻塞，简化演示） ----------
Socket connect_host(const std::string& host, const std::string& port) {
    struct addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (::getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0) {
        return Socket{};
    }
    int fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd < 0) { ::freeaddrinfo(res); return Socket{}; }
    if (::connect(fd, res->ai_addr, res->ai_addrlen) != 0) {
        ::close(fd); ::freeaddrinfo(res); return Socket{};
    }
    ::freeaddrinfo(res);
    int flags = ::fcntl(fd, F_GETFL, 0);
    ::fcntl(fd, F_SETFL, flags | O_NONBLOCK);   // 设置非阻塞
    return Socket{fd};
}

// ---------- 极简协程返回类型：持有句柄 + 信号量，可查询并低 CPU 等待 ----------
// 说明：wait() 不用“自旋+yield”（会吃满一个 CPU 核，见 examples/cpu_bench.cpp），
//       而是用 counting_semaphore 让等待线程真正睡眠，CPU 占用趋近 0。
struct AsyncTask {
    struct promise_type {
        std::counting_semaphore<1>* sem_ = nullptr;   // 指向 AsyncTask 持有的信号量
        AsyncTask get_return_object() {
            AsyncTask t{std::coroutine_handle<promise_type>::from_promise(*this)};
            sem_ = t.sem_.get();                      // 让 promise 也能访问到它
            return t;
        }
        // 自定义 final awaiter：协程到达 final suspend 点（已挂起）后发信号
        struct FinalAwaiter {
            std::counting_semaphore<1>* sem_;
            bool await_ready() noexcept { return false; }
            void await_suspend(std::coroutine_handle<>) noexcept { sem_->release(); }
            void await_resume() noexcept {}
        };
        std::suspend_never initial_suspend() { return {}; }   // 启动即执行
        FinalAwaiter final_suspend() noexcept { return {sem_}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    Handle h_ = nullptr;
    std::unique_ptr<std::counting_semaphore<1>> sem_ =
        std::make_unique<std::counting_semaphore<1>>(0);   // 初始 0：wait 先阻塞

    explicit AsyncTask(Handle h) : h_(h) {}
    AsyncTask(const AsyncTask&) = delete;             // 不可复制
    AsyncTask(AsyncTask&& o) noexcept
        : h_(std::exchange(o.h_, nullptr)), sem_(std::move(o.sem_)) {}
    ~AsyncTask() { if (h_) h_.destroy(); }            // 负责销毁协程帧

    bool done() const { return h_ && h_.done(); }     // 协程是否已结束
    // 同步等待协程跑完：线程睡眠而非忙等，几乎不占 CPU
    void wait() {
        if (!h_) return;
        if (!h_.done()) sem_->acquire();              // 睡眠，直到 final_suspend release()
    }
};

// ---------- 下载一个 URL ----------
AsyncTask download_url(const std::string& url) {
    // 解析 host / port / path
    std::string rest = url.substr(url.find("://") + 3);
    std::string host = rest.substr(0, rest.find('/'));
    std::string path = rest.find('/') == std::string::npos ? "/" : rest.substr(rest.find('/'));
    std::string port = "80";

    std::cout << "[协程] 连接 " << host << " ..." << std::endl;
    Socket sock = connect_host(host, port);
    if (sock.get() < 0) {
        std::cout << "[协程] 连接失败: " << url << std::endl;
        co_return;
    }

    std::string req = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    ::send(sock.get(), req.data(), req.size(), 0);

    std::cout << "[协程] 等待响应（await_ready 探测可读性）..." << std::endl;
    char buf[4096];
    std::string content;
    while (true) {
        co_await ReadableAwaiter{sock.get()};   // 挂起等待 socket 可读
        ssize_t n = ::recv(sock.get(), buf, sizeof(buf), 0);  // 可读后真正读取
        if (n <= 0) break;
        content.append(buf, static_cast<size_t>(n));
    }
    std::cout << "[协程] 下载完成，共 " << content.size() << " 字节" << std::endl;
    std::cout << "[协程] 内容预览: "
              << content.substr(0, 200) << (content.size() > 200 ? " ..." : "")
              << std::endl;
}

int main(int argc, char** argv) {
    std::string url = argc > 1 ? argv[1] : "http://example.com/";
    std::cout << "主线程启动协程: " << url << std::endl;
    AsyncTask task = download_url(url);         // 返回对象持有协程句柄
    std::cout << "主线程未被阻塞，可以继续干活..." << std::endl;

    // 检查协程是否已结束（done() 可随时查询），未结束则等待，
    // 避免 main 提前 return 把后台协程杀掉
    if (!task.done()) {
        std::cout << "协程仍在运行，主线程等待其结束..." << std::endl;
        task.wait();                            // 同步等待协程跑完（等价 join）
    }
    std::cout << "主线程确认协程已结束，安全退出。" << std::endl;
    return 0;
}
