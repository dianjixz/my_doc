// CPU 占用对比基准：自旋 wait vs 信号量 wait
// 编译: g++ -std=c++20 -pthread cpu_bench.cpp -o cpu_bench
#include <coroutine>
#include <iostream>
#include <thread>
#include <chrono>
#include <utility>
#include <semaphore>
#include <memory>
#include <sys/resource.h>

// 进程累计 CPU 时间（微秒，含所有线程）
long long cpu_us() {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return (long long)ru.ru_utime.tv_sec * 1'000'000LL + ru.ru_utime.tv_usec
         + (long long)ru.ru_stime.tv_sec * 1'000'000LL + ru.ru_stime.tv_usec;
}

// 模拟异步：挂起后丢给后台线程，sleep 一段时间再 resume
struct TimerAwaiter {
    int ms_;
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
        std::thread([h, ms = ms_] {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            h.resume();
        }).detach();
    }
    void await_resume() {}
};

// ---------- 方案 A：自旋 + yield（当前 coro_download.cpp 的写法） ----------
struct SpinTask {
    struct promise_type {
        SpinTask get_return_object() {
            return SpinTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using H = std::coroutine_handle<promise_type>;
    H h_;
    explicit SpinTask(H h) : h_(h) {}
    SpinTask(SpinTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ~SpinTask() { if (h_) h_.destroy(); }
    SpinTask(const SpinTask&) = delete;
    void wait() { while (h_ && !h_.done()) std::this_thread::yield(); }  // 忙等
};

// ---------- 方案 B：信号量（线程真正睡眠） ----------
struct SemTask {
    struct promise_type {
        std::counting_semaphore<1>* sem_ = nullptr;   // 指向 AsyncTask 持有的信号量
        SemTask get_return_object() {
            SemTask t{std::coroutine_handle<promise_type>::from_promise(*this)};
            sem_ = t.sem_.get();
            return t;
        }
        // 自定义 final awaiter：协程已挂起（到达 final suspend 点）后再发信号
        struct FinalAwaiter {
            std::counting_semaphore<1>* sem_;
            bool await_ready() noexcept { return false; }
            void await_suspend(std::coroutine_handle<>) noexcept { sem_->release(); }
            void await_resume() noexcept {}
        };
        std::suspend_never initial_suspend() { return {}; }
        FinalAwaiter final_suspend() noexcept { return {sem_}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using H = std::coroutine_handle<promise_type>;
    H h_;
    std::unique_ptr<std::counting_semaphore<1>> sem_ =
        std::make_unique<std::counting_semaphore<1>>(0);   // 初始 0：先阻塞
    explicit SemTask(H h) : h_(h) {}
    SemTask(SemTask&& o) noexcept
        : h_(std::exchange(o.h_, {})), sem_(std::move(o.sem_)) {}
    ~SemTask() { if (h_) h_.destroy(); }
    SemTask(const SemTask&) = delete;
    void wait() {
        if (!h_) return;
        if (!h_.done()) sem_->acquire();    // 睡眠，直到 final_suspend release()
    }
};

SpinTask spin_work(int ms) { co_await TimerAwaiter{ms}; }
SemTask  sem_work(int ms)  { co_await TimerAwaiter{ms}; }

int main() {
    const int ms = 1500;   // 后台任务耗时 1.5 秒

    // A: 自旋
    auto t0 = std::chrono::steady_clock::now();
    long long c0 = cpu_us();
    SpinTask a = spin_work(ms);
    a.wait();
    long long c1 = cpu_us();
    auto t1 = std::chrono::steady_clock::now();
    double wallA = std::chrono::duration<double>(t1 - t0).count();
    double cpuA = (c1 - c0) / 1'000'000.0;
    std::cout << "[自旋+yield ] 墙钟 " << wallA << " s, CPU 时间 " << cpuA
              << " s, CPU 占用 ≈ " << (int)(cpuA / wallA * 100) << " %\n";

    // B: 信号量
    auto t2 = std::chrono::steady_clock::now();
    long long c2 = cpu_us();
    SemTask b = sem_work(ms);
    b.wait();
    long long c3 = cpu_us();
    auto t3 = std::chrono::steady_clock::now();
    double wallB = std::chrono::duration<double>(t3 - t2).count();
    double cpuB = (c3 - c2) / 1'000'000.0;
    std::cout << "[信号量     ] 墙钟 " << wallB << " s, CPU 时间 " << cpuB
              << " s, CPU 占用 ≈ " << (int)(cpuB / wallB * 100) << " %\n";
    return 0;
}
