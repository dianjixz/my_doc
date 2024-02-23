# 自旋锁（spin lock）

自旋锁是Linux中使用非常频繁的锁，原理简单。

内核当发生访问资源冲突的时候，可以有两种锁的解决方案选择：

- 一个是原地等待
- 一个是挂起当前进程，调度其他进程执行（睡眠）

Spinlock 是内核中提供的一种比较常见的锁机制，自旋锁是“原地等待”的方式解决资源冲突的，即，一个线程获取了一个自旋锁后，另外一个线程期望获取该自旋锁，获取不到，只能够原地“打转”（忙等待）。由于自旋锁的这个忙等待的特性，注定了它使用场景上的限制 —— 自旋锁不应该被长时间的持有（消耗 CPU 资源），一般应用在中断上下文。

## 原理

下面以去银行办业务为例来讲解。

1. 银行的办事大厅一般会有几个窗口同步进行。今天很不巧，只有一个窗口提供服务。现在的银行服务都是采用取号排队，叫号服务的方式。
2. 当你去银行办理业务的时候，首先会去取号机器领取小票，上面写着你排多少号。然后你就可以排队等待了。一般还会有个显示屏，上面会显示一个数字（例如："请xxx号到1号窗口办理"），代表当前可以被服务顾客的排队号码。每办理完一个顾客的业务，显示屏上面的数字都会增加1。等待的顾客都会对比自己手上写的编号和显示屏上面是否一致，如果一致的话，就可以去前台办理业务了。
3. 现在早上刚开业，顾客A是今天的第一个顾客，去取号机器领取0号（next计数）小票，然后看到显示屏上显示0（owner计数），顾客A就知道现在轮到自己办理业务了。
4. 顾客A到前台办理业务（持有锁）中，顾客B来了。同样，顾客B去取号机器拿到1号（next计数）小票。然后乖乖的坐在旁边等候。顾客A依然在办理业务中，此时顾客C也来了。顾客C去取号机器拿到2号（next计数）小票。顾客C也乖乖的找个座位继续等待。
5. 终于，顾客A的业务办完了（释放锁）。然后，显示屏上面显示1（owner计数）。顾客B和C都对比显示屏上面的数字和自己手中小票的数字是否相等。顾客B终于可以办理业务了（持有锁）。顾客C依然等待中。
6. 顾客B的业务办完了（释放锁）。然后，显示屏上面显示2（owner计数）。顾客C终于开始办理业务（持有锁）。顾客C的业务办完了（释放锁）。
7. 3个顾客都办完了业务离开了。只留下一个银行柜台服务员。
8. 最终，显示屏上面显示3（owner计数）。取号机器的下一个排队号也是3号（next计数）。无人办理业务（锁是释放状态）。

Linux中针对每一个spin lock会有两个计数。分别是next和owner（初始值为0）。进程A申请锁时，会判断next和owner的值是否相等。如果相等就代表锁可以申请成功，否则原地自旋。直到owner和next的值相等才会退出自旋。假设进程A申请锁成功，然后会next加1。此时owner值为0，next值为1。进程B也申请锁，保存next得值到局部变量temp（temp = 1）中。由于next和owner值不相等，因此原地自旋读取owner的值，判断owner和temp是否相等，直到相等退出自旋状态。当然next的值还是加1，变成2。进程A释放锁，此时会将owner的值加1，那么此时B进程的owner和temp的值都是1，因此B进程获得锁。当B进程释放锁后，同样会将owner的值加1。最后owner和next都等于2，代表没有进程持有锁。next就是一个记录申请锁的次数，而owner是持有锁进程的计数值。

## 实际场景

### 一、考虑下面的场景（内核抢占场景）：

（1）进程A在某个系统调用过程中访问了共享资源 R 

（2）进程B在某个系统调用过程中也访问了共享资源 R 会不会造成冲突呢？

假设在A访问共享资源R的过程中发生了中断，中断唤醒了沉睡中的，优先级更高的B，在中断返回现场的时候，发生进程切换，B启动执行，并通过系统调用访问了R，如果没有锁保护，则会出现两个thread进入临界区，导致程序执行不正确。OK，我们加上spin lock看看如何：A在进入临界区之前获取了spin lock，同样的，在A访问共享资源R的过程中发生了中断，中断唤醒了沉睡中的，优先级更高的B，B在访问临界区之前仍然会试图获取spin lock，这时候由于A进程持有spin lock而导致B进程进入了永久的spin……怎么破？

