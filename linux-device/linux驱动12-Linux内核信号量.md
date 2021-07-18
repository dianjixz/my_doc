# Linux内核信号量

# 概念

Linux内核的信号量在概念和原理上和用户态的System V的IPC机制信号量是相同的，不过他**绝不可能在内核之外使用**，因此他和System V的IPC机制信号量毫不相干。

如果有一个任务想要获得已经被占用的信号量时，信号量会将其放入一个等待队列（它不是站在外面痴痴地等待而是将自己的名字写在任务队列中）然后让其睡眠。

当持有信号量的进程将信号释放后，处于等待队列中的一个任务将被唤醒（因为队列中可能不止一个任务），并让其获得信号量。这一点与自旋锁不同，处理器可以去执行其它代码。

# 应用场景

由于争用信号量的进程在等待锁重新变为可用时会睡眠，所以信号量适用于锁会被长时间持有的情况；相反，锁被短时间持有时，使用信号量就不太适宜了，因为睡眠、维护等待队列以及唤醒所花费的开销可能比锁占用的全部时间表还要长。

**举2个生活中的例子：**

1. 我们坐火车从南京到新疆需要2天的时间，这个'任务'特别的耗时，只能坐在车上等着车到站，但是我们没有必要一直睁着眼睛等，理想的情况就是我们上车就直接睡觉，醒来就到站（看过《异形》的读者会深有体会），这样从人（用户）的角度来说，体验是最好的，对比于进程，程序在等待一个耗时事件的时候，没有必须要一直占用CPU，可以暂停当前任务使其进入休眠状态，当等待的事件发生之后再由其他任务唤醒，类似于这种场景采用信号量比较合适。
2. 我们有时候会等待电梯、洗手间，这种场景需要等待的时间并不是很多，如果我们还要找个地方睡一觉，然后等电梯到了或者洗手间可以用了再醒来，那很显然这也没有必要，我们只需要排好队，刷一刷抖音就可以了，对比于计算机程序，比如驱动在进入中断例程，在等待某个寄存器被置位，这种场景需要等待的时间往往很短暂，系统开销甚至远小于进入休眠的开销，所以这种场景采用自旋锁比较合适。

关于信号量和自旋锁，以及死锁问题，我们后面会再详细讨论。

# 使用方法

一个任务要想访问共享资源，首先必须得到信号量，获取信号量的操作将把信号量的值减1，若当前信号量的值为负数，表明无法获得信号量，该任务必须挂起在 该信号量的等待队列等待该信号量可用；若当前信号量的值为非负数，表示能获得信号量，因而能即时访问被该信号量保护的共享资源。

当任务访问完被信号量保护的共享资源后，必须释放信号量，释放信号量通过把信号量的值加1实现，如果信号量的值为非正数，表明有任务等待当前信号量，因此他也唤醒所有等待该信号量的任务。

# 内核信号量的构成

内核信号量类似于自旋锁，因为当锁关闭着时，它不允许内核控制路径继续进行。然而，当内核控制路径试图获取内核信号量锁保护的忙资源时，相应的进程就被挂起。只有在资源被释放时，进程才再次变为可运行。

只有可以睡眠的函数才能获取内核信号量；中断处理程序和可延迟函数都不能使用内核信号量。

内核信号量是struct semaphore类型的对象，在内核源码中位于include\linux\semaphore.h文件

~~~ c
struct semaphore{
    raw_spinlock_t        lock;
    unsigned int        count;
    struct list_head    wait_list;
}
~~~

| 成员      | 描述                                                         |
| :-------- | :----------------------------------------------------------- |
| lock      | 在2.6.33之后的版本，内核加入了raw_spin_lock系列，使用方法和spin_lock系列一模一样，只是参数spinlock_t变为了raw_spinlock_t |
| count     | 相当于信号量的值，大于0，资源空闲；等于0，资源忙，但没有进程等待这个保护的资源；小于0，资源不可用，并至少有一个进程等待资源 |
| wait_list | 内核链表，当前获得信号量的任务会与该成员一起注册到等待的链表中 |

