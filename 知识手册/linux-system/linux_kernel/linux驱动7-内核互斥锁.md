# 互斥体概述

信号量是在并行处理环境中对多个处理器访问某个公共资源进行保护的机制，mutex用于互斥操作。信号量的count初始化为1，down()/up()也可以实现类似mutex的作用。

mutex的语义相对于信号量要简单轻便一些，在锁争用激烈的测试场景下，mutex比信号量执行速度更快，可扩展性更好，另外mutex数据结构的定义比信号量小。

# **mutex的优点**

1. mutex和信号量相比要高效的多：
2. mutex最先实现自旋等待机制
3. mutex在睡眠之前尝试获取锁
4. mutex实现MCS所来避免多个CPU争用锁而导致CPU高速缓存颠簸现象。

# **mutex的使用注意事项：**

1. 同一时刻只有一个线程可以持有mutex。
2. 只有锁持有者可以解锁。不能再一个进程中持有mutex，在另外一个进程中释放他。
3. 不允许递归地加锁和解锁。
4. 当进程持有mutex时，进程不可以退出。
5. mutex必须使用官方API来初始化。
6. mutex可以睡眠，所以不允许在中断处理程序或者中断下半部中使用，例如tasklet、定时器等。

**目录：**

~~~ c
/linux/include/linux/mutex.h

/*
 * Simple, straightforward mutexes with strict semantics:
 *
 * - only one task can hold the mutex at a time
 * - only the owner can unlock the mutex
 * - multiple unlocks are not permitted
 * - recursive locking is not permitted
 * - a mutex object must be initialized via the API
 * - a mutex object must not be initialized via memset or copying
 * - task may not exit with mutex held
 * - memory areas where held locks reside must not be freed
 * - held mutexes must not be reinitialized
 * - mutexes may not be used in hardware or software interrupt
 *   contexts such as tasklets and timers
 *
 * These semantics are fully enforced when DEBUG_MUTEXES is
 * enabled. Furthermore, besides enforcing the above rules, the mutex
 * debugging code also implements a number of additional features
 * that make lock debugging easier and faster:
 *
 * - uses symbolic names of mutexes, whenever they are printed in debug output
 * - point-of-acquire tracking, symbolic lookup of function names
 * - list of all locks held in the system, printout of them
 * - owner tracking
 * - detects self-recursing locks and prints out all relevant info
 * - detects multi-task circular deadlocks and prints out all affected
 *   locks and tasks (and only those tasks)
 */
struct mutex {
  /* 1: unlocked, 0: locked, negative: locked, possible waiters */
  atomic_t    count;
  spinlock_t    wait_lock;
  struct list_head  wait_list;
#if defined(CONFIG_DEBUG_MUTEXES) || defined(CONFIG_SMP)
  struct task_struct  *owner;
#endif
#ifdef CONFIG_MUTEX_SPIN_ON_OWNER
  void      *spin_mlock;  /* Spinner MCS lock */
#endif
#ifdef CONFIG_DEBUG_MUTEXES
  const char     *name;
  void      *magic;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
  struct lockdep_map  dep_map;
#endif
};
~~~

**作用及访问规则：**

1. 互斥锁主要用于实现内核中的互斥访问功能。内核互斥锁是在原子 API 之上实现的，但这对于内核用户是不可见的。
2. 对它的访问必须遵循一些规则：同一时间只能有一个任务持有互斥锁，而且只有这个任务可以对互斥锁进行解锁。互斥锁不能进行递归锁定或解锁。一个互斥锁对象必须通过其API初始化，而不能使用memset或复制初始化。一个任务在持有互斥锁的时候是不能结束的。互斥锁所使用的内存区域是不能被释放的。使用中的互斥锁是不能被重新初始化的。并且互斥锁不能用于中断上下文。
3. 互斥锁比当前的内核信号量选项更快，并且更加紧凑。

# **互斥体的使用**

## 初始化

静态定义如下：

~~~ c
DEFINE_MUTEX(name);
~~~

动态初始化mutex，如下：

~~~ c
mutex_init(&mutex);
~~~

具体实现如下：