linux的kernel很简单，在A进程获取spin lock的时候，禁止本CPU上的抢占（上面的永久spin的场合仅仅在本CPU的进程抢占本CPU的当前进程这样的场景中发生）。如果A和B运行在不同的CPU上，那么情况会简单一些：A进程虽然持有spin lock而导致B进程进入spin状态，不过由于运行在不同的CPU上，A进程会持续执行并会很快释放spin lock，解除B进程的spin状态

### 二、再考虑下面的场景（中断上下文场景）：

（1）运行在CPU0上的进程A在某个系统调用过程中访问了共享资源 R 

（2）运行在CPU1上的进程B在某个系统调用过程中也访问了共享资源 R 

**（3）外设P的中断handler中也会访问共享资源 R 在这样的场景下，使用spin lock可以保护访问共享资源R的临界区吗？**

我们假设CPU0上的进程A持有spin lock进入临界区，这时候，外设P发生了中断事件，并且调度到了CPU1上执行，看起来没有什么问题，执行在CPU1上的handler会稍微等待一会CPU0上的进程A，等它立刻临界区就会释放spin lock的，但是，如果外设P的中断事件被调度到了CPU0上执行会怎么样？

CPU0上的进程A在持有spin lock的状态下被中断上下文抢占，而抢占它的CPU0上的handler在进入临界区之前仍然会试图获取spin lock，悲剧发生了，CPU0上的P外设的中断handler永远的进入spin状态，这时候，CPU1上的进程B也不可避免在试图持有spin lock的时候失败而导致进入spin状态。为了解决这样的问题，linux kernel采用了这样的办法：如果涉及到中断上下文的访问，spin lock需要和禁止本 CPU 上的中断联合使用。

### 三、再考虑下面的场景（底半部场景）

linux kernel中提供了丰富的bottom half的机制，虽然同属中断上下文，不过还是稍有不同。我们可以把上面的场景简单修改一下：外设P不是中断handler中访问共享资源R，而是在的bottom half中访问。使用spin lock+禁止本地中断当然是可以达到保护共享资源的效果，但是使用牛刀来杀鸡似乎有点小题大做，这时候disable bottom half就OK了

### 四、中断上下文之间的竞争

同一种中断handler之间在uni core和multi core上都不会并行执行，这是linux kernel的特性。如果不同中断handler需要使用spin lock保护共享资源，对于新的内核（不区分fast handler和slow handler），所有handler都是关闭中断的，因此使用spin lock不需要关闭中断的配合。

bottom half又分成softirq和tasklet，同一种softirq会在不同的CPU上并发执行，因此如果某个驱动中的softirq的handler中会访问某个全局变量，对该全局变量是需要使用spin lock保护的，不用配合disable CPU中断或者bottom half。tasklet更简单，因为同一种tasklet不会多个CPU上并发。

### 实现

我们首先定义描述自旋锁的结构体arch_spinlock_t。

~~~ c
typedef struct {
    union{
        unsigned int slock;
        struct __raw_tickets{
            unsigned short owner;
            unsigned short next;
        }tisckets;
    };
}arch_spinlock_t;
~~~

如上面的原理描述，我们需要两个计数，分别是owner和next。slock所占内存区域覆盖owner和next（据说C语言学好的都能看得懂）。下面实现申请锁操作 arch_spin_lock。

~~~ c
static inline void arch_spin_lock(arch_spinlock_t *lock){
    arch_spinlock_t old_lock;
    old_lock.slock = lock->slock;
    lock->tickets.next++;
    while(old_lock.tickets.next != old_lock.tickets.owner){
        old_lock.tickets.owner = lock->tickets.owner;
    }
}
~~~

1. 继续上面的举例。顾客从取号机器得到排队号。
2. 取号机器更新下个顾客将要拿到的排队号。
3. 看一下显示屏，判断是否轮到自己了。
4. 一直盯着显示屏对比是不是轮到自己了。

释放锁的操作就非常简单了。还记得上面银行办理业务的例子吗？释放锁的操作仅仅是显示屏上面的排队号加1。我们仅仅需要将owner计数加1即可。arch_spin_unlock实现如下。

~~~ c
static inline void arch_spin_unlock(arch_spinlock_t *lock){
    lock->tickets.owner++;
}
~~~

# 信号量（semaphore）