# 信号量的API

初始化

~~~ c
DECLARE_MUTEX(name)
~~~

该宏声明一个信号量name并初始化他的值为1，即声明一个互斥锁。

~~~ c
DECLARE_MUTEX_LOCKED(name)
~~~

该宏声明一个互斥锁name，但把他的初始值设置为0，即锁在创建时就处在已锁状态。因此对于这种锁，一般是先释放后获得。

~~~ c
void sema_init (struct semaphore *sem, int val);
~~~

该函用于数初始化设置信号量的初值，他设置信号量sem的值为val。

注意：

val设置为1说明只有一个持有者，这种信号量叫二值信号量或者叫互斥信号量。

我们还允许信号量可以有多个持有者，这种信号量叫计数信号量，在初始化时要说明最多允许有多少个持有者也可以把信号量中的val初始化为任意的正数值n，在这种情况下，最多有n个进程可以并发地访问这个资源。

~~~ c
void init_MUTEX (struct semaphore *sem);
~~~

该函数用于初始化一个互斥锁，即他把信号量sem的值设置为1。

~~~ c
void init_MUTEX_LOCKED (struct semaphore *sem);
~~~

该函数也用于初始化一个互斥锁，但他把信号量sem的值设置为0，即一开始就处在已锁状态。

# PV操作

## 获取信号量（P）

~~~ c
void down(struct semaphore * sem);
~~~

该函数用于获得信号量sem，他会导致调用该函数的进程睡眠，**因此不能在中断上下文**（包括IRQ上下文和softirq上下文）使用该函数。该函数将把sem的值减1，如果信号量sem的值非负，就直接返回，否则调用者将被挂起，直到别的任务释放该信号量才能继续运行。

~~~ c
int down_interruptible(struct semaphore * sem);
~~~

该函数功能和down类似，不同之处为，down不会被信号（signal）打断，但down_interruptible能被信号打断，因此该函数有返回值来区分是正常返回还是被信号中断，如果返回0，表示获得信号量正常返回，如果被信号打断，返回-EINTR。

~~~ c
int down_trylock(struct semaphore * sem);
~~~

该函数试着获得信号量sem，如果能够即时获得，他就获得该信号量并返回0，否则，表示不能获得信号量sem，返回值为非0值。因此，他不会导致调用者睡眠，能在中断上下文使用。

~~~ c
int down_killable(struct semaphore *sem);
int down_timeout(struct semaphore *sem, long jiffies);
int down_timeout_interruptible(struct semaphore *sem, long jiffies);
~~~

## 释放内核信号量（V）

~~~ c
void up(struct semaphore * sem);
~~~

该函数释放信号量sem，即把sem的值加1，如果sem的值为非正数，表明有任务等待该信号量，因此唤醒这些等待者。

## 补充

~~~ c
int down_interruptible(struct semaphore *sem)
~~~

这个函数的功能就是获得信号量，如果得不到信号量就睡眠，此时没有信号打断，那么进入睡眠。但是在睡眠过程中可能被信号打断，打断之后返回-EINTR，主要用来进程间的互斥同步。

下面是该函数的注释：

~~~ c
/**
* down_interruptible - acquire the semaphore unless interrupted
* @sem: the semaphore to be acquired
*
* Attempts to acquire the semaphore. If no more tasks are allowed to
* acquire the semaphore, calling this function will put the task to sleep.
* If the sleep is interrupted by a signal, this function will return -EINTR.
* If the semaphore is successfully acquired, this function returns 0.
*/
~~~

一个进程在调用down_interruptible()之后，如果sem<0，那么就进入到可中断的睡眠状态并调度其它进程运行， 但是一旦该进程收到信号，那么就会从down_interruptible函数中返回。并标记错误号为:-EINTR。

一个形象的比喻：传入的信号量为1好比天亮，如果当前信号量为0，进程睡眠，直到（信号量为1）天亮才醒，但是可能中途有个闹铃（信号）把你闹醒。

