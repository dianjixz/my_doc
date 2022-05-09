c语言中锁的作用,c 互斥锁

https://blog.csdn.net/weixin_29421565/article/details/117010880


~~~ c

操作函数pthread_mutex_t lock; /* 互斥锁定义 */

pthread_mutex_init(&lock, NULL); /* 动态初始化，成功返回0，失败返回非0 */

pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER； /* 静态初始化 */

pthread_mutex_lock(&lock); /* 阻塞的锁定互斥锁 */

pthread_mutex_trylock(&thread_mutex)；/* 非阻塞的锁定互斥锁，成功获得互斥锁返回0，如果未能获得互斥锁，立即返回一个错误码 */

pthread_mutex_unlock(&lock)； /* 解锁互斥锁 */

pthread_mutex_destroy(&lock) /* 销毁互斥锁 */

~~~


~~~ c
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
功能：使用alarm函数可以设置一个时间值(闹钟时间)，当执行到该函数之后开始计时，超时时产生SIGALRM信号。如果不忽略或不捕捉此信号，则其默认动作是终止该进程 
参数：

    定时秒数（以秒为单位）
    如果秒为零，则取消任何挂起的警报。在任何情况下，任何先前设置的报警都会被取消

返回值：

    成功：第一次调用返回0，不是第一次调用返回以前设置的闹钟时间的余留秒数
    失败：非0

注意事项：

    参数seconds的值是产生SIGALRM需要经过的时钟秒数。当这一时刻达到时，信号由内核产生，由于进程调度的延迟，所以进程得到控制从而能够处理该信号还需要一定的时间间隔
    如果我们想要捕捉SIGALRM信号，并且处理SIGALRM信号，必须在alarm之前书写该信号的处理函数

#include <unistd.h>
useconds_t ualarm(useconds_t usecs, useconds_t interval);

功能：在usecs微秒后，将SIGALRM信号发送给进程，并且之后每隔interval微秒再发送一次SIGALRM信号。如果不对SIGALRM信号进程处理，默认操作是终止进程
延迟可能会因任何系统活动、处理调用所花费的时间或系统计时器的粒度而略微延长
参数：

    usecs：第一次触发SIGALRM信号的时间
    interval：第一次触发SIGALRM信号之后每隔interval微秒再触发一次SIGALRM信号
    以微秒为单位

返回值：此函数返回以前设置的任何警报的剩余微秒数。如果没有挂起警报，则返回0（第一次调用该函数也返回0）。常见的errno如下：

    EINTR：被一个信号打断了

    EINVAL：usecs或interval不小于1000000(在被认为是错误的系统上)


#include <sys/time.h>
int getitimer (int which, struct itimerval* value);
int setitimer (int which, struct itimerval* newvalue, struct itimerval* oldvalue);

which有三种状态:
ITIMER_REAL: 对指定时间值,按自然时间计数, 时间到发出SIGALRM信号.
ITIMER_VIRTUAL: 对指定时间值, 当只在用户态时（进程执行的时候）计数,  时间到发出SIGVTALRM信号.
ITIMER_PROF: 对指定时间值, 用户态或内核态（进程执行与系统为进程调度）都计数, 时间到, 发出SIGPROF信号, 与ITIMER_VIRTVAL联合, 常用来计算系统内核时间和用户时间.

struct timeval
{
long tv_sec; /* 秒 */
long tv_usec; /* 微秒 */
};

struct itimerval
{
struct timeval it_interval;  /* 时间间隔 *///循环定时时间
struct timeval it_value; /* 当前时间计数 */第一次计时时间
};
it_interval用来指定每隔多长时间执行任务, it_value用来保存当前时间离执行任务还有多长时间. 比如说, 你指定it_interval为2秒(微秒为0), 开始的时候我们把it_value的时间也设定为2秒(微秒为0), 当过了一秒, it_value就减少一个为1, 再过1秒, 则it_value又减少1, 变为0, 这个时候发出信号(告诉用户时间到了, 可以执行任务了), 并且系统自动把it_value的置重置为it_interval的值, 即2秒, 再重新计数.





~~~