信号量又称为信号灯，它是用来协调不同进程间的数据对象的，而最主要的应用是共享内存方式的进程间通信。本质上，信号量是一个计数器，它用来记录对某个资源（如共享内存）的存取状况。它负责协调各个进程，以保证他们能够正确、合理的使用公共资源。它和spin lock最大的不同之处就是：无法获取信号量的进程可以睡眠，因此会导致系统调度。一般说来，为了获得共享资源，进程需要执行下列操作：　　 

（1） 测试控制该资源的信号量。　　 

（2） 若此信号量的值为正，则允许进行使用该资源。进程将信号量减1。

（3） 若此信号量为0，则该资源目前不可用，进程进入睡眠状态，直至信号量值大于0，进程被唤醒，转入步骤（1）。　　 

（4） 当进程不再使用一个信号量控制的资源时，信号量值加1。如果此时有进程正在睡眠等待此信号量，则唤醒此进程。　 　 

维护信号量状态的是Linux内核操作系统而不是用户进程。我们可以从头文件/usr/src/linux/include/linux/sem.h 中看到内核用来维护信号量状态的各个结构的定义。信号量是一个数据集合，用户可以单独使用这一集合的每个元素。　 

　 信号量（semaphore）是进程间通信处理同步互斥的机制。是在多线程环境下使用的一种措施，它负责协调各个进程，以保证他们能够正确、合理的使用公共资源。它和spin lock最大的不同之处就是：无法获取信号量的进程可以睡眠，因此会导致系统调度。

## 原理

信号量一般可以用来标记可用资源的个数。

**举2个生活中的例子：**

1. 我们要坐火车从南京到新疆，这个'任务'特别的耗时，只能在车上等着车到站，但是我们没有必要一直睁着眼睛等着车到站，最好的情况就是我们上车就直接睡觉，醒来就到站，这样从人（用户）的角度来说，体验是最好的，对比于进程，程序在等待一个耗时的任务的时候，没有必须要占用CPU，可以暂停当前任务使其进入休眠状态，当等待的事件发生之后再由其他任务唤醒，这种场景采用信号量比较合适。
2. 我们在等待电梯、等待洗手间，这种场景需要等待的事件并不是很多，如果我们还要找个地方睡一觉，然后等电梯到了或者洗手间可以用了再醒来，那很显然这也没有必要，我们只需要排好队，刷一刷抖音就可以了，对比于计算机程序，比如驱动在进入中断例程，在等待某个寄存器被置位，这种场景需要等待的时间很短暂，系统开销远小于进入休眠的开销，所以这种场景采用自旋锁比较合适。

## 实现

为了记录可用资源的数量，我们肯定需要一个count计数，标记当前可用资源数量。当然还要一个可以像图书管理员一样的笔记本功能。用来记录等待借书的同学。所以，一个双向链表即可。因此只需要一个count计数和等待进程的链表头即可。描述信号量的结构体如下。

~~~ c
struct semaphore{
    unsigned int count;
    struct list_head wait_list;
};
~~~

在Linux中，每个进程就相当于是每个借书的同学。通知一个同学，就相当于唤醒这个进程。因此，我们还需要一个结构体记录当前的进程信息（task_struct）。

~~~ c
struct semaphor_waiter{
    struct list_head list;
    struct task_struct *task;
};
~~~

struct semaphore_waiter的list成员是当进程无法获取信号量的时候挂入semaphore的wait_list成员。task成员就是记录后续被唤醒的进程信息。

将当前进程赋给task，并利用其list成员将该变量的节点加入到以sem中的wait_list为头部的一个列表中，假设有多个进程在sem上调用down_interruptible，则sem的wait_list上形成的队列如下图：

![微信图片_20210607152607](../../../image\微信图片_20210607152607.jpg)

(注：将一个进程阻塞，一般的经过是先把进程放到等待队列中，接着改变进程的状态，比如设为TASK_INTERRUPTIBLE，然后调用调度函数schedule()，后者将会把当前进程从cpu的运行队列中摘下) 

一切准备就绪，现在就可以实现信号量的申请函数。

~~~ c
void down(struct semaphore *sem){
    struct semaphore_waiter waiter;
    if(sem->count >0){
        sem->count+++;
        return ;
    }
    waiter.task = current;
    list_add_tail(&waiter.list,&sem->wait_list);
    schedule();
}
~~~

1. 如果信号量标记的资源还有剩余，自然可以成功获取信号量。只需要递减可用资源计数。
2. 既然无法获取信号量，就需要将当前进程挂入信号量的等待队列链表上。
3. schedule()主要是触发任务调度的示意函数，主动让出CPU使用权。在让出之前，需要将当前进程从运行队列上移除。

