 

用户和应用程序可以通过 proc 得到系统的信息，并可以改变内核的某些参数。由于系统的信息，如进程，是动态改变的，所以用户或应用程序读取 proc 文件时，proc 文件系统是动态从系统内核读出所需信息并提交的。

下面列出的这些文件或子文件夹，并不是都是在你的系统中存在，这取决于你的内核配置和装载的模块。另外，在 proc 下还有三个很重要的目录：net，scsi 和 sys。 sys 目录是可写的，可以通过它来访问或修改内核的参数，而 net 和 scsi 则依赖于内核配置。例如，如果系统不支持 scsi，则 scsi 目录不存在。

除了以上介绍的这些，还有的是一些以数字命名的目录，它们是进程目录。系统中当前运行的每一个进程都有对应的一个目录在 proc 下，以进程的 PID 号为目录名，它们是读取进程信息的接口。而 self 目录则是读取进程本身的信息接口，是一个 link。

**/proc/[pid]/auxv**

/proc/[pid]/auxv 包含传递给进程的 ELF 解释器信息，格式是每一项都是一个 unsigned long长度的 ID 加上一个 unsigned long 长度的值。最后一项以连续的两个 0x00 开头。举例如下：

```
$ hexdump -x /proc/2406/auxv

0000000    0021    0000    0000    0000    f000    f7fa    7fff    0000
0000010    0010    0000    0000    0000    fbff    1f8b    0000    0000
0000020    0006    0000    0000    0000    1000    0000    0000    0000
0000030    0011    0000    0000    0000    0064    0000    0000    0000
0000040    0003    0000    0000    0000    0040    0040    0000    0000
0000050    0004    0000    0000    0000    0038    0000    0000    0000
0000060    0005    0000    0000    0000    0007    0000    0000    0000
0000070    0007    0000    0000    0000    0000    0000    0000    0000
0000080    0008    0000    0000    0000    0000    0000    0000    0000
0000090    0009    0000    0000    0000    55e0    0045    0000    0000
00000a0    000b    0000    0000    0000    0000    0000    0000    0000
00000b0    000c    0000    0000    0000    0000    0000    0000    0000
00000c0    000d    0000    0000    0000    0000    0000    0000    0000
00000d0    000e    0000    0000    0000    0000    0000    0000    0000
00000e0    0017    0000    0000    0000    0000    0000    0000    0000
00000f0    0019    0000    0000    0000    f079    f7f6    7fff    0000
0000100    001f    0000    0000    0000    ffea    f7f6    7fff    0000
0000110    000f    0000    0000    0000    f089    f7f6    7fff    0000
0000120    0000    0000    0000    0000    0000    0000    0000    0000
0000130
```

**/proc/[pid]/cmdline**