~~~ c
#define mutex_init(mutex) \
do {       \
 static struct lock_class_key __key;  \
       \
 __mutex_init((mutex), #mutex, &__key);  \
} while (0)

void
__mutex_init(struct mutex *lock, const char *name, struct lock_class_key *key)
{
 atomic_set(&lock->count, 1);
 spin_lock_init(&lock->wait_lock);
 INIT_LIST_HEAD(&lock->wait_list);
 mutex_clear_owner(lock);
#ifdef CONFIG_MUTEX_SPIN_ON_OWNER
 lock->spin_mlock = NULL;
#endif

 debug_mutex_init(lock, name, key);
}
~~~

## 申请互斥锁

mutex操作列表如下：

| 方法                         | 描述                                                         |
| ---------------------------- | ------------------------------------------------------------ |
| mutex_lock(struct mutex*)    | 为指定的mutex上锁，如果不可用则睡眠                          |
| mutex_unlock(struct mutex*)  | 为指定的mutex解锁                                            |
| mutex_trylock(struct mutex*) | 视图获取指定的mutex，如果成功则返回1；否则锁被获取，返回值是0 |
| mutex_is_lock(struct mutex*) | 如果锁已被征用，则返回1；否则返回0                           |

mutex的简洁性和高效性源自于相比使用信号量更多的受限性。它不同于信号量，因为mutex仅仅实现了Dijkstra设计初衷中的最基本的行为。因此mutex的使用场景相对而言更严格。

(1)代码：linux/kernel/mutex.c

~~~ c
void inline fastcall __sched mutex_lock(struct mutex *lock);   
 //获取互斥锁。
~~~

实际上是先给count做自减操作，然后使用本身的自旋锁进入临界区操作。首先取得count的值，在将count置为－1，判断如果原来count的置为1，也即互斥锁可以获得，则直接获取，跳出。否则进入循环反复测试互斥锁的状态。在循环中，也是先取得互斥锁原来的状态，在将其之为－1，判断如果可以获取(等于1)，则退出循环，否则设置当前进程的状态为不可中断状态，解锁自身的自旋锁，进入睡眠状态，待被在调度唤醒时，再获得自身的自旋锁，进入新一次的查询其自身状态(该互斥锁的状态)的循环。

(2)具体参见linux/kernel/mutex.c

~~~ c
int fastcall __sched mutex_lock_interruptible(struct mutex *lock)；
~~~

和mutex_lock()一样，也是获取互斥锁。在获得了互斥锁或进入睡眠直到获得互斥锁之后会返回0。如果在等待获取锁的时候进入睡眠状态收到一个信号(被信号打断睡眠)，则返回_EINIR。

(3)具体参见linux/kernel/mutex.c

~~~ c
int fastcall __sched mutex_trylock(struct mutex *lock);
~~~

试图获取互斥锁，如果成功获取则返回1，否则返回0，不等待。

## 释放互斥锁

具体参见linux/kernel/mutex.c

~~~ c
void fastcall mutex_unlock(struct mutex *lock);
~~~

释放被当前进程获取的互斥锁。该函数不能用在中断上下文中，而且不允许去释放一个没有上锁的互斥锁。

## 互斥锁试用注意事项

1. 任何时刻中只有一个任务可以持有mutex, 也就是说，mutex的使用计数永远是1
2. 给mutex锁者必须负责给其再解锁——你不能在一个上下文中锁定一个mutex，而在另 一个上下文中给它解锁。这个限制使得mutex不适合内核同用户空间复杂的同步场景。最 常使用的方式是：在同一上下文中上锁和解锁。
3. 递归地上锁和解锁是不允许的。也就是说，你不能递归地持有同一个锁，同样你也不能再去解锁一个已经被解开的mutex
4. 当持有一个mutex时 ，进程不可以退出
5. mutex不能在中断或者下半部中使用，即使使用mutex_trylock()也不行
6. mutex只能通过官方API管理：它只能使用上节中描述的方法初始化，不可被拷贝、手动 初始化或者重复初始化

## 信号量和互斥体

互斥体和信号量很相似，内核中两者共存会令人混淆。所幸，它们的标准使用方式都有简单规范：除非mutex的某个约束妨碍你使用，否则相比信号量要优先使用mutex。当你写新代码时，只有碰到特殊场合（一般是很底层代码）才会需要使用信号量。因此建议 选mutex。如果发现不能满足其约束条件，且没有其他别的选择时，再考虑选择信号量

## 自旋锁和互斥体使用场合

了解何时使用自旋锁，何时使用互斥体（或信号量）对编写优良代码很重要，但是多数情况下，并不需要太多的考虑，因为在中断上下文中只能使用自旋锁，而在任务睡眠时只能使用互斥体。

下面总结一下各种锁的需求情况

| 需求             | 建议的加锁方法 |
| ---------------- | -------------- |
| 低开销加锁       | 优先使用自旋锁 |
| 短期锁定         | 优先使用自旋锁 |
| 长期锁定         | 优先使用互斥体 |
| 中断上下文中加锁 | 使用自旋锁     |
| 持有锁需要睡眠   | 使用互斥体     |

## **互斥锁锁定和解锁使用实例**

使用方法如下：

~~~ c
1. struct mutex mutex;
2. mutex_init(&mutex); /*定义*/
3. //加锁
4. mutex_lock(&mutex);
5.  
6. //临界区
7. 
8. //解锁
9. mutex_unlock(&mutex);
~~~

可以看出，互斥体就是一个简化版的信号量，因为不再需要管理任何使用计数。

下面网卡DM9000的驱动，其中写入eeprom的操作试用了mutex机制：

~~~ c
static void
dm9000_write_eeprom(board_info_t *db, int offset, u8 *data)
{
 unsigned long flags;

 if (db->flags & DM9000_PLATF_NO_EEPROM)
  return;

 mutex_lock(&db->addr_lock);

 spin_lock_irqsave(&db->lock, flags);
 iow(db, DM9000_EPAR, offset);
 iow(db, DM9000_EPDRH, data[1]);
 iow(db, DM9000_EPDRL, data[0]);
 iow(db, DM9000_EPCR, EPCR_WEP | EPCR_ERPRW);
 spin_unlock_irqrestore(&db->lock, flags);

 dm9000_wait_eeprom(db);

 mdelay(1); /* wait at least 150uS to clear */

 spin_lock_irqsave(&db->lock, flags);
 iow(db, DM9000_EPCR, 0);
 spin_unlock_irqrestore(&db->lock, flags);

 mutex_unlock(&db->addr_lock);
}
~~~

可以看到每次驱动向eeprom写入数据（访问临界资源），都需要首先获得该资源对应的互斥锁db->addr_lock,并且使用完毕必须释放该锁。