释放信号的实现也是比较简单。实现如下。

~~~ c
void up(struct semaphore *sem){
    struct semaphore)waiter waiter;
    if(list_empty(&sem->wait_list)){
        sem->count++;
        return ;
    }
    waiter = list_first_entry(&sem->wait_list,struct semaphore_waiter,list);
    list_del(&waiter->list);
    wake_up_process(waiter->task);
}
~~~

1. 如果等待链表没有进程，那么自然只需要增加资源计数。
2. 从等待进程链表头取出第一个进程，并从链表上移除。然后就是唤醒该进程

# 读写锁（read-write lock）

不管是自旋锁还是信号量在同一时间只能有一个进程进入临界区。对于有些情况，我们是可以区分读写操作的。因此，我们希望对于读操作的进程可以并发进行。对于写操作只限于一个进程进入临界区。而这种同步机制就是读写锁。读写锁一般具有以下几种性质。

1. 同一时间有且仅有一个写进程进入临界区。
2. 在没有写进程进入临界区的时候，同时可以有多个读进程进入临界区。
3. 读进程和写进程不可以同时进入临界区。

读写锁有两种，一种是信号量类型，另一种是spin lock类型。下面以spin lock类型讲解。

## 原理

下面我们用厕所模型来理解读写锁。

1. 我发现公司一般都会有保洁阿姨打扫厕所。如果以男厕所为例的话，我觉得男士进入厕所就相当于读者进入临界区。因为可以有多个男士进厕所。而保洁阿姨进入男士厕所就相当于写者进入临界区。
2. 假设A男士发现保洁阿姨不在打扫厕所，就进入厕所。随后B和C同时也进入厕所。
3. 然后保洁阿姨准备打扫厕所，发现有男士在厕所里面，因此只能在门口等待。
4. ABC都离开了厕所。保洁阿姨迅速进入厕所打扫。然后D男士去上厕所，发现保洁阿姨在里面。灰溜溜的出来了在门口等着。现在体会到了写者（保洁阿姨）具有排他性，读者（男士）可以并发进入临界区了吧。

既然我们允许多个读者进入临界区，因此我们需要一个计数统计读者的个数。同时，由于写者永远只存在一个进入临界区，因此只需要一个bit标记是否有写进程进入临界区。所以，我们可以将两个计数合二为一。只需要1个unsigned int类型即可。最高位（bit31）代表是否有写者进入临界区，低31位（0~30bit）统计读者个数。

![微信图片_20210607153330](../../../image\微信图片_20210607153330.png)

## 实现

描述读写锁只需要1个变量即可，因此我们可以定义读写锁的结构体如下。

~~~ c
typedef struct {
        volatile unsigned int lock;
} arch_rwlock_t;
~~~

既然区分读写操作，因此肯定会有两个申请锁函数，分别是读和写。首先，我们看一下read_lock操作的实现。

~~~ c
static inline void arch_read_lock(arch_rwlock_t *rw)
{
        unsigned int tmp;

        do {
                tmp = rw->lock;
                tmp++;                            /* 1 */
        } while(tmp & (1 << 31));                 /* 2 */
        rw->lock = tmp;
}
~~~

1. 增加读者计数，最后会更新到rw->lock中。
2. 更新rw->lock前提是没有写者，因此这里会判断是否有写者已经进入临界区（判断方法是rw->lock变量bit 31的值）。如果，有写者已经进入临界区，就在这里循环。一直等到写者离开。

当读进程离开临界区的时候会调用read_unlock释放锁。read_unlock实现如下。

~~~ c
static inline void arch_read_unlock(arch_rwlock_t *rw)
{
        rw->lock--;
}
~~~

递减读者计数即可。

读操作看完了，我们看看写操作是如何实现的。arch_write_lock实现如下。

~~~ c
static inline void arch_write_lock(arch_rwlock_t *rw)
{
        unsigned int tmp;

        do {
                tmp = rw->lock;
        } while(tmp);                       /* 1 */
        rw->lock = 1 << 31;                 /* 2 */
}
~~~

1. 由于写者是排他的（读者和写者都不能有），因此这里只有rw->lock的值为0，当前的写者才可以进入临界区。
2. 置位rw->lock的bit31，代表有写者进入临界区。

当写进程离开临界区的时候会调用write_unlock释放锁。write_unlock实现如下。

