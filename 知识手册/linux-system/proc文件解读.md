 

# /proc 文件解读

[![img](https://cdn2.jianshu.io/assets/default_avatar/8-a356878e44b45ab268a3b0bbaaadeeb7.jpg)](https://www.jianshu.com/u/2e407ef7bdef)

[zpei0411](https://www.jianshu.com/u/2e407ef7bdef)关注

2018.08.03 22:19:58字数 3,066阅读 1,983

   /proc 目录是Linux系统上一种伪文件系统（也即虚拟文件系统），存储的是当前内核运行状态的一系列特殊文件，用户可以通过这些文件查看有关系统硬件及当前正在运行进程的信息，甚至可以通过更改其中某些文件来改变内核的运行状态。

   基于 /proc 文件系统如上所述的特殊性，其内的文件也常被称作虚拟文件，并具有一些独特的特点。例如，其中有些文件虽然使用查看命令查看时会返回大量信息，但文件本身的大小却会显示为0字节。此外，这些特殊文件中大多数文件的时间及日期属性通常为当前系统时间和日期，这跟它们随时会被刷新（存储于RAM中）有关。

   为了查看及使用上的方便，这些文件通常会按照相关性进行分类存储于不同的目录甚至子目录中，如 /proc/scsi 目录中存储的就是当前系统上所有 SCSI 设备的相关信息，/proc/N 中存储的则是系统当前正在运行的进程的相关信息，其中N为正在运行的进程（因此，在某进程结束后其相关目录则会消失）。

### /proc 目录介绍

   可以看到，在 /proc 目录下，有许多以数字命名的子目录，这些数字表示系统当前正在运行进程的进程号，里面包含对应进程相关的多个信息文件。



![img](https://upload-images.jianshu.io/upload_images/13236273-60cd2ebb3ee19e57.png?imageMogr2/auto-orient/strip|imageView2/2/w/724/format/webp)

image.png



   第一个进程是 init 进程，因此它的 process-id 为 1。然后对这个目录执行一个 ls 命令，这会显示很多文件，其中一些文件是每个进程都会有的



```bash
ubuntu@ip-172-31-27-158:/proc/1$ sudo ls -l /proc/1
total 0
dr-xr-xr-x 2 root root 0 Aug  3 08:16 attr
-rw-r--r-- 1 root root 0 Aug  3 08:16 autogroup
-r-------- 1 root root 0 Aug  3 08:16 auxv
-r--r--r-- 1 root root 0 Oct 27  2017 cgroup
--w------- 1 root root 0 Aug  3 08:16 clear_refs
-r--r--r-- 1 root root 0 Oct 27  2017 cmdline #启动当前进程的完整命令，但僵尸进程目录中的此文件不包含任何信息；可使用 more /proc/1/cmdline 查看
-rw-r--r-- 1 root root 0 Oct 27  2017 comm
-rw-r--r-- 1 root root 0 Aug  3 08:16 coredump_filter
-r--r--r-- 1 root root 0 Aug  3 08:16 cpuset
lrwxrwxrwx 1 root root 0 Jul 25 02:17 cwd -> / #指向当前进程运行目录的一个符号链接；
-r-------- 1 root root 0 Oct 27  2017 environ #当前进程的环境变量列表，彼此间用空字符（NULL）隔开；变量用大写字母表示，其值用小写字母表示；
lrwxrwxrwx 1 root root 0 Aug  3 08:16 exe -> /sbin/init #指向启动当前进程的可执行文件（完整路径）的符号链接，通过/proc/N/exe可以启动当前进程的一个拷贝；
dr-x------ 2 root root 0 Aug  3 08:16 fd #这是个目录，包含当前进程打开的每一个文件的文件描述符（file descriptor），这些文件描述符是指向实际文件的一个符号链接；
dr-x------ 2 root root 0 Jan 30  2018 fdinfo
-rw-r--r-- 1 root root 0 Aug  3 08:16 gid_map
-r-------- 1 root root 0 Aug  3 08:16 io
-r--r--r-- 1 root root 0 Aug  3 08:16 latency
-r--r--r-- 1 root root 0 Oct 27  2017 limits #当前进程所使用的每一个受限资源的软限制、硬限制和管理单元；此文件仅可由实际启动当前进程的UID用户读取
-rw-r--r-- 1 root root 0 Aug  3 08:16 loginuid
dr-x------ 2 root root 0 Aug  3 08:16 map_files
-r--r--r-- 1 root root 0 Jul 25 02:17 maps #当前进程关联到的每个可执行文件和库文件在内存中的映射区域及其访问权限所组成的列表；
-rw------- 1 root root 0 Aug  3 08:16 mem #当前进程所占用的内存空间，由open、read和lseek等系统调用使用，不能被用户读取；
-r--r--r-- 1 root root 0 Aug  3 08:16 mountinfo
-r--r--r-- 1 root root 0 Aug  3 08:16 mounts
-r-------- 1 root root 0 Aug  3 08:16 mountstats
dr-xr-xr-x 7 root root 0 Aug  3 08:16 net
dr-x--x--x 2 root root 0 Aug  3 08:16 ns
-r--r--r-- 1 root root 0 Aug  3 08:16 numa_maps
-rw-r--r-- 1 root root 0 Aug  3 08:16 oom_adj
-r--r--r-- 1 root root 0 Aug  3 08:16 oom_score
-rw-r--r-- 1 root root 0 Aug  3 08:16 oom_score_adj
-r--r--r-- 1 root root 0 Aug  3 08:16 pagemap
-r--r--r-- 1 root root 0 Aug  3 08:16 personality
-rw-r--r-- 1 root root 0 Aug  3 08:16 projid_map
lrwxrwxrwx 1 root root 0 Oct 27  2017 root -> / #指向当前进程运行根目录的符号链接；在Unix和Linux系统上，通常采用chroot命令使每个进程运行于独立的根目录； 
-rw-r--r-- 1 root root 0 Aug  3 08:16 sched
-r--r--r-- 1 root root 0 Aug  3 08:16 schedstat
-r--r--r-- 1 root root 0 Aug  3 08:16 sessionid
-rw-r--r-- 1 root root 0 Aug  3 08:16 setgroups
-r--r--r-- 1 root root 0 Aug  3 08:16 smaps
-r--r--r-- 1 root root 0 Aug  3 08:16 stack
-r--r--r-- 1 root root 0 Oct 27  2017 stat #当前进程的状态信息，包含一系统格式化后的数据列，可读性差，通常由ps命令使用；
-r--r--r-- 1 root root 0 May 23 09:35 statm #当前进程占用内存的状态信息，通常以“页面”（page）表示； 
-r--r--r-- 1 root root 0 Oct 27  2017 status #与stat所提供信息类似，但可读性较好，如下所示，每行表示一个属性信息；
-r--r--r-- 1 root root 0 Aug  3 08:16 syscall
dr-xr-xr-x 3 root root 0 Jul 12 03:32 task #目录文件，包含由当前进程所运行的每一个线程的相关信息，每个线程的相关信息文件均保存在一个由线程号（tid）命名的目录中，类似于每个进程目录中的内容
-r--r--r-- 1 root root 0 Aug  3 08:16 timers
-rw-r--r-- 1 root root 0 Aug  3 08:16 uid_map
-r--r--r-- 1 root root 0 Aug  3 08:16 wchan
```

### /proc 常见文件介绍

1. /proc/apm
   高级电源管理（APM）版本信息及电池相关状态信息，通常由apm命令使用；

2. /proc/buddyinfo
   用于诊断内存碎片问题的相关信息文件；

3. /proc/cmdline
   在启动时传递至内核的相关参数信息，这些信息通常由lilo或grub等启动管理工具进行传递；

   ![img](https://upload-images.jianshu.io/upload_images/13236273-54edf0e141812b68.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

   image.png

4. /proc/cpuinfo
   处理器的相关信息的文件

   ![img](https://upload-images.jianshu.io/upload_images/13236273-6a67eb35cf43da0e.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

   image.png

5. /proc/crypto
   系统上已安装的内核使用的密码算法及每个算法的详细信息列表；

   ![img](https://upload-images.jianshu.io/upload_images/13236273-e5774149b382f8b1.png?imageMogr2/auto-orient/strip|imageView2/2/w/1140/format/webp)

   image.png

6. /proc/devices
   系统已经加载的所有块设备和字符设备的信息，包含主设备号和设备组（与主设备号对应的设备类型）名；

   ![img](https://upload-images.jianshu.io/upload_images/13236273-02e35165c8f99924.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

   image.png

7. /proc/diskstats
   每块磁盘设备的磁盘I/O统计信息列表；

   ![img](https://upload-images.jianshu.io/upload_images/13236273-e752b2b20054ddbe.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

   image.png

8. /proc/dma
   每个正在使用且注册的ISA DMA通道的信息列表；

   ![img](https://upload-images.jianshu.io/upload_images/13236273-cc2f2ae86edec34a.png?imageMogr2/auto-orient/strip|imageView2/2/w/674/format/webp)

   image.png

9. /proc/execdomains
   内核当前支持的执行域（每种操作系统独特“个性”）信息列表；

   ![img](https://upload-images.jianshu.io/upload_images/13236273-6cff320e1b4b6691.png?imageMogr2/auto-orient/strip|imageView2/2/w/748/format/webp)

   image.png

10. /proc/fb
    帧缓冲设备列表文件，包含帧缓冲设备的设备号和相关驱动信息；

11. /proc/filesystems
    当前被内核支持的文件系统类型列表文件，被标示为nodev的文件系统表示不需要块设备的支持；通常mount一个设备时，如果没有指定文件系统类型将通过此文件来决定其所需文件系统的类型；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-53037d94ff6ed180.png?imageMogr2/auto-orient/strip|imageView2/2/w/848/format/webp)

    image.png

12. /proc/interrupts
    X86或X86_64体系架构系统上每个IRQ相关的中断号列表；多路处理器平台上每个CPU对于每个I/O设备均有自己的中断号；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-c7e3585e584acfbb.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

    image.png

13. /proc/iomem
    每个物理设备上的记忆体（RAM或者ROM）在系统内存中的映射信息；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-197250549684bbff.png?imageMogr2/auto-orient/strip|imageView2/2/w/786/format/webp)

    image.png

14. /proc/ioports
    当前正在使用且已经注册过的与物理设备进行通讯的输入-输出端口范围信息列表；如下面所示，第一列表示注册的I/O端口范围，其后表示相关的设备；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-232f86192f3f3a93.png?imageMogr2/auto-orient/strip|imageView2/2/w/532/format/webp)

    image.png

15. /proc/kallsyms
    模块管理工具用来动态链接或绑定可装载模块的符号定义，由内核输出；（内核2.5.71以后的版本支持此功能）；通常这个文件中的信息量相当大；

16. /proc/kcore
    系统使用的物理内存，以ELF核心文件（core file）格式存储，其文件大小为已使用的物理内存（RAM）加上4KB；这个文件用来检查内核数据结构的当前状态，因此，通常由GBD通常调试工具使用，但不能使用文件查看命令打开此文件；

17. /proc/kmsg
    此文件用来保存由内核输出的信息，通常由/sbin/klogd或/bin/dmsg等程序使用，不要试图使用查看命令打开此文件；

18. /proc/loadavg
    保存关于CPU和磁盘I/O的负载平均值，其前三列分别表示每1秒钟、每5秒钟及每15秒的负载平均值，类似于uptime命令输出的相关信息；第四列是由斜线隔开的两个数值，前者表示当前正由内核调度的实体（进程和线程）的数目，后者表示系统当前存活的内核调度实体的数目；第五列表示此文件被查看前最近一个由内核创建的进程的PID；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-b4a9e5979fb2c9c6.png?imageMogr2/auto-orient/strip|imageView2/2/w/1072/format/webp)

    image.png

19. /proc/locks
    保存当前由内核锁定的文件的相关信息，包含内核内部的调试数据；每个锁定占据一行，且具有一个惟一的编号；如下输出信息中每行的第二列表示当前锁定使用的锁定类别，POSIX表示目前较新类型的文件锁，由lockf系统调用产生，FLOCK是传统的UNIX文件锁，由flock系统调用产生；第三列也通常由两种类型，ADVISORY表示不允许其他用户锁定此文件，但允许读取，MANDATORY表示此文件锁定期间不允许其他用户任何形式的访问；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-af632ea1ca7b0788.png?imageMogr2/auto-orient/strip|imageView2/2/w/788/format/webp)

    image.png

20. /proc/mdstat
    保存RAID相关的多块磁盘的当前状态信息，在没有使用RAID机器上，其显示为如下状态：

    ![img](https://upload-images.jianshu.io/upload_images/13236273-17a3a5b838b2edf9.png?imageMogr2/auto-orient/strip|imageView2/2/w/376/format/webp)

    image.png

21. /proc/meminfo
    系统中关于当前内存的利用状况等的信息，常由free命令使用；可以使用文件查看命令直接读取此文件，其内容显示为两列，前者为统计属性，后者为对应的值；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-a2ef3dca627e79cf.png?imageMogr2/auto-orient/strip|imageView2/2/w/534/format/webp)

    image.png

22. /proc/mounts
    在内核2.4.29版本以前，此文件的内容为系统当前挂载的所有文件系统，在2.4.19以后的内核中引进了每个进程使用独立挂载名称空间的方式，此文件则随之变成了指向/proc/self/mounts（每个进程自身挂载名称空间中的所有挂载点列表）文件的符号链接；/proc/self是一个独特的目录，后文中会对此目录进行介绍；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-e6e25fd0f35e0766.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

    image.png

    如下所示，其中第一列表示挂载的设备，第二列表示在当前目录树中的挂载点，第三点表示当前文件系统的类型，第四列表示挂载属性（ro或者rw），第五列和第六列用来匹配/etc/mtab文件中的转储（dump）属性；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-5a6195048aef7085.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

    image.png

23. /proc/modules
    当前装入内核的所有模块名称列表，可以由lsmod命令使用，也可以直接查看；如下所示，其中第一列表示模块名，第二列表示此模块占用内存空间大小，第三列表示此模块有多少实例被装入，第四列表示此模块依赖于其它哪些模块，第五列表示此模块的装载状态（Live：已经装入；Loading：正在装入；Unloading：正在卸载），第六列表示此模块在内核内存（kernel memory）中的偏移量；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-a78c448a78373586.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

    image.png

24. /proc/partitions
    块设备每个分区的主设备号（major）和次设备号（minor）等信息，同时包括每个分区所包含的块（block）数目（如下面输出中第三列所示）；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-c13b39737e3be1d0.png?imageMogr2/auto-orient/strip|imageView2/2/w/732/format/webp)

    image.png

25. /proc/pci
    内核初始化时发现的所有PCI设备及其配置信息列表，其配置信息多为某PCI设备相关IRQ信息，可读性不高，可以用“/sbin/lspci –vb”命令获得较易理解的相关信息；在2.6内核以后，此文件已为/proc/bus/pci目录及其下的文件代替；

26. /proc/slabinfo
    在内核中频繁使用的对象（如inode、dentry等）都有自己的cache，即slab pool，而/proc/slabinfo文件列出了这些对象相关slap的信息；详情可以参见内核文档中slapinfo的手册页；

27. /proc/stat
    实时追踪自系统上次启动以来的多种统计信息；如下所示，其中，
    “cpu”行后的八个值分别表示以1/100（jiffies）秒为单位的统计值（包括系统运行于用户模式、低优先级用户模式，运系统模式、空闲模式、I/O等待模式的时间等）；
    “intr”行给出中断的信息，第一个为自系统启动以来，发生的所有的中断的次数；然后每个数对应一个特定的中断自系统启动以来所发生的次数；
    “ctxt”给出了自系统启动以来CPU发生的上下文交换的次数。
    “btime”给出了从系统启动到现在为止的时间，单位为秒；
    “processes (total_forks) 自系统启动以来所创建的任务的个数目；
    “procs_running”：当前运行队列的任务的数目；
    “procs_blocked”：当前被阻塞的任务的数目；

    ![img](https://upload-images.jianshu.io/upload_images/13236273-eac5e7ba479be283.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

    image.png

28. /proc/swaps
    当前系统上的交换分区及其空间利用信息，如果有多个交换分区的话，则会每个交换分区的信息分别存储于/proc/swap目录中的单独文件中，而其优先级数字越低，被使用到的可能性越大；下面是作者系统中只有一个交换分区时的输出信息；



```ruby
ubuntu@ip-172-31-27-158:~$ # more /proc/swaps 
Filename                                Type            Size    Used    Priority 
/dev/sda8                               partition       642560  0       -1 
```

1. /proc/uptime
   系统上次启动以来的运行时间，如下所示，其第一个数字表示系统运行时间，第二个数字表示系统空闲时间，单位是秒；

   ![img](https://upload-images.jianshu.io/upload_images/13236273-d7bcb8f1e6c2162e.png?imageMogr2/auto-orient/strip|imageView2/2/w/658/format/webp)

   image.png

2. /proc/version
   当前系统运行的内核版本号,有的系统会显示系统安装的gcc版本

   ![img](https://upload-images.jianshu.io/upload_images/13236273-7e87954b6e65c60f.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

   image.png

3. /proc/vmstat
   当前系统虚拟内存的多种统计数据，信息量可能会比较大，这因系统而有所不同，可读性较好；下面为输出信息的一个片段；（2.6以后的内核支持此文件）

   ![img](https://upload-images.jianshu.io/upload_images/13236273-3ced49d53ea3d24d.png?imageMogr2/auto-orient/strip|imageView2/2/w/790/format/webp)

   image.png

4. /proc/zoneinfo
   内存区域（zone）的详细信息列表，信息量较大，下面列出的是一个输出片段：

   ![img](https://upload-images.jianshu.io/upload_images/13236273-5ce67698663aa761.png?imageMogr2/auto-orient/strip|imageView2/2/w/796/format/webp)

   image.png

### /proc/sys目录详解

   与/proc/目录中其它目录不相同的是，/proc/sys/目录下的文件不仅能提供系统的有关信息，而且还允许用户立即停止或开启内核的某些特性及功能。在/proc/sys/目录中的/proc/sys/net/子目录更是与网络息息相关，我们可以通过设置此目录下的某些文件来开启与网络应用相关的特殊功能，同时，也可以通过设置这个目录下的某些文件来保护我们的网络安全。
/proc/sys/net/ 目录的具体说明和设置见 http://blog.51cto.com/liuyuanljy/162643

参考： https://www.cnblogs.com/vigarbuaa/archive/2013/06/04/3118070.html
http://blog.51cto.com/liuyuanljy/162643
