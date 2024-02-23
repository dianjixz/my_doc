# 什么是文件描述符？

Linux 中一切都可以看作文件，包括普通文件、链接文件、Socket 以及设备驱动等，对其进行相关操作时，都可能会创建对应的文件描述符。文件描述符（file descriptor）是内核为了高效管理已被打开的文件所创建的索引，用于指代被打开的文件，对文件所有 I/O 操作相关的系统调用都需要通过文件描述符。

Linux启动后，会默认打开3个文件描述符，分别是：

**0：标准输入  standard input**

**1：正确输出  standard output**

**2：错误输出  error output**

这就是为什么我们在程序运行时可以直接打印信息和从命令终端获取信息的原因。

并且以后打开文件后。新增文件绑定描述符 可以依次增加（从3开始累加）。每一条shell命令执行，都会继承父进程的文件描述符。因此，所有运行的shell命令，都会有默认3个文件描述符。

![微信图片_20210607142833](../../../image\微信图片_20210607142833.jpg)

   **进程级别的文件描述符表****files_struct**：内核为每个进程维护一个文件描述符表，该表记录了文件描述符的相关信息，包括文件描述符、指向打开文件表中记录的指针。

   **系统级别的打开文件表file**：内核对所有打开文件维护的一个进程共享的打开文件描述表，表中存储了处于打开状态文件的相关信息，包括文件类型、访问权限、文件操作函数(file_operations)等。

   **系统级别的 i-node 表**：i-node 结构体记录了文件相关的信息，包括文件长度，文件所在设备，文件物理位置，创建、修改和更新时间等，"ls -i" 命令可以查看文件 i-node 节点。

进程在Linux内核中是由结构体task_struct维护，进程打开的所有文件描述符都在进程维护的结构体task_struct的files变量中维护：

 ~~~ c
 //include\linux\sched.h
 
 struct task_struct {
       ……
   /* open file information */
       struct files_struct *files;
       ……
 }
 ~~~

该结构体定义如下：

 ~~~ c
 /*
  * Open file table structure
  */
 struct files_struct {
   /*
    * read mostly part
    */
     atomic_t count;
     struct fdtable __rcu *fdt;
     struct fdtable fdtab;
   /*
    * written part on a separate cache line in SMP
    */
     spinlock_t file_lock ____cacheline_aligned_in_smp;
     int next_fd;
     unsigned long close_on_exec_init[1];
     unsigned long open_fds_init[1];
     struct file __rcu * fd_array[NR_OPEN_DEFAULT];
 };
 ~~~

该进程所有打开的文件对应的file指针均由fd_array维护，文件描述符和数组下标一一对应。

# 进程打开文件相关信息管理

文件描述符是一种系统资源，可以通过以下命令来查看文件描述符的上限。

**查看所有进程允许打开的最大 fd 数量**

> root@ubunutu:/sys/class# cat /proc/sys/fs/file-max
>
> 100984

**查看所有进程已经打开的 fd 数量以及允许的最大数量**

> root@ubunutu:/sys/class# cat /proc/sys/fs/file-nr
>
> 6368	0	100984

**查看单个进程允许打开的最大 fd 数量.**

> root@ubunutu:/sys/class# ulimit -n
>
> 1024

**查看某个文件被哪些进程打开？**

可以借助lsof命令

编写调试代码如下：

~~~ c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
    int fd;
    fb = open("test",O_RDWR|O_CREAT);
    if(fd < 0)
    {
        perror("open fail \n");
        return -1;
    }
    sleep(100);
    printf("open ok \n");
    return 0;
}
~~~

该代码功能是打开文件test，然后休眠100秒，我们需要在这100秒内执行lsof操作。

~~~ bash
root@ubuntu:~/# ./a.out &
[1] 7298
root@ubuntu:~/# lsof test
COMMAND	PID	USER	FD	TYPE	DEVICE	SIZE/OFF	NOOE NAME
a.out	7298 root	3u	REG		8,1		0			939477	test
~~~

&是程序放在后台运行,为了释放终端，方便输入下一个命令；

7284：程序进程ID；

lsof功能：查看某个文件被进程打开的详细信息。

**查看某个进程打开了哪些文件？**

接着上述的例子，ls -l /proc/{PID}/fd 可以查看某个进程打开了哪些文件。

> ls -l /proc/7298/fd

可以看到该进程打开了除了test之外，还打开了前面所述的3个默认文件,结构体对应关系

![微信图片_20210607144057](../../../image\微信图片_20210607144057.png)

如下：

