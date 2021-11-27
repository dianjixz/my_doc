 

1.查看总内存：
~~~
#  grep MemTotal /proc/meminfo
~~~
2.查看可用内存及已用内存（free）
~~~
# free -m      (默认是以kb为单位，-m表示以Mb为单位)
~~~
3.top命令，进程监控
~~~
# top   ( 进入监控界面后按M后可以看到以内存占用大小排序的视图 )
~~~
4. ps 命令，监控指定的进程
~~~
# ps -e -o 'pid,comm,args,pcpu,rsz,vsz,stime,user,uid' | grep root | sort -nrk5
~~~
5.内存释放
~~~
# sync   (将所有未写的系统 缓冲区写到磁盘中，否则在释放缓存的过程中，可能会丢失未保存的文件)
# cat   /proc/sys/vm/drop_caches   （查询当前缓存释放的参数：0-不释放，1-释放页缓存，2-释放dentries和inodes，3-释放所有缓存）
# echo 3 > /proc/sys/vm/drop_caches   （将参数写入drop_caches文件，此命令即时生效，内存立即释放）
~~~
————————————————
版权声明：本文为CSDN博主「易勇2017」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/Q1648778537/article/details/78680107

# linux+内存碎片查看,关于linux 内存碎片指数

[linux](https://so.csdn.net/so/search?from=pc_blog_highlight&q=linux)针对每一个node的每个zone的每个order，都有一个碎片指数来描述当前的碎片程度，也就是

extfrag_index 参数：

extfrag_index这个要展示出来，需要内核编译了两个选项，即：

\#if defined(CONFIG_DEBUG_FS) && defined(CONFIG_COMPACTION)

否则不会生成这个文件。

[root@localhost ~]# grep CONFIG_DEBUG_FS /boot/config-3.10.0-693.5.2.el7.x86_64

CONFIG_DEBUG_FS=y

[root@localhost ~]# grep CONFIG_COMPACTION /boot/config-3.10.0-693.5.2.el7.x86_64

CONFIG_COMPACTION=y

如果确定已经编译进入内核，但是也看不到/sys/kernel/debug/下的数据，那说明没有挂载，或者挂载的路径不是/sys/kernel/debug/，如果没有挂载则需要挂载一下：

mount -t debugfs none /sys/kernel/debug

然后，在linux里面展示如下：

[root@localhost ~]# cat /sys/kernel/debug/extfrag/extfrag_index

Node , zone DMA -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000

Node , zone DMA32 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000

Node , zone Normal -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 0.995 0.998

Node , zone Normal -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 0.988 0.994 0.997

那这些数据怎么理解呢？

我们先来看一下打印这些数据的函数：

static void extfrag_show_print(struct seq_file *m,

pg_data_t *pgdat, struct zone *zone)

{undefined

unsigned int order;

int index;

/* Alloc on stack as interrupts are disabled for zone walk */

struct contig_page_info info;

seq_printf(m, "Node %d, zone %8s ",

pgdat->node_id,

zone->name);

for (order = ; order < MAX_ORDER; ++order) {undefined

fill_contig_page_info(zone, order, &info);

index = __fragmentation_index(order, &info);

seq_printf(m, "%d.%03d ", index / , index % );------------可以看出，"."前面是__fragmentation_index返回值的除数，后面是余数

}

seq_putc(m, '\n');

}

如下就是计算碎片指数的函数：

static int __fragmentation_index(unsigned int order, struct contig_page_info *info)

{undefined

unsigned long requested = 1UL << order;

if (!info->free_blocks_total)------------没有内存，返回0，都没有内存，谈何碎片

return 0;

/* Fragmentation index only makes sense when a request would fail */

if (info->free_blocks_suitable)------------返回-1000，也就是展示的是-1.000，那么这个时候没有意义，因为内存充足，不关心碎片指数，碎片指数只在申请内存失败的时候有意义

return -1000;

/*

\* Index is between 0 and 1 so return within 3 decimal places

*

\* 0 => allocation would fail due to lack of memory

\* 1 => allocation would fail due to fragmentation

*/

return 1000 - div_u64( (1000+(div_u64(info->free_pages * 1000ULL, requested))), info->free_blocks_total);

}

可以看出，越靠近1000，则碎片越严重，很容易分配失败。-1000表示内存充足，不需要关心碎片指数。为0代表压根就没free内存了，也不需要关心碎片指数。这两个极端都不需要考虑。

我们来看具体的数据：

[root@localhost ~]# cat /proc/buddyinfo

Node , zone DMA

Node , zone DMA32

Node , zone Normal

Node , zone Normal

[root@localhost ~]# cat /sys/kernel/debug/extfrag/extfrag_index

Node , zone DMA -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000

Node , zone DMA32 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000

Node , zone Normal -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 0.995 0.998

Node , zone Normal -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 0.988 0.994 0.997

然后迁移合并一下内存:

[root@localhost ~]# echo >/proc/sys/vm/compact_memory

[root@localhost ~]# cat /proc/buddyinfo

Node , zone DMA

Node , zone DMA32

Node , zone Normal

Node , zone Normal

[root@localhost ~]# cat /sys/kernel/debug/extfrag/extfrag_index

Node , zone DMA -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000

Node , zone DMA32 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000

Node , zone Normal -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000

Node , zone Normal -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 -1.000 0.993

总的来看，当有最后一个大页的内存的时候，即4k*2的10次方，也就是4M的连续页面的时候，问题都不大，因为可以拆分页面。

所以如果需要脚本判断当前系统的碎片程度，可以看最后4列的值，如果都为-1.000，没问题，否则多少存在碎片，如果值越大，则越碎。

可以通过配置cat /proc/sys/vm/extfrag_threshold来缓解碎片的问题。这个值默认是500.

如果超过了extfrag_threshold这个值, kswapd就会触发memory compaction . 所以, 这个值设置接近1000, 说明系统在内存碎片的处理倾向于把旧的页换出, 以符合申请的需要; 而设置接近0, 表示系统在内存碎片的处理倾向于做memory compaction，迁移合并及时的话，就可以缓解碎片。但迁移合并是耗性能的，所以可以将其配置为200左右。

linux内存碎片防治技术

Linux kernel组织管理物理内存的方式是buddy system(伙伴系统),而物理内存碎片正式buddy system的弱点之一,为了预防以及解决碎片问题,kernel采取了一些实用技术,这 ...

linux内存碎片的概念

一般在内核术语中的“碎片”都是基于物理内存而言的,我没有太看懂你得出碎片是针对地址空间这个结论的逻辑.但我认为,既然你知道了malloc是用户空间调用的,那么你所谓的碎片也是从用户空间的视角而言的,但 ...

Linux 驱动开发

linux驱动开发总结(一) 基础性总结 1, linux驱动一般分为3大类: * 字符设备 * 块设备 * 网络设备 2, 开发环境构建: * 交叉工具链构建 * NFS和tftp服务器安装 3, ...

linux kernel内存碎片防治技术

Linux kernel组织管理物理内存的方式是buddy system(伙伴系统),而物理内存碎片正式buddy system的弱点之一,为了预防以及解决碎片问题,kernel采取了一些实用技术,这 ...

linux socket高性能服务器处理框架

这个博客很多东西 http://blog.csdn.net/luozhonghua2014/article/details/37041765  思考一种高性能的服务器处理框架 1.首先需要一个内存池 ...

嵌入式Linux C笔试题积累(转)

http://blog.csdn.net/h_armony/article/details/6764811 1.  嵌入式系统中断服务子程序(ISR) 中断是嵌入式系统中重要的组成部分,这导致了很 ...

linux内存基础知识和相关调优方案

内存是计算机中重要的部件之中的一个.它是与CPU进行沟通的桥梁. 计算机中全部程序的执行都是在内存中进行的.因此内存的性能对计算机的影响很大.内存作用是用于临时存放CPU中的运算数据,以及与硬盘等外部 ...

伙伴系统之伙伴系统概述--Linux内存管理(十五)

在内核初始化完成之后, 内存管理的责任就由伙伴系统来承担. 伙伴系统基于一种相对简单然而令人吃惊的强大算法. Linux内核使用二进制伙伴算法来管理和分配物理内存页面, 该算法由Knowlton设计, ...

现在的 Linux 内核和 Linux 2.6 的内核有多大区别？

作者:larmbr宇链接:https://www.zhihu.com/question/35484429/answer/62964898来源:知乎著作权归作者所有.商业转载请联系作者获得授权,非商业转 ...

随机推荐

Python基于pandas的数据处理(二)

14 抽样 df.sample(10, replace = True) df.sample(3) df.sample(frac = 0.5) # 按比例抽样 df.sample(frac = 10, ...

6-04使用SQL语句更新数据

修改数据语法: UPDATTE 表名 SET 列名 =更新值 WHERE 更新条件 1:省略WHERE条件的更新: 更新性别: UPDATE UserInfo SET Gender=1 三行受影响. ...

有意义的命名 Meaningful names

名副其实 use intention-revealing names 变量.函数或类的名称应该已经答复了所有的大问题.它该告诉你,他为什么会存在,他做什么事,应该怎么用.我们应该选择都是致命了计量对象 ...

Javascript模板及其中的数据逻辑分离思想(MVC)

\#Javascript模板及其中的数据逻辑分离思想 ##需求描述 项目数据库的题目表描述了70-120道题目,并且是会变化的,要根据数据库中的数据描述,比如,选择还是填空题,是不是重点题,题目总分是多 ...

220 DIV2 A. Inna and Pink Pony

Inna and Pink Pony 输入n,m,i,j,a,b 可以看成n行m列的矩阵,起点(i,j),每次移动(a,b),(-a,-b),(-a,b),(a,-b) 可移动到(1,m),(n,1) ...

Python基本程序结构

条件判断: 计算机之所以能做很多自动化的任务,因为它可以自己做条件判断.比如,输入用户年龄,根据年龄打印不同的内容,在Python程序中,用if语句实现:

Linux之批量挂载硬盘

\############parted工具分区############### #!/bin/bash #shell脚本开头格式PATH=/bin:/sbin:/usr/bin:/usr/sbin #保证 ...

使用tf.print()打印tensor内容

使用tf.Print()打印tensor内容,这是tensorflow中调试bug的一个手段,例子如下所示: import tensorflow as tf a = tf.Variable(tf.ra ...

Python2.7-getpass

getpass模块,当用户输入密码时,可以不在屏幕上显示,但是用户也不能看到自己输了几位输了什么 1.模块方法 1.1 getpass.getpass([prompt[, stream]]):prom ...

Web Service 或 WCF调用时读取 XML 数据时，超出最大字符串内容长度配额(8192)解决方法

1.调用服务时服务 当我们使用 Web Service 或 WCF 服务时,常把读取的数据转化为string类型(xml格式),当数据量达到一 定数量时,会出现以下异常: 错误:格式化程序尝试对消息反 ...