又如：小强下午放学回家，回家了就要开始吃饭嘛，这时就会有两种情况：情况一：饭做好了，可以开始吃；情况二：当他到厨房去的时候发现妈妈还在做， 妈妈就对他说：“你先去睡会，待会做好了叫你。” 小强就答应去睡会，不过又说了一句：“睡的这段时间要是小红来找我玩，你可以叫醒我。” 小强就是down_interruptible，想吃饭就是获取信号量，睡觉对应这里的休眠，而小红来找我玩就是中断休眠。

使用可被中断的信号量版本的意思是，万一出现了semaphore的死锁，还有机会用ctrl+c发出软中断，让等待这个内核驱动返回的用户态进程退出。而不是把整个系统都锁住了。在休眠时，能被中断信号终止，这个进程是可以接受中断信号的！

比如你在命令行中输入# sleep 10000，按下ctrl + c，就给上面的进程发送了进程终止信号。信号发送给用户空间，然后通过系统调用，会把这个信号传给递给驱动。信号只能发送给用户空间，无权直接发送给内核的，那1G的内核空间，我们是无法直接去操作的。

# 内核信号量的使用例程

场景1

在驱动程序中，当多个线程同时访问相同的资源时（驱动中的全局变量时一种典型的共享资源），可能会引发“竞态“，因此我们必须对共享资源进行并发控制。Linux内核中

解决并发控制的最常用方法是自旋锁与信号量（绝大多数时候作为互斥锁使用）。

![微信图片_20210607223002](D:\work\git\my_doc\image\微信图片_20210607223002.png)

场景2

有时候我们希望设备只能被一个进程打开，当设备被占用的时候，其他设备必须进入休眠。

信号处理示意图

![微信图片_20210607223022](D:\work\git\my_doc\image\微信图片_20210607223022.png)

如上图：

1. 进程A首先通过open（）打开设备文件，调用到内核的hello_open（），并调用down_interruptible（），因为此时信号量没有被占用，所以进程A可以获得信号量；
2. 进程A获得信号量之后继续处理原有任务，此时进程B也要通过open（）打开设备文件，同样调用内核函数hello_open(),但此时信号量获取不到，于是进程B被阻塞；
3. 进程A任务执行完毕，关闭设备文件，并通过up（）释放信号量，于是进程B被唤醒，并得以继续执行剩下的任务，
4. 进程B执行完任务，释放设备文件，通过up()释放信号量

代码如下：

~~~ c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/semaphore.h>

static int major = 250;
static int minor = 0;
static dev_t devno;
static struct cdev cdev;


static struct class *cls;
static struct device *test_device;