`实际开发中，可能会遇到 fd 资源超过上限导致的 "Too many open files" 之类的问题，一般都是因为没有及时释放掉 fd，若循环执行超过单个进程允许打开的最大 fd 数量，程序就会出现异常。`

# Linux设备文件三大结构：inode,file,file_operations

驱动程序就是向下控制硬件，向上提供接口，驱动向上提供的接口最终对应到应用层有三种方式：**设备文件，/proc，/sys**，其中最常用的就是使用设备文件，而Linux设备中用的最多的就是字符设备，本文就以字符设备为例来分析创建并打开一个字符设备的文件内部机制。

**struct inode**

  Linux中一切皆文件，当我们在Linux中创建一个文件时，就会在相应的文件系统创建一个inode与之对应。

  对于不同的文件类型，inode被填充的成员内容也会有所不同，以创建字符设备为例，我们知道，add_chrdev_region其实是把一个**驱动对象**和一个(一组)**设备号**联系到一起。而创建设备文件，其实是把**设备文件**和**设备号**联系到一起。至此，这三者就被绑定在一起了。这样，内核就有能力创建一个**struct inode**实例了，下面是Linux 3.14内核中的inode。这个inode是VFS的inode，是最具体文件系统的inode的进一步封装，也是驱动开发中关心的inode，针对具体的文件系统，还有struct ext2_inode_info 等结构。

~~~ c
//include/linux/fs.h 596
/*
 * Keep mostly read-only and often accessed (especially for
 * the RCU path lookup and 'stat' data) fields at the beginning
 * of the 'struct inode'
 */
struct inode {
  umode_t      i_mode;           //表示访问权限控制
  unsigned short    i_opflags;
  kuid_t      i_uid;             //用户ID
  kgid_t      i_gid;            //用户组ID
  unsigned int    i_flags;      //文件系统标志

#ifdef CONFIG_FS_POSIX_ACL
  struct posix_acl  *i_acl;
  struct posix_acl  *i_default_acl;
#endif

  const struct inode_operations  *i_op;
  struct super_block  *i_sb;
  struct address_space  *i_mapping;

#ifdef CONFIG_SECURITY
  void      *i_security;
#endif

  /* Stat data, not accessed from path walking */
  unsigned long    i_ino;
  /*
   * Filesystems may only read i_nlink directly.  They shall use the
   * following functions for modification:
   *
   *    (set|clear|inc|drop)_nlink
   *    inode_(inc|dec)_link_count
   */
  union {                       //硬链接数计数
    const unsigned int i_nlink;
    unsigned int __i_nlink;
  };
  dev_t      i_rdev;          //设备号
  loff_t      i_size;         //以字节为单位的文件大小
  struct timespec    i_atime; //最后access时间
  struct timespec    i_mtime; //最后modify时间
  struct timespec    i_ctime; //最后change时间
  spinlock_t    i_lock;  /* i_blocks, i_bytes, maybe i_size */
  unsigned short          i_bytes;
  unsigned int    i_blkbits;
  blkcnt_t    i_blocks;

#ifdef __NEED_I_SIZE_ORDERED
  seqcount_t    i_size_seqcount;
#endif

  /* Misc */
  unsigned long    i_state;
  struct mutex    i_mutex;

  unsigned long    dirtied_when;  /* jiffies of first dirtying */

  struct hlist_node  i_hash;
  struct list_head  i_wb_list;  /* backing dev IO list */
  struct list_head  i_lru;    /* inode LRU list */
  struct list_head  i_sb_list;
  union {
    struct hlist_head  i_dentry;//目录项链表
    struct rcu_head    i_rcu;
  };
  u64      i_version;
  atomic_t    i_count;//引用计数,当引用计数变为0时，会释放inode实例
  atomic_t    i_dio_count;
  atomic_t    i_writecount;//写者计数
  const struct file_operations  *i_fop;  /* former ->i_op->default_file_ops */
  struct file_lock  *i_flock;
  struct address_space  i_data;
#ifdef CONFIG_QUOTA  
//创建设备文件的时候i_fops填充的是def_chr_fops,
//blk_blk_fops,def_fifo_fops,bad_sock_fops之一,
//参见创建过程中调用的init_special_inode()
  struct dquot    *i_dquot[MAXQUOTAS];
#endif
  struct list_head  i_devices;
  union {
  //特殊文件类型的union,pipe,cdev,blk.link etc,
  //i_cdev表示这个inode属于一个字符设备文件，
  //本文中创建设备文件的时候会把与之相关的设备号的驱动对象cdev拿来填充
    struct pipe_inode_info  *i_pipe;
    struct block_device  *i_bdev;
    struct cdev    *i_cdev;
  };