~~~ c
static inline void arch_write_unlock(arch_rwlock_t *rw)
{
        rw->lock = 0;
}
~~~

同样由于写者是排他的，因此只需要将rw->lock置0即可。代表没有任何进程进入临界区。毕竟是因为同一时间只能有一个写者进入临界区，当这个写者离开临界区的时候，肯定是意味着现在没有任何进程进入临界区。

以上的代码实现其实会导致写进程饿死现象。例如，A、B、C三个进程进入读临界区，D进程尝试获得写锁，此时只能等待A、B、C三个进程退出临界区。如果在推出之前又有F、G进程进入读临界区，那么将出现D进程饿死现象。

# 互斥体（mutex）

互斥体实现了“互相排斥”（mutual exclusion）同步的简单形式（所以名为互斥体(mutex)）。互斥体禁止多个线程同时进入受保护的代码“临界区”（critical section）。因此，在任意时刻，只有一个线程被允许进入这样的代码保护区。

任何线程在进入临界区之前，必须获取（acquire）与此区域相关联的互斥体的所有权。如果已有另一线程拥有了临界区的互斥体，其他线程就不能再进入其中。这些线程必须等待，直到当前的属主线程释放（release）该互斥体。

**什么时候需要使用互斥体呢？**互斥体用于保护共享的易变代码，也就是，全局或静态数据。这样的数据必须通过互斥体进行保护，以防止它们在多个线程同时访问时损坏。　　 前文提到的semaphore在初始化count计数的时候，可以分为计数信号量和互斥信号量（二值信号量）。mutex和初始化计数为1的二值信号量有很大的相似之处。他们都可以用做资源互斥。但是mutex却有一个特殊的地方：只有持锁者才能解锁。但是，二值信号量却可以在一个进程中获取信号量，在另一个进程中释放信号量。如果是应用在嵌入式应用的RTOS，针对mutex的实现还会考虑优先级反转问题。

## 原理

既然mutex是一种二值信号量，因此就不需要像semaphore那样需要一个count计数。由于mutex具有“持锁者才能解锁”的特点，所以我们需要一个变量owner记录持锁进程。释放锁的时候必须是同一个进程才能释放。当然也需要一个链表头，主要用来便利睡眠等待的进程。原理和semaphore及其相似，因此在代码上也有体现。

## 实现

mutex的实现代码和Linux中实现会有差异，但是依然可以为你呈现设计的原理。下面的设计代码更像是部分RTOS中的代码。mutex和semaphore一样，我们需要两个类似的结构体分别描述mutex。

~~~ c
struct mutex_waiter {
        struct list_head   list;
        struct task_struct *task;
};

struct mutex {
    long   owner;
    struct list_head wait_list;
};
~~~

struct mutex_waiter的list成员是当进程无法获取互斥量的时候挂入mutex的wait_list链表。

首先实现申请互斥量的函数。

~~~ c
void mutex_take(struct mutex *mutex)
{
        struct mutex_waiter waiter;

        if (!mutex->owner) {
                mutex->owner = (long)current;           /* 1 */
                return;
        }

        waiter.task = current;
        list_add_tail(&waiter.list, &mutex->wait_list); /* 2 */
        schedule();                                     /* 2 */
}
~~~

1. 当mutex->owner的值为0的时候，代表没有任何进程持有锁。因此可以直接申请成功。然后，记录当前申请锁进程的task_struct。
2. 既然不能获取互斥量，自然就需要睡眠等待，挂入等待链表。

互斥量的释放代码实现也同样和semaphore有很多相似之处。

~~~ c
int mutex_release(struct mutex *mutex)
{
        struct mutex_waiter *waiter;

        if (mutex->owner != (long)current)                         /* 1 */
                return -1;

        if (list_empty(&mutex->wait_list)) {
                mutex->owner = 0;                                  /* 2 */
                return 0;
        }

        waiter = list_first_entry(&mutex->wait_list, struct mutex_waiter, list);
        list_del(&waiter->list);
        mutex->owner = (long)waiter->task;                         /* 3 */
        wake_up_process(waiter->task);                             /* 4 */

        return 0;
} 
~~~

1. mutex具有“持锁者才能解锁”的特点就是在这行代码体现。
2. 如果等待链表没有进程，那么自然只需要将mutex->owner置0，代表没有锁是释放状态。
3. mutex->owner的值改成当前可以持锁进程的task_struct。
4. 从等待进程链表取出第一个进程，并从链表上移除。然后就是唤醒该进程。