/proc/[pid]/cmdline 是一个只读文件，包含进程的完整[命令](https://www.linuxcool.com/)行信息。如果该进程已经被交换出内存或者这个进程是 zombie 进程，则这个文件没有任何内容。该文件以空字符 null 而不是换行符作为结束标志。举例如下：

```
$ ps aux|grep frps
root      2406  0.1  0.1  54880 10524 ?        Sl   Dec11  21:30 frps -c ./frps.ini

$ cat /proc/2406/cmdline
frps-c./frps.ini
```

**/proc/[pid]/comm**

/proc/[pid]/comm 包含进程的[命令](https://www.linuxcool.com/)名。举例如下：

```
$ cat /proc/2406/comm
frps
```

**/proc/[pid]/cwd**

/proc/[pid]/cwd 是进程当前工作目录的符号链接。举例如下：

```
$ ls -lt /proc/2406/cwd
lrwxrwxrwx 1 root root 0 Dec 12 20:39 /proc/2406/cwd -> /home/mike/frp_0.13.0_linux_amd64
```

**/proc/[pid]/environ**

/proc/[pid]/environ 显示进程的环境变量。举例如下：

```
$ strings /proc/2406/environ

SUPERVISOR_GROUP_NAME=ssh
TERM=linux
SUPERVISOR_SERVER_URL=unix:///var/run/supervisor.sock
SUPERVISOR_PROCESS_NAME=ssh
RUNLEVEL=2
UPSTART_EVENTS=runlevel
PREVLEVEL=N
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin
UPSTART_INSTANCE=
UPSTART_JOB=rc
SUPERVISOR_ENABLED=1
runlevel=2
PWD=/
previous=N
```

**/proc/[pid]/exe**

/proc/[pid]/exe 为实际运行程序的符号链接。举例如下：

```
$ ls -lt /proc/2406/exe
lrwxrwxrwx 1 root root 0 Dec 11 19:00 /proc/2406/exe -> /usr/bin/frps
```

**/proc/[pid]/fd**

/proc/[pid]/fd 是一个目录，包含进程打开文件的情况。举例如下：

```
$ ls -lt /proc/2406/fd

lrwx------ 1 root root 64 Dec 24 09:39 77 -> socket:[44377722]
lrwx------ 1 root root 64 Dec 17 15:07 47 -> socket:[29482617]
lr-x------ 1 root root 64 Dec 12 20:18 0 -> pipe:[13282]
l-wx------ 1 root root 64 Dec 12 20:18 1 -> pipe:[13283]
lrwx------ 1 root root 64 Dec 12 20:18 10 -> socket:[12238218]
lrwx------ 1 root root 64 Dec 12 20:18 4 -> anon_inode:[eventpoll]
lrwx------ 1 root root 64 Dec 12 20:18 40 -> socket:[19378614]
```

目录中的每一项都是一个符号链接，指向打开的文件，数字则代表文件描述符。

**/proc/[pid]/latency**

/proc/[pid]/latency 显示哪些代码造成的延时比较大。如果要使用这个特性需要执行：

```
$ echo 1 > /proc/sys/kernel/latencytop
```

举例如下

```
$ cat /proc/2406/latency

Latency Top version : v0.1
30667 10650491 4891 poll_schedule_timeout do_sys_poll SyS_poll system_call_fastpath 0x7f636573dc1d
8 105 44 futex_wait_queue_me futex_wait do_futex SyS_futex system_call_fastpath 0x7f6365a167bc
```

每一行前三个数字分别是后面代码执行的次数、总共执行延迟时间(单位是微秒)和最长执行延迟时间(单位是微秒)。后面则是代码完整的调用栈。

**/proc/[pid]/maps**

/proc/[pid]/maps 显示进程的内存区域映射信息。举例如下：

```
$ cat /proc/2406/maps
00400000-006ea000 r-xp 00000000 fd:01 1727569                            /usr/bin/frps
006ea000-00a6c000 r--p 002ea000 fd:01 1727569                            /usr/bin/frps
00a6c000-00ab1000 rw-p 0066c000 fd:01 1727569                            /usr/bin/frps
00ab1000-00ad4000 rw-p 00000000 00:00 0
c000000000-c00000b000 rw-p 00000000 00:00 0
c41feac000-c420000000 rw-p 00000000 00:00 0
c420000000-c420400000 rw-p 00000000 00:00 0
c420400000-c420700000 rw-p 00000000 00:00 0
c420700000-c420800000 rw-p 00000000 00:00 0
c420800000-c420900000 rw-p 00000000 00:00 0
c420900000-c420a00000 rw-p 00000000 00:00 0
c420a00000-c421ea0000 rw-p 00000000 00:00 0
c421ea0000-c422a00000 rw-p 00000000 00:00 0
c422a00000-c422a60000 rw-p 00000000 00:00 0
7f0418c01000-7f0418ee1000 rw-p 00000000 00:00 0
7ffff7f4f000-7ffff7f70000 rw-p 00000000 00:00 0                          [stack:5121]
7ffff7fad000-7ffff7faf000 r--p 00000000 00:00 0                          [vvar]
7ffff7faf000-7ffff7fb1000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
```

其中注意的一点是 [stack:] 是线程的堆栈信息，对应于 /proc/[pid]/task/[tid]/ 路径。

**/proc/[pid]/root**

/proc/[pid]/root 是进程根目录的符号链接。举例如下：

```
$  ls -lt /proc/2406/root
lrwxrwxrwx 1 root root 0 Dec 12 20:39 /proc/2406/root -> /
```

**/proc/[pid]/stack**

/proc/[pid]/stack 示当前进程的内核调用栈信息，只有内核编译时打开了 CONFIG_STACKTRACE 编译选项，才会生成这个文件。举例如下：

```
$ cat /proc/2406/stack
[] futex_wait_queue_me+0xc6/0x130
[] futex_wait+0x17d/0x270
[] do_futex+0xd5/0x520
[] SyS_futex+0x71/0x150
[] entry_SYSCALL_64_fastpath+0x16/0x75
[] 0xffffffffffffffff
```

**/proc/[pid]/statm**

/proc/[pid]/statm 显示进程所占用内存大小的统计信息。包含七个值，度量单位是 page(page大小可通过 getconf PAGESIZE 得到)。举例如下：

```
$ cat /proc/2406/statm
13720 2617 493 746 0 12007 0
```

各个值含义：

```
a）进程占用的总的内存
b）进程当前时刻占用的物理内存
c）同其它进程共享的内存
d）进程的代码段
e）共享库(从2.6版本起，这个值为0)
f）进程的堆栈
g）dirty pages(从2.6版本起，这个值为0)
```

**/proc/[pid]/status**

/proc/[pid]/status 包含进程的状态信息。其很多内容与 /proc/[pid]/stat 和 /proc/[pid]/statm 相同，但是却是以一种更清晰地方式展现出来。举例如下：

```
$ cat /proc/2406/status
Name:   frps
State:  S (sleeping)
Tgid:   2406
Ngid:   0
Pid:    2406
PPid:   2130
TracerPid:  0
Uid:    0   0   0   0
Gid:    0   0   0   0
FDSize: 128
Groups: 0
NStgid: 2406
NSpid:  2406
NSpgid: 2406
NSsid:  2130
VmPeak:    54880 kB
VmSize:    54880 kB
VmLck:         0 kB
VmPin:         0 kB
VmHWM:     34872 kB
VmRSS:     10468 kB
VmData:    47896 kB
VmStk:       132 kB
VmExe:      2984 kB
VmLib:         0 kB
VmPTE:        68 kB
VmPMD:        20 kB
VmSwap:        0 kB
HugetlbPages:          0 kB
Threads:    11
SigQ:   0/31834
SigPnd: 0000000000000000
ShdPnd: 0000000000000000
SigBlk: 0000000000000000
SigIgn: 0000000000000000
SigCgt: fffffffe7fc1feff
CapInh: 0000000000000000
CapPrm: 0000003fffffffff
CapEff: 0000003fffffffff
CapBnd: 0000003fffffffff
CapAmb: 0000000000000000
Seccomp:    0
Cpus_allowed:   f
Cpus_allowed_list:  0-3
Mems_allowed:   00000000,00000001
Mems_allowed_list:  0
voluntary_ctxt_switches:    2251028
nonvoluntary_ctxt_switches: 18031
```

关于信号（signal）的信息：SigQ 分为两部分（例如 0/31834），前面表示当前处在队列中的信号（0），后面则表示队列一共可以存储多少信号（31834）；SigPnd 表示当前线程 pending 的信号，而ShdPnd 则表示整个进程 pending 的信号；SigBlk、SigIgn 和 SigCgt 分别表示对信号的处理是阻塞，忽略，还是捕获。（关于Unix信号的相关知识，可以参考 Unix: Dealing with signals）。

**/proc/[pid]/syscall**

/proc/[pid]/syscall 显示当前进程正在执行的系统调用。举例如下：

```
$ cat /proc/2406/syscall
202 0xab3730 0x0 0x0 0x0 0x0 0x0 0x7ffff7f6ec68 0x455bb3
```

第一个值是系统调用号（202代表poll），后面跟着 6 个系统调用的参数值（位于寄存器中），最后两个值依次是堆栈指针和指令计数器的值。如果当前进程虽然阻塞，但阻塞函数并不是系统调用，则系统调用号的值为 -1，后面只有堆栈指针和指令计数器的值。如果进程没有阻塞，则这个文件只有一个 running 的字符串。

内核编译时打开了 CONFIG_HAVE_ARCH_TRACEHOOK 编译选项，才会生成这个文件。

**/proc/[pid]/wchan**

/proc/[pid]/wchan 显示当进程 sleep 时，kernel 当前运行的函数。举例如下：

```
$ cat /proc/2406/wchan
futex_wait_queue_meadmin
```


一. proc文件系统介绍

Linux系统上的/proc目录是一种文件系统，即proc文件系统。

/与其它常见的文件系统不同的是，/proc是一种伪文件系统（也即虚拟文件系统），它不包含任何磁盘上的文件，而是由内核在运行时动态生成的文件和目录。

在Linux中存在着一类特殊的伪文件系统，用于使用与文件接口统一的操作来完成各种功能，例如ptyfs、devfs、sysfs和procfs。而procfs就是其中应用最广泛的一种伪文件系统。

procfs是Linux内核信息的抽象文件接口，大量内核中的信息以及可调参数都被作为常规文件映射到一个目录树中，这样我们就可以简单直接的通过echo或cat这样的文件操作命令对系统信息进行查取和调整了。同时procfs也提供了一个接口，使得我们自己的内核模块或用户态程序可以通过procfs进行参数的传递。在当今的Linux系统中，大量的系统工具也通过procfs获取内核参数，例如ps、lspci等等，没有procfs它们将可能不能正常工作。

而在 /proc 目录中，每个文件和目录都代表着一个系统内核的运行时状态。

这些文件和目录可以用来查询和修改系统的各种参数和配置信息，如进程信息、内存使用情况、系统负载、网络状态等等。通过 /proc 文件系统，用户和程序可以直接访问和操作内核的运行时数据，而无需编写专门的内核模块或系统调用。

/proc 文件系统是一个只读文件系统，其中的大多数文件都是只读的，因为它们反映了内核的运行时状态。但是，有些文件可以被修改以改变内核的配置。例如，可以通过修改 /proc/sys/kernel/hostname 文件来更改系统的主机名。

总的来说，/proc 文件系统为系统管理员和开发人员提供了一种方便的方式来查询和调试系统的状态和性能。

proc文件系统有哪些特点呢？

    可读性：proc文件系统的信息是以文本格式呈现的，这使得它易于读取和解析。

    系统级别：proc文件系统提供了对内核和系统级别信息的访问，例如系统中正在运行的进程、内存使用情况、CPU信息、硬件配置等等。

    可以用于诊断和调试：proc文件系统提供了对系统状态和进程信息的实时访问，这使得它非常有用于诊断和调试系统问题。

proc文件系统提供了一种非常方便的方式来访问和监控系统的内部状态，它是Linux操作系统中非常重要的组成部分

另外：proc 放置的数据都是在内存当中，例如系统内核、进程、外部设备的状态及网络状态等。因为这个目录下的数据都是在内存当中，所以本身不占任何硬盘空间。

在这里插入图片描述

个人主页：我是沐风晓月

本文首发于csdn， 直接在csdn搜【我是沐风晓月】
二. 进程目录中的常见文件介绍

/proc目录中包含许多以数字命名的子目录，这些数字表示系统当前正在运行进程的进程号，里面包含对应进程相关的多个信息文件。

[root@mufeng ~]# ll /proc |head
总用量 0
dr-xr-xr-x.  9 root           root                         0 12月  8 19:36 1
dr-xr-xr-x.  9 root           root                         0 3月   6 22:26 10
dr-xr-xr-x.  9 root           root                         0 3月   6 22:26 11
dr-xr-xr-x.  9 root           root                         0 3月   6 22:26 12
dr-xr-xr-x.  9 root           root                         0 3月   6 22:26 13
dr-xr-xr-x.  9 root           root                         0 3月   6 22:26 14
dr-xr-xr-x.  9 root           root                         0 3月   6 22:26 16
dr-xr-xr-x.  9 root           root                         0 12月  8 19:39 16773
dr-xr-xr-x.  9 root           root                         0 12月  8 19:39 16781
[root@mufeng ~]# 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13

我们来查看运行的一个PID为11713的进程：

[root@mufeng proc]# ll /proc/17713
总用量 0
dr-xr-xr-x.  2 root root 0 3月   6 23:22 attr
-rw-r--r--.  1 root root 0 3月   6 23:22 autogroup
-r--------.  1 root root 0 3月   6 23:22 auxv
-r--r--r--.  1 root root 0 3月   6 23:22 cgroup
--w-------.  1 root root 0 3月   6 23:22 clear_refs
-r--r--r--.  1 root root 0 3月   6 22:27 cmdline
-rw-r--r--.  1 root root 0 3月   6 23:22 comm
-rw-r--r--.  1 root root 0 3月   6 23:22 coredump_filter
-r--r--r--.  1 root root 0 3月   6 23:22 cpuset
lrwxrwxrwx.  1 root root 0 3月   6 23:22 cwd -> /root
-r--------.  1 root root 0 3月   6 23:22 environ
lrwxrwxrwx.  1 root root 0 3月   6 23:07 exe -> /usr/libexec/evolution-calendar-factory-subprocess
dr-x------.  2 root root 0 3月   6 23:22 fd
dr-x------.  2 root root 0 3月   6 23:22 fdinfo
-rw-r--r--.  1 root root 0 3月   6 23:22 gid_map
-r--------.  1 root root 0 3月   6 23:22 io
-r--r--r--.  1 root root 0 3月   6 23:22 limits
-rw-r--r--.  1 root root 0 3月   6 23:22 loginuid
dr-x------.  2 root root 0 3月   6 23:22 map_files
-r--r--r--.  1 root root 0 3月   6 23:22 maps
-rw-------.  1 root root 0 3月   6 23:22 mem
-r--r--r--.  1 root root 0 3月   6 23:22 mountinfo
-r--r--r--.  1 root root 0 3月   6 23:22 mounts
-r--------.  1 root root 0 3月   6 23:22 mountstats
dr-xr-xr-x.  6 root root 0 3月   6 23:22 net
dr-x--x--x.  2 root root 0 3月   6 23:22 ns
-r--r--r--.  1 root root 0 3月   6 23:22 numa_maps
-rw-r--r--.  1 root root 0 3月   6 23:22 oom_adj
-r--r--r--.  1 root root 0 3月   6 23:22 oom_score
-rw-r--r--.  1 root root 0 3月   6 23:22 oom_score_adj
-r--r--r--.  1 root root 0 3月   6 23:22 pagemap
-r--------.  1 root root 0 3月   6 23:22 patch_state
-r--r--r--.  1 root root 0 3月   6 23:22 personality
-rw-r--r--.  1 root root 0 3月   6 23:22 projid_map
lrwxrwxrwx.  1 root root 0 3月   6 22:29 root -> /
-rw-r--r--.  1 root root 0 3月   6 23:22 sched
-r--r--r--.  1 root root 0 3月   6 23:22 schedstat
-r--r--r--.  1 root root 0 3月   6 23:22 sessionid
-rw-r--r--.  1 root root 0 3月   6 23:22 setgroups
-r--r--r--.  1 root root 0 3月   6 23:22 smaps
-r--r--r--.  1 root root 0 3月   6 23:22 stack
-r--r--r--.  1 root root 0 3月   6 23:07 stat
-r--r--r--.  1 root root 0 3月   6 23:22 statm
-r--r--r--.  1 root root 0 3月   6 22:27 status
-r--r--r--.  1 root root 0 3月   6 23:22 syscall
dr-xr-xr-x. 11 root root 0 3月   6 22:29 task
-r--r--r--.  1 root root 0 3月   6 23:22 timers
-rw-r--r--.  1 root root 0 3月   6 23:22 uid_map
-r--r--r--.  1 root root 0 3月   6 23:22 wchan
[root@mufeng proc]# 

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27
    28
    29
    30
    31
    32
    33
    34
    35
    36
    37
    38
    39
    40
    41
    42
    43
    44
    45
    46
    47
    48
    49
    50
    51
    52
    53

    cmdline 启动当前进程的完整命令，但僵尸进程目录中的此文件不包含任何信息；

[root@mufeng proc]# more 17942/cmdline
/usr/libexec/fwupd/fwupd

    1
    2

    cwd 指向当前进程运行目录的一个符号链接；

    environ 当前进程的环境变量列表，彼此间用空字符（NULL）隔开；变量用大写字母表示，其值用小写字母表示；

[root@mufeng proc]# more 17942/environ
LANG=zh_CN.UTF-8

    1
    2
    3

    exe — 指向启动当前进程的可执行文件（完整路径）的符号链接，通过/proc/N/exe可以启动当前进程的一个拷贝；

    fd — 这是个目录，包含当前进程打开的每一个文件的文件描述符（file descriptor），这些文件描述符是指向实际文件的一个符号链接；

ot@mufeng proc]# ll /proc/76501/fd
总用量 0
lrwx------. 1 root root 64 3月   6 23:29 0 -> /dev/pts/0
lrwx------. 1 root root 64 3月   6 23:29 1 -> /dev/pts/0
lrwx------. 1 root root 64 3月   6 23:28 2 -> /dev/pts/0
lrwx------. 1 root root 64 3月   6 23:29 3 -> /var/tmp/a.txt.swp

    1
    2
    3
    4
    5
    6
    7

    limits 当前进程所使用的每一个受限资源的软限制、硬限制和管理单元；此文件仅可由实际启动当前进程的UID用户读取

    maps — 当前进程关联到的每个可执行文件和库文件在内存中的映射区域及其访问权限所组成的列表；

[root@mufeng proc]# ll /proc/76501/maps
-r--r--r--. 1 root root 0 3月   6 23:30 /proc/76501/maps
[root@mufeng proc]# 

    1
    2
    3
    4

    mem — 当前进程所占用的内存空间，由open、read和lseek等系统调用使用，不能被用户读取；

    root — 指向当前进程运行根目录的符号链接；在Unix和Linux系统上，通常采用chroot命令使每个进程运行于独立的根目录；

    stat — 当前进程的状态信息，包含一系统格式化后的数据列，可读性差，通常由ps命令使用；

    statm — 当前进程占用内存的状态信息，通常以“页面”（page）表示；

    status — 与stat所提供信息类似，但可读性较好，如下所示，每行表示一个属性信息；

[root@mufeng proc]# more /proc/76501/status
Name:	vim
Umask:	0022
State:	S (sleeping)
Tgid:	76501
Ngid:	0
Pid:	76501
PPid:	75155
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
FDSize:	256
Groups:	0 
VmPeak:	  151768 kB
VmSize:	  151544 kB
VmLck:	       0 kB
VmPin:	       0 kB

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17

    task 目录文件，包含由当前进程所运行的每一个线程的相关信息，每个线程的相关信息文件均保存在一个由线程号（tid）命名的目录中，这类似于其内容类似于每个进程目录中的内容；

三. /proc目录下其他常见的文件介绍
3.1 常见的文件

    /proc/cpuinfo: 这个文件包含了有关 CPU 的详细信息，包括厂商、型号、频率、缓存等信息。

    /proc/meminfo: 这个文件包含了有关内存的详细信息，包括可用内存、缓存大小、交换分区等信息。

    /proc/loadavg: 这个文件包含了系统的负载平均值，它表示系统正在运行的进程数和等待 CPU 的进程数的平均值。

    /proc/version: 这个文件包含了系统的版本信息，包括内核版本、GCC 版本等信息。

    /proc/net/tcp: 这个文件包含了有关 TCP 连接的信息，包括本地地址、远程地址、状态等信息。

    /proc/net/udp: 这个文件包含了有关 UDP 连接的信息，包括本地地址、远程地址、状态等信息。

    /proc/sys/kernel/hostname: 这个文件包含了系统的主机名。

    /proc/sys/kernel/pid_max: 这个文件包含了系统允许的最大进程 ID

    /proc/sys/fs/file-nr: 这个文件包含了有关打开文件的统计信息，包括当前打开的文件数、已分配的文件句柄数和最大文件句柄

3.2 /proc/sys目录详解

/proc/sys 目录下包含了许多系统内核运行时的配置文件，可以通过修改这些文件来改变系统的行为。
文件	介绍
/proc/sys/kernel/hostname	用于设置或获取系统的主机名。
/proc/sys/kernel/pid_max	用于设置或获取系统支持的最大进程ID号。
/proc/sys/kernel/sem	用于设置或获取系统的信号量限制。
/proc/sys/kernel/shmmax	用于设置或获取系统共享内存段的最大大小。
/proc/sys/kernel/shmall	用于设置或获取系统共享内存的最大页数。
/proc/sys/net/ipv4/tcp_keepalive_time	用于设置或获取TCP Keepalive的时间间隔。
/proc/sys/net/ipv4/ip_forward	用于控制IP包的转发。
/proc/sys/net/ipv4/tcp_syncookies	用于控制系统是否启用TCP Syncookie保护。
/proc/sys/vm/swappiness	用于设置内核在不足内存时候的换页策略。
/proc/sys/fs/file-max	用于设置或获取系统可以同时打开的文件数的最大值

需要注意的是，这些文件都是在内核运行时动态生成的，实际上并不是磁盘上的文件。因此，修改这些文件并不会直接修改磁盘上的文件，而是会直接影响内核的运行。如果想要在系统启动时自动修改这些文件，可以在 /etc/sysctl.conf 中设置。
————————————————
版权声明：本文为CSDN博主「我是沐风晓月」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/wisdom_futrue/article/details/129372804