  __u32      i_generation;

#ifdef CONFIG_FSNOTIFY
  __u32      i_fsnotify_mask; /* all events this inode cares about */
  struct hlist_head  i_fsnotify_marks;
#endif

#ifdef CONFIG_IMA
  atomic_t    i_readcount; /* struct files open RO */
#endif
 //inode的私有数据
  void      *i_private; /* fs or device private pointer */
};

~~~

重要的成员已经添加注释。

**struct file**

Linux内核会为每一个进程维护一个文件描述符表，这个表其实就是struct file[]的索引。open()的过程其实就是根据传入的路径填充好一个file结构并将其赋值到数组中并返回其索引。下面是file的主要内容

~~~ c
struct file {
    union {
        struct llist_node    fu_llist;
        struct rcu_head     fu_rcuhead;
    } f_u;
    struct path        f_path;
#define f_dentry    f_path.dentry
    struct inode        *f_inode;    /* cached value */
    const struct file_operations    *f_op;
    /*
     * Protects f_ep_links, f_flags.
     * Must not be taken from IRQ context.
     */
    spinlock_t        f_lock;
    atomic_long_t        f_count;
    unsigned int         f_flags;
    fmode_t            f_mode;
    struct mutex        f_pos_lock;
    loff_t            f_pos;
    struct fown_struct    f_owner;
    const struct cred    *f_cred;
    struct file_ra_state    f_ra;
    u64            f_version;
#ifdef CONFIG_SECURITY
    void            *f_security;
#endif
    /* needed for tty driver, and maybe others */
    void            *private_data;

#ifdef CONFIG_EPOLL
    /* Used by fs/eventpoll.c to link all the hooks to this file */
    struct list_head    f_ep_links;
    struct list_head    f_tfile_llink;
#endif /* #ifdef CONFIG_EPOLL */
    struct address_space    *f_mapping;
#ifdef CONFIG_DEBUG_WRITECOUNT
    unsigned long f_mnt_write_state;
#endif
} __attribute__((aligned(4)));    
/* lest something weird decides that 2 is OK */
~~~

关键成员定义如下：

**struct file**

`-->f_path里存储的是open传入的路径，VFS就是根据这个路径逐层找到相应的inode`

`-->f_inode里存储的是找到的inode`

`-->f_op里存储的就是驱动提供的file_operations对象，这个对象应该在第一次open()的时候被填充，具体地，应用层的open通过层层搜索会调用**inode.i_fops->open()**，即我们注册的open接口函数**chrdev_open()**`

`-->f_count的作用是记录对文件对象的引用计数，也即当前有多少个使用CLONE_FILES标志克隆的进程在使用该文件。典型的应用是在POSIX线程中。就像在内核中普通的引用计数模块一样，最后一个进程调用put_files_struct()来释放文件描述符。`

`-->f_flags当打开文件时指定的标志，对应系统调用open的int flags，比如驱动程序为了支持非阻塞型操作需要检查这个标志是否有O_NONBLOCK。`

`-->f_mode;对文件的读写模式，对应系统调用open的mod_t mode参数，比如O_RDWR。如果驱动程序需要这个值，可以直接读取这个字段。`

`-->private_data表示file结构的私有数据`

# mknod 做了什么事？

本例假定我们创建两个串口com0、com1，他们公用同一个主设备号250，次设备号分别为0、1，他们公用同一个字符设备驱动，那么我们的驱动要能够根据应用进程打开的是设备com0还是com1来操作不同的串口。

首先创建两个设备节点：

~~~ bash
mknod /dev/com0 c 250 0
mknod /dev/com1 c 250 1
~~~

内核为了维护这两个文件节点，内核需要创建结构体维护这两个文件，具体如下图所示：

![微信图片_20210607144733](../../../image\微信图片_20210607144733.jpg)

当我们通过命令mknod创建一个字符设备文件，那么内核就会创建好一个inode会存在存储器中，创建和该文件实体一一对应的inode。这个inode和其他的inode一样，通常用来存储关于这个文件的静态信息(不变的信息)，包括这个设备文件对应的设备号，文件的路径以及对应的驱动对象等。

inode作为VFS四大对象之一，在驱动开发中很少需要自己进行填充，更多的是在open()方法中进行查看并根据需要填充我们的file结构。

创建字符设备 /dev/com0、 /dev/com1,只是增加了对应的inode节点，此时VFS层并没有并没有创建file结构体，而且inode和驱动也并没有产生联系。

# 进程打开设备文件发生了什么？


 当进程试图打开设备文件的时候，系统做了什么事？ 

 如果应用程序执行以下代码：