static struct semaphore sem;
static int hello_open (struct inode *inode, struct file *filep)
{
    
    if(down_interruptible(&sem))//p
    {
        return -ERESTARTSYS;
    }
      return 0;
}
static int hello_release (struct inode *inode, struct file *filep)
{
    up(&sem);//v
    return 0;
}
static struct file_operations hello_ops =
{
    .open = hello_open,
    .release = hello_release,
};
static int hello_init(void)
{
    int result;
    int error;    
    printk("hello_init \n");
    result = register_chrdev( major, "hello", &hello_ops);
    if(result < 0)
    {
        printk("register_chrdev fail \n");
        return result;
    }
    devno = MKDEV(major,minor);
    cls = class_create(THIS_MODULE,"helloclass");
    if(IS_ERR(cls))
    {
        unregister_chrdev(major,"hello");
        return result;
    }
    test_device = device_create(cls,NULL,devno,NULL,"test");
    if(IS_ERR(test_device ))
    {
        class_destroy(cls);
        unregister_chrdev(major,"hello");
        return result;
    }
    sem_init(&sem,1);
    return 0;
}
static void hello_exit(void)
{
    printk("hello_exit \n");
    device_destroy(cls,devno);    
    class_destroy(cls);
    unregister_chrdev(major,"hello");
    return;
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("daniel.peng")；
~~~

测试程序 test.c

~~~ c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
main()
{
    int fd;
    
    printf("before open\n ");    
    fd = open("/dev/test",O_RDWR);  //原子变量  0
    if(fd<0)
    {
        perror("open fail \n");
        return;
    }
    printf("open ok ,sleep......\n ");    
    sleep(20);
    printf("wake up from sleep!\n ");        
    close(fd);   //加为1
}
~~~

编译步骤

1 make 生成 hello.ko

2 gcc test.c -o a

3 gcc test.c -o b

测试步骤

1. 安装驱动

~~~ bash
insmod hello.ko
~~~

先运行进程A，在运行进程B

可见进程A成功打开设备，在进程A sleep期间会一直占有该字符设备，进程B由于无法获得信号量，进入休闲，结合代码可知，进程B被阻塞在函数open()中。

进程A 结束了sleep，并释放字符设备以及信号量，进程B被唤醒获得信号量，并成功打开了字符设备。

进程B执行完sleep函数后退出，并释放字符设备和信号量。

# 读-写信号量

跟自旋锁一样，信号量也有区分读-写信号量之分。

如果一个读写信号量当前没有被写者拥有并且也没有写者等待读者释放信号量，那么任何读者都可以成功获得该读写信号量；否则，读者必须被挂起直到写者释放该信号量。如果一个读写信号量当前没有被读者或写者拥有并且也没有写者等待该信号量，那么一个写者可以成功获得该读写信号量，否则写者将被挂起，直到没有任何访问者。因此，写者是排他性的，独占性的。

读写信号量有两种实现，一种是通用的，不依赖于硬件架构，因此，增加新的架构不需要重新实现它，但缺点是性能低，获得和释放读写信号量的开销大；另一种是架构相关的，因此性能高，获取和释放读写信号量的开销小，但增加新的架构需要重新实现。在内核配置时，可以通过选项去控制使用哪一种实现。

读写信号量的相关API：

~~~ c
DECLARE_RWSEM(name)
~~~

该宏声明一个读写信号量name并对其进行初始化

~~~ c
void init_rwsem(struct rw_semaphore *sem);
~~~

该函数对读写信号量sem进行初始化。

~~~ c
void down_read(struct rw_semaphore *sem);
~~~

读者调用该函数来得到读写信号量sem。该函数会导致调用者睡眠，因此只能在进程上下文使用。

~~~ c
int down_read_trylock(struct rw_semaphore *sem);
~~~

该函数类似于down_read，只是它不会导致调用者睡眠。它尽力得到读写信号量sem，如果能够立即得到，它就得到该读写信号量，并且返回1，否则表示不能立刻得到该信号量，返回0。因此，它也可以在中断上下文使用。

~~~ c
void down_write(struct rw_semaphore *sem);
~~~

写者使用该函数来得到读写信号量sem，它也会导致调用者睡眠，因此只能在进程上下文使用。

~~~ c
int down_write_trylock(struct rw_semaphore *sem);
~~~

该函数类似于down_write，只是它不会导致调用者睡眠。该函数尽力得到读写信号量，如果能够立刻获得，就获得该读写信号量并且返回1，否则表示无法立刻获得，返回0。它可以在中断上下文使用。

~~~ c
void up_read(struct rw_semaphore *sem);
~~~

读者使用该函数释放读写信号量sem。它与down_read或down_read_trylock配对使用。

如果down_read_trylock返回0，不需要调用up_read来释放读写信号量，因为根本就没有获得信号量。

~~~ c
void up_write(struct rw_semaphore *sem);
~~~

写者调用该函数释放信号量sem。它与down_write或down_write_trylock配对使用。如果down_write_trylock返回0，不需要调用up_write，因为返回0表示没有获得该读写信号量。

~~~ c
void downgrade_write(struct rw_semaphore *sem);
~~~

该函数用于把写者降级为读者，这有时是必要的。因为写者是排他性的，因此在写者保持读写信号量期间，任何读者或写者都将无法访问该读写信号量保护的共享资源，对于那些当前条件下不需要写访问的写者，降级为读者将，使得等待访问的读者能够立刻访问，从而增加了并发性，提高了效率。

读写信号量适于在读多写少的情况下使用，在linux内核中对进程的内存映像描述结构的访问就使用了读写信号量进行保护。