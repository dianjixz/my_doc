#### 定义头文件：

~~~ c
#include <linux/wait.h>
~~~

#### 定义和初始化等待队列头（workqueue）：

静态的,用宏：

~~~ c
#define DECLARE_WAIT_QUEUE_HEAD(name) \
    wait_queue_head_t name = __WAIT_QUEUE_HEAD_INITIALIZER(name)
~~~

#### 动态的,也是用宏：

~~~ c
#define init_waitqueue_head(q)              \
    do {                        \
        static struct lock_class_key __key; \
                            \
        __init_waitqueue_head((q), #q, &__key); \
    } while (0)
~~~

#### 定义实例

~~~ c
wait_queue_head_t wq;
init_waitqueue_head(&wq);
~~~

### 阻塞接口：

~~~ c
wait_event(wq, condition)
wait_event_timeout(wq, condition, timeout)
wait_event_interruptible(wq, condition)
wait_event_interruptible_timeout(wq, condition, timeout)
wait_event_hrtimeout(wq, condition, timeout)
wait_event_interruptible_hrtimeout(wq, condition, timeout)
wait_event_interruptible_exclusive(wq, condition)
wait_event_interruptible_locked(wq, condition)
wait_event_interruptible_locked_irq(wq, condition)
wait_event_interruptible_exclusive_locked(wq, condition)
wait_event_interruptible_exclusive_locked_irq(wq, condition)
wait_event_killable(wq, condition)
wait_event_lock_irq_cmd(wq, condition, lock, cmd)
wait_event_lock_irq(wq, condition, lock)
wait_event_interruptible_lock_irq_cmd(wq, condition, lock, cmd)
wait_event_interruptible_lock_irq(wq, condition, lock)
wait_event_interruptible_lock_irq_timeout(wq, condition, lock,  timeout)
~~~

#### 参数

```
wq        定义的等待队列头，
condition 为条件表达式，当wake up后，condition为真时，唤醒阻塞的进程，为假时，继续睡眠。
```

#### 功能说明

接口版本比较多，各自都有自己合适的应用场合，但是常用的是前面四个。

~~~ c
wait_event：不可中断的睡眠，条件一直不满足，会一直睡眠。
wait_event_timeout：不可中断睡眠，当超过指定的timeout（单位是jiffies）时间，不管有没有wake up，还是条件没满足，都要唤醒进程，此时返回的是0。在timeout时间内条件满足返回值为timeout或者1；
wait_event_interruptible:可被信号中断的睡眠，被信号打断唤醒时，返回负值-ERESTARTSYS；wake up时，条件满足的，返回0。除了wait_event没有返回值，其它的都有返回，有返回值的一般都要判断返回值。如下例：

    int flag = 0;
    if(wait_event_interruptible(&wq，flag == 1))
        return -ERESTARTSYS;

wait_event_interruptible_timeout：是wait_event_timeout和wait_event_interruptible_timeout的结合版本，有它们两个的特点。
~~~

其他的接口，用的不多，有兴趣可以自己看看。

### 解除阻塞接口（唤醒）

接口定义：

~~~ c
#define wake_up(x)          __wake_up(x, TASK_NORMAL, 1, NULL)
#define wake_up_nr(x, nr)       __wake_up(x, TASK_NORMAL, nr, NULL)
#define wake_up_all(x)          __wake_up(x, TASK_NORMAL, 0, NULL)
#define wake_up_locked(x)       __wake_up_locked((x), TASK_NORMAL, 1)
#define wake_up_all_locked(x)       __wake_up_locked((x), TASK_NORMAL, 0)

#define wake_up_interruptible(x)    __wake_up(x, TASK_INTERRUPTIBLE, 1, NULL)
#define wake_up_interruptible_nr(x, nr) __wake_up(x, TASK_INTERRUPTIBLE, nr, NULL)
#define wake_up_interruptible_all(x)    __wake_up(x, TASK_INTERRUPTIBLE, 0, NULL)
#define wake_up_interruptible_sync(x)   __wake_up_sync((x), TASK_INTERRUPTIBLE, 1)
~~~

#### 功能说明

~~~ c
wake_up：一次只能唤醒挂在这个等待队列头上的一个进程
wake_up_nr：一次唤起nr个进程（等待在同一个wait_queue_head_t有很多个）
wake_up_all：一次唤起所有等待在同一个wait_queue_head_t上所有进程
wake_up_interruptible：对应wait_event_interruptible版本的wake up
wake_up_interruptible_sync：保证wake up的动作原子性，wake_up这个函数，很有可能函数还没执行完，就被唤起来进程给抢占了，这个函数能够保证wak up动作完整的执行完成。
~~~

其他的也是与对应阻塞接口对应的。

#### 使用实例

以字符设备为例，在没有数据的时候，在read函数中实现读阻塞，当向内核写入数据时，则唤醒阻塞在该等待队列的所有任务。

读操作

~~~ c
static ssize_t hello_read(struct file *filp,char __user *buf,size_t size,loff_t *poss)  
{  
    wait_event_interruptible(rwq,flage!=0);
    ……………
    flage=0;
    wake_up_interruptible(&wwq);
    return size;  
}   
~~~

写操作

~~~ c
static ssize_t hello_write(struct file *filp,const char __user *buf,size_t size,loff_t *poss)  
{  
    wait_event_interruptible(wwq,flage!=1);
    ……………
    flage=1;
    wake_up_interruptible(&rwq);
    return size;  
}    
~~~

#### 如何同步支持非阻塞？

上述操作虽然实现了阻塞功能，但是我们在应用程序打开一个字符设备的时候，有时候我们希望操作是非阻塞的，比如：

~~~ c
fd=open("/dev/hello",O_RDONLY|O_NONBLOCK); 
~~~

那么驱动如何得到这个标记呢？

参考《[手把手教Linux驱动6-inode,file,file_operations关系](http://mp.weixin.qq.com/s?__biz=MzIyMjk2NDg2NQ==&mid=2247484949&idx=1&sn=c3372c88e3e703592dd910c889c438c6&chksm=e8243bfbdf53b2ed52fac6e74066d38815b2b492cd82230ae451167288148dc1e469ebf660cb&scene=21#wechat_redirect)》，该标记会存储在结构体struct file的f_flags成员中。

所以程序可以修改如下：

~~~ c
static ssize_t hello_read(struct file *filp,char __user *buf,size_t size,loff_t *poss)  
{  
	int ret = 0;

	if(flage==0)
	{
		if(filp->f_flags & O_NONBLOCK)
		{
			
		  return -EAGAIN;
		}
		wait_event_interruptible(rwq,flage!=0);
	}	
        ……………
	flage=0;
	wake_up_interruptible(&wwq);
    return size;  
}
~~~

#### 一种灵活的添加删除等待队列头中的等待队列：

（1）定义：
静态：

~~~ c
#define DECLARE_WAITQUEUE(name, tsk)                    \
    wait_queue_t name = __WAITQUEUE_INITIALIZER(name, tsk)
~~~

（2）动态：

~~~ c
wait_queue_t wa;
init_waitqueue_entry(&wa,&tsk);
~~~

tsk是进程结构体，一般是current（linux当前进程就是用这个获取）。还可以用下面的，设置自定义的等待队列回调函数,上面的是linux默认的一个回调函数default_wake_function(),不过默认的用得最多：

~~~ c
wait_queue_t wa;
wa->private = &tsk;
int func(wait_queue_t *wait, unsigned mode, int flags, void *key)
{
    //
}
init_waitqueue_func_entry(&wa,func);
~~~

（回调有什么作用？）
用下面函数将等待队列，加入到等待队列头（带remove的是从工作队列头中删除工作队列）：

~~~ c
extern void add_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
extern void add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait);
extern void remove_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
~~~

上面的阻塞和解除阻塞接口，只能是对当前进程阻塞/解除阻塞,有了这几个灵活的接口，我们可以单独定义一个等待队列，只要获取进程task_struct指针，我们可以将任何进程加入到这个等待队列，然后加入到等待队列头，我们能将其它任何进程（不仅仅是当前进程），挂起睡眠，当然唤醒时，如果用wake_up_all版本的话，也会一同唤起。这种情况，阻塞不能用上面的接口了，我们需要用下一节讲述的接口（schedule()），解除阻塞可以用wake_up,wake_up_interruptible等。

#### 更高级灵活的阻塞：

阻塞当前进程的原理：用函数set_current_state()修改当前进程为TASK_INTERRUPTIBLE（不可中断睡眠）或TASK_UNINTERRUPTIBLE（可中断睡眠）状态，然后调用schedule()告诉内核重新调度，由于当前进程状态已经为睡眠状态，自然就不会被调度。schedule()简单说就是告诉内核当前进程主动放弃CPU控制权。这样来，就可以说当前进程在此处睡眠，即阻塞在这里。

在上一小节“灵活的添加删等待队列头中的等待队列”，将任意进程加入到waitqueue，然后类似用：

~~~ c
task_struct *tsk;
wait_queue_t wa;
//假设tsk已经指向某进程控制块
p->state = TASK_INTERRUPTIBLE;//or TASK_UNINTERRUPTIBLE
init_waitqueue_entry(&wa,&tsk);
~~~

就能将任意进程挂起，当然，还需要将wa，挂到等待队列头，然后用wait_event(&wa),进程就会从就绪队列中退出，进入到睡眠队列，直到wake up时，被挂起的进程状态被修改为TASK_RUNNING，才会被再次调度。（主要是schedule()下面会说到）。

## wait_event实现原理：

先看下wait_event实现：

~~~ c
#define __wait_event(wq, condition)                     \
do {                                    \
    DEFINE_WAIT(__wait);                        \
                                    \
    for (;;) {                          \
        prepare_to_wait(&wq, &__wait, TASK_UNINTERRUPTIBLE);    \
        if (condition)                      \
            break;                      \
        schedule();                     \
    }                               \
    finish_wait(&wq, &__wait);                  \
} while (0)

#define wait_event(wq, condition)                   \
do {                                    \
    if (condition)                          \
        break;                          \
    __wait_event(wq, condition);                    \
} while (0)
~~~

#### DEFINE_WAIT：

  定义一个工作队列。

#### prepare_to_wait：

~~~ c
定义：void prepare_to_wait(wait_queue_head_t *q, wait_queue_t *wait, int state)
功能：将工作队列wait加入到工作队列头q，并将当前进程设置为state指定的状态，一般是TASK_UNINTERRUPTIBLE或TASK_INTERRUPTIBLE状态（在这函数里有调用set_current_state）。
    第一个参数：工作队列头
    第二个参数：工作队列
    第三个参数：当前进程要设置的状态
~~~

**finish_wait**：  

  用了prepare_to_wait之后，当退出时，一定要用这个函数清空等待队列。

**功能：**

1.  该函数首先调用prepare_to_wait，修改进程到睡眠状态，
2.  条件不满足，schedule()就放弃CPU控制权，睡眠，
3.  当wake up的时候，阻塞在wq（也可以说阻塞在wait_event处）等待队列头上的进程，再次得到运行，接着执行schedule()后面的代码，这里，显然是个循环，prepare_to_wait再次设置当前进程为睡眠状态，然后判断条件是否满足，
4. 满足就退出循环，finish_wait将当前进程恢复到TASK_RUNNING状态，也就意味着阻塞解除。不满足，继续睡下去。如此反复等待条件成立。


明白这个过程，用prepare_to_wait和schedule()来实现更为灵活的阻塞，就很简单了，解除阻塞和前面的一样用wake_up,wake_up_interruptible等。

下面是wake_up和wait_event流程图：

![微信图片_20210607165236](../../../image\微信图片_20210607165236.jpg)

**独占等待
**

------

  当调用wake_up时，所有等待在该队列上的进程都被唤醒，并进入可运行状态如果只有一个进程可获得资源，此时，其他的进程又将再次进入休眠，如果数量很大，被称为”**疯狂售群**”。这样会非常占用系统资源。



解决方法：

wait_queue_t成员flage有个重要的标志WQ_FLAG_EXCLUSIVE，表示：

```
当一个等待队列入口有 WQ_FLAG_EXCLUSEVE 标志置位, 它被添加到等待队列的尾部. 没有这个标志的入口项, 添加到开始.
当 wake_up 被在一个等待队列上调用, 它在唤醒第一个有WQ_FLAG_EXCLUSIVE 标志的进程后停止.
```

wait_event默认总是将waitqueue加入开始，而wake_up时总是一个一个的从开始处唤醒，如果不断有waitqueue加入，那么最开始加入的，就一直得不到唤醒，有这个标志，就避免了这种情况。

prepare_to_wait_exclusive()就是加入了这个标志的。

### 补充

```
Linux将进程状态描述为如下五种：
1. TASK_RUNNING：可运行状态。处于该状态的进程可以被调度执行而成为当前进程。
2. TASK_INTERRUPTIBLE：可中断的睡眠状态。处于该状态的进程在所需资源有效时被唤醒，也可以通过信号或定时中断唤醒（因为有signal_pending()函数）。
3. TASK_UNINTERRUPTIBLE：不可中断的睡眠状态。处于该状态的进程仅当所需资源有效时被唤醒。
4. TASK_ZOMBIE：僵尸状态。表示进程结束且已释放资源，但其task_struct仍未释放。
5. TASK_STOPPED：暂停状态。处于该状态的进程通过其他进程的信号才能被唤醒
```

![微信图片_20210607165312](../../../image\微信图片_20210607165312.png)

#### Linux 通过结构体task_struct维护所有运行的线程、进程，不同状态的任务，会由不同的队列进行维护，schedule()函数就负责根据这些状态的变化调度这些任务。关于进程的调度，后续会新开文章详细介绍。

## 实例

下面实例主要功能是基于我们之前课程《[手把手教Linux驱动3-之字符设备架构详解，有这篇就够了](http://mp.weixin.qq.com/s?__biz=MzUxMjEyNDgyNw==&mid=2247486317&idx=2&sn=b8e5ee6153e03dae32e53970c81eddf0&chksm=f9687999ce1ff08f881907d59cac8387340829875a0e6e3b5da7445a515c64de798b96c9f259&scene=21#wechat_redirect)》最后的代码实例，在此基础上增加写阻塞的功能。

1. 内核中有缓冲内存，以及是否可以访问的标记；

~~~ c
int flage=0;  //1：有数据可读  0：无数据，不可读
char kbuf[128];
~~~

1. 初始状态下flage为0，没有数据；
2. 应用进程读取数据会调用到内核函数hello_read()，如果flage为1，则直接读走数据，并将改flage置1，如果flage为0，则进程阻塞,直到有进程写入数据将该flage置1；
3. 应用进程每次写入数据会调用到内核函数hello_write()，如果flage为0，则直接写入数据，并设置flage为1，如果为1，则阻塞，直到有其他进程调用到读函数hello_read()将flage置0。

#### 驱动

~~~ c
/********************************************* 
*hellodev.c 
*********************************************/  
#include <linux/module.h>  
#include <linux/ioctl.h>  
#include <linux/wait.h> 
#include <linux/poll.h> 
#include <linux/device.h> 
#include <linux/types.h>  
#include <linux/fs.h>  
#include <linux/errno.h>  
#include <linux/mm.h>  
#include <linux/sched.h>  
#include <linux/init.h>  
#include <linux/cdev.h>  
#include <asm/io.h>  
#include <asm/system.h>  
#include <asm/uaccess.h>  
 #include<linux/slab.h> 
  
static int hello_major = 250;  
static struct class *hello_class;
#define  DEV_NAME "hello_cls" 
module_param(hello_major,int,S_IRUGO);  
dev_t devno;

struct cdev cdev;  
int num;
int flage=0;
char kbuf[128];
wait_queue_head_t rwq; //read wq
wait_queue_head_t wwq;  //write wq
int hello_open(struct inode *inode,struct file *filp)  
{  
 
  return 0;  
}  
int hello_release(struct inode *inode,struct file *filp)  
{  
    return 0;  
}  
static ssize_t hello_read(struct file *filp,char __user *buf,size_t size,loff_t *poss)  
{  
  int ret = 0;

  if(flage==0)
  {
    if(filp->f_flags & O_NONBLOCK)
    {
      
      return -EAGAIN;
    }
    wait_event_interruptible(rwq,flage!=0);
  }  
  if(copy_to_user(buf,kbuf,size)) 
  {
    return -EFAULT;
  }
  flage=0;
  wake_up_interruptible(&wwq);
    return size;  
}   
static ssize_t hello_write(struct file *filp,const char __user *buf,size_t size,loff_t *poss)  
{  
    int ret = 0;
  if(size>128||size<0)
  {
    return -EINVAL;
  }
  wait_event_interruptible(wwq,flage!=1);

   if(copy_from_user(kbuf,buf,size)) 
  {
    return -EFAULT;
  }
  flage=1;
  wake_up_interruptible(&rwq);
    return size;  
}  
 
static const struct file_operations hello_fops =  
{  
    .owner  = THIS_MODULE,  
    .read   = hello_read,  
    .write  = hello_write,  
    .open   = hello_open,  
    .release = hello_release,  
};   
static int hellodev_init(void)  
{  
  int result;  
  int i;  
  struct device *hello_dev;

  devno = MKDEV(hello_major,0);  
  if(hello_major)  
      result = register_chrdev_region(devno,2,"hello");  
  else    
  {  
      result = alloc_chrdev_region(&devno,0,2,"hello");  
      hello_major = MAJOR(devno);  
  }  
  if(result < 0)  
      return result;    

  cdev_init(&cdev,&hello_fops);  
  cdev.owner = THIS_MODULE;  
  cdev.ops = &hello_fops;    

  cdev_add(&cdev,MKDEV(hello_major,0),1);  
  init_waitqueue_head(&rwq);
  init_waitqueue_head(&wwq);

  hello_class = class_create(THIS_MODULE, DEV_NAME);//类名字 
  if (IS_ERR(hello_class)) {
    printk(KERN_WARNING "class_create faihello ! \n");
    goto err_3;
  } 
  hello_dev = device_create(hello_class, NULL, devno, NULL, "hello");
  if (IS_ERR(hello_dev)) {
    printk(KERN_WARNING "device_create faihello! \n");
    goto err_4;
  }
  return 0; 
err_4:
  class_destroy(hello_class);
err_3:
  cdev_del(&cdev);
  unregister_chrdev_region(MKDEV(hello_major,0),2);   
  return 0;  
}  
static void hellodev_exit(void)  
{ 
  device_destroy(hello_class, devno);  
  class_destroy(hello_class);
  cdev_del(&cdev);
  unregister_chrdev_region(MKDEV(hello_major,0),2);  
}    
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("yikoulinux");
module_init(hellodev_init);  
module_exit(hellodev_exit);  
~~~

#### 测试程序

read.c

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/select.h>

int main()
{
  int fd= 0;
  char buf[128];
  int num;
//  fd=open("/dev/hello",O_RDONLY); 阻塞方式读取 
  fd=open("/dev/hello",O_RDONLY|O_NONBLOCK);  //非阻塞   
  if(fd<0) 
  {     
    printf("open memdev failed!\n");     
    return -1;               
  }     
  read(fd,buf,sizeof(buf));  
  
  printf("num:%s\n",buf);  
  close(fd);
  return 0;     
  
}
~~~

write.c

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/select.h>
int main()
{
  int fd =0;
  char buf[128]="hello yikouLlinux";
  int num;

  fd=open("/dev/hello",O_RDWR);     
  if(fd <0)     
  {     
    printf("open device failed!\n");     
    return -1;               
  }     
  write(fd,buf,sizeof(buf));  
  close(fd);
  return 0;     
  
}
~~~

掌握了等待队列的用法，后面我们就可以进行中断的讲解了。