~~~ c
fd0 = open("/dev/com0",O_RDWR);
fd1 = open("/dev/com1",O_RDWR);
~~~

各个结构体之间关系入下图所示：

![微信图片_20210607144903](../../../image\微信图片_20210607144903.jpg)

当应用程序执行open函数，该函数会调用到内核的sys_open（），该函数会根据该设备节点inode保存的信息，i_flags:文件类型, i_rdev:设备号，初始化结构体inode其他信息，比如inode->i_cdev，此时已经指向我们注册的cdev结构体。

通过设备号，可以很容易找到该设备在设备号全局管理数组chedevs[]的下标，进而找到我们注册的驱动cdev以及file_operations。

同时内核会在VFS层为创建结构体file，该函数调用成功之后，应用层会返回整型值用来和该file对应，就是上图的文件描述符fd0、fd1。

其中：

~~~ c
file->f_dentry->d_inode->i_rdev  保存对应的设备节点的设备号，
file-> f_op保存我们注册的file_operations 字符设备接口函数集合。
~~~

由此可得在read和write等其他接口函数中，我们可以通过file来得到次设备号。

【注意】同一个文件如果打开了两次，那么第二次linux内核仍然会重新分配1个新的file结构体和文件描述符。

# 驱动如何支持多种同类型设备

对于同种类型设备，比如多个串口、网口等，这些驱动比较类似，仅仅是一些寄存器基地址不一样，所以我们没有必须要为每一个设备单独写一个驱动，这些设备的驱动完全可以共用同一个驱动，我们只需要在驱动中区分出设备的次设备号，然后根据次设备号的访问不同的内存地址空间即可。

  根据上一届内容，驱动的read、write可以通过以下方式获得设备号:

~~~ c
file->f_dentry->d_inode->i_rdev
~~~

这样我们就可以通过宏MINOR来提取此设备号。

**实现代码如下：**

~~~ c
ssize_t dev_fifo_read (struct file *file, char __user *buf, size_t size,
loff_t *pos)
{
  int minor = MINOR(file->f_dentry->d_inode->i_rdev);
  struct mydev *cd;
  
  printk("read() MINOR(file->f_dentry->d_inode->i_rdev)=%d\n",minor);

  cd = (struct mydev *)file->private_data;
  printk("read()    file->private_data    cd->test=%d\n",cd->test);

  if(copy_to_user(buf, &minor, size)){
    return -EFAULT;
  }
  return size;
}
~~~

当驱动可以提取次设备号之后，我们就可以实现一份驱动支持多个同种类型的设备。

# **如何获得注册的设备结构体私有地址**?

  在大多情况下，我们会创建一个自定义的设备信息维护结构体，同时创建一个指针数组用来管理不同的设备。

~~~ c
#define MAX_COM_NUM 2

struct mydev{
  struct cdev cdev;
  char *reg;
  int test;
};
struct mydev *pmydev[MAX_COM_NUM];
~~~

然后通过成员cdev注册字符设备，

~~~ c
  for(i=0;i<MAX_COM_NUM;i++)
  {
    pmydev[i]->test = i;
    cdev_init(&pmydev[i]->cdev,&dev_fifo_ops);
    devno = MKDEV(major,i);  
    
    error = cdev_add(&pmydev[i]->cdev,devno,1);
    if(error < 0)
    {
      printk("cdev_add fail \n");
      goto ERR2;
    }
  }
~~~

想一个问题:如果我们为每一个同类型设备分配独立的设备结构体，分别注册对应的cdev，假如我打开/dev/com0 进行操作的时候，我怎么知道com0对应我们自己定义的设备管理结构体变量的地址呢?

有问题是好的，我们带着问题出发，看看大牛们是怎么做的。

~~~ c
//打开设备
static int dev_fifo_open (struct inode *inode, struct file *file)
{
  struct mydev *cd;
  
  cd = container_of(inode->i_cdev, struct mydev, cdev);
  file->private_data = cd;
  return 0;
}
~~~

该函数功能：

字符设备架构调用我们注册的接口函数open会传递参数inode和file，inode->i_cdev指向了我们注册的pmydev[i]->cdev，**在open中通过inode->cdev来识别具体的设备，通过container_of来找到对应的pmycdev结构体变量，并将其私有数据隐藏到file结构的private_data中，进而识别同一个驱动操作一类设备**。

 而read，write接口函数可以直接通过file的 private_data获取对应的pmycdev结构体变量。

~~~ bash
cd = (struct mydev *)file->private_data;
~~~

**【补充1】**

再来看下contianer_of 接口功能参数如下：

![](../../../image\微信图片_20210607145257.png)

该宏是如何实现的，留给读者自己思考。

**【补充2】**

**我们也可以在回调cdev.fops->open()阶段重新填充file结构的fop，进而实现同一个驱动操作不同的设备，这种思想就是内核驱动中常用的分层！**

执行结果如下:

![微信图片_20210607145337](../../../image\微信图片_20210607145337.jpg)

由结果可知,应用程序正确读取了minor的值。

![](../../../image\微信图片_20210607145409.png)

   从内核log来看，MINOR(file->f_dentry->d_inode->i_rdev)可以成功读取此设备号。而read接口函数也成功通过file->private_data得到了设备结构体变量（初始化的时候为不同设备的test成员附了不同的值）。

 **驱动程序：**

~~~ c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
static int major = 250;
static int minor = 0;
static dev_t devno;

#define MAX_COM_NUM 2

struct mydev{
  struct cdev cdev;
  char *reg;
  int test;
};
struct mydev *pmydev[MAX_COM_NUM];

ssize_t dev_fifo_read (struct file *file, char __user *buf, size_t size, loff_t *pos)
{
  int minor = MINOR(file->f_dentry->d_inode->i_rdev);
  struct mydev *cd;
  
  printk("read() MINOR(file->f_dentry->d_inode->i_rdev)=%d\n",minor);

  cd = (struct mydev *)file->private_data;
  printk("read()       file->private_data         cd->test=%d\n",cd->test);

  if(copy_to_user(buf, &minor, size)){
    return -EFAULT;
  }

  return size;
}
int dev_fifo_close (struct inode *inode, struct file *file)
{
  printk("dev_fifo_close()\n");
  return 0;
}
//打开设备
static int dev_fifo_open (struct inode *inode, struct file *file)
{
  struct mydev *cd;
  
  cd = container_of(inode->i_cdev, struct mydev, cdev);
  file->private_data = cd;
  return 0;
}
static struct file_operations dev_fifo_ops = 
{
  .open = dev_fifo_open,
  .read = dev_fifo_read,
  .release = dev_fifo_close,
};
static int dev_fifo_init(void)
{
  int result;
  int error;
  int i = 0;
  
  printk("dev_fifo_init \n");
  devno = MKDEV(major,minor);  
  result = register_chrdev_region(devno, MAX_COM_NUM, "test");
  if(result<0)
  {
    printk("register_chrdev_region fail \n");
    goto ERR1;
  }
  
  
  for(i=0;i<MAX_COM_NUM;i++)
  {
    pmydev[i] =kmalloc(sizeof(struct mydev), GFP_KERNEL);
  }
  
  for(i=0;i<MAX_COM_NUM;i++)
  {
    pmydev[i]->test = i; 
    cdev_init(&pmydev[i]->cdev,&dev_fifo_ops);
    devno = MKDEV(major,i);    
    error = cdev_add(&pmydev[i]->cdev,devno,1);
    if(error < 0)
    {
      printk("cdev_add fail \n");
      goto ERR2;
    }
  }
  return 0;
ERR2:
  devno = MKDEV(major,0);  
  unregister_chrdev_region(devno,MAX_COM_NUM);
  for(i=0;i<MAX_COM_NUM;i++)
  {
    kfree(pmydev[i]);
  }
  return error;
ERR1:
  return result;
}
static void dev_fifo_exit(void)
{
  int i;
  
  printk("dev_fifo_exit \n");
  
  for(i=0;i<MAX_COM_NUM;i++)
  {
    cdev_del(&pmydev[i]->cdev);
  }
  for(i=0;i<MAX_COM_NUM;i++)
  {
    kfree(pmydev[i]);
  }
  devno = MKDEV(major,0);  
  unregister_chrdev_region(devno,MAX_COM_NUM);
  return;
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("daniel.peng");
module_init(dev_fifo_init);
module_exit(dev_fifo_exit);
~~~

**测试程序**

~~~ c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
main()
{
  int fd0,fd1;
  int minor;
  
  fd0 = open("/dev/com0",O_RDWR);
  if(fd0<0)
  {
    perror("open fail \n");
    return;
  }
  printf("open /dev/com0 OK\n");

  read(fd0,&minor,sizeof(minor));
  printf("minor of /dev/com0 =%d\n",minor);
  close(fd0);
  
  fd1 = open("/dev/com1",O_RDWR);
  if(fd1<0)
  {
    perror("open fail \n");
    return;
  }
  printf("open /dev/com1 OK\n");

  read(fd1,&minor,sizeof(minor));
  printf("minor of /dev/com1 =%d\n",minor);
  close(fd1);
}
~~~

