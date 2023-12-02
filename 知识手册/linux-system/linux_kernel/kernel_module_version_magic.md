version magic翻译过来就是版本魔法，本节我们来了解内核模块的version magic，也就是kernel（内核）和内核模块的版本问题。本节的内容是笔者的一些理解，如有不对的地方，希望大家指正，我们互相学习，共同进步。 作者：正点原子官方 https://www.bilibili.com/read/cv12137195/ 出处：bilibili

02 如何查看内核版本和内核模块版本
2.1 uname和modinfo指令用法

uname [选项参数]

选项参数如下：

-a,  依次按照如下选项输出有关信息：

-s,   输出kernel名称；

-n,  输出主机名；

-r,  输出kernel发行版本号；

-v,  输出操作系统版本；

-m, 输出主机的硬件架构名称；

-p,  输出处理器类型；

-i,   输出硬件平台；

-o,  输出操作系统名称


      一般用的较多的是"uname -r"和"uname -a"。

      modinfo主要用于查看内核模块的信息，用法：

modinfo 模块名

    例如要查看led.ko的信息，直接执行"modinfo led.ko"指令。
2.2 查看kernel和内核模块版本

       在开发板文件系统下执行"uname -r"可以查看kernel发行版号，执行如下指令：

uname -r


        在开发板文件系统的"/lib/modules"下，可以查看内核模块版本（"/lib/modules/"下的文件夹）：
图2.2.1 查看kernel和内核模块版本信息

可以看到，kernel和内核模块的发行版本号都是"4.1.15-gad512fa"。

       "uname -r"查看的是内核发行版本号，"uname -a"可以查看更具体的版本信息，如下：

uname -a


图2.2.2 查看kernel具体版本信息

内核名称：Linux

主机名：ATK-IMX6U

kernel发行版号：4.1.15-gad512fa

操作系统版本和时间：

 #1 SMP PREEMPT Mon Mar 29 16:02:02 CST 2021

硬件架构、处理器类型和硬件平台都是armv7l

操作系统名称：GNU/Linux


        当开发板启动到kernel阶段时，kernel启动时也会打印kernel的版本号：

Linux version 4.1.15-gad512fa (alientek@ubuntu) (gcc version 5.3.0 (GCC) ) #1 SMP PREEMPT Mon Mar 29 16:02:02 CST 2021


    在以上信息中，"(alientek@ubuntu)"是编译kernel所使用的机器，其中，"alientek"是用户名，ubuntu是主机名。"(gccversion 5.3.0 (GCC))"是gcc版本。通过"(alientek@ubuntu)"这一串打印信息，可以初步判断板子上的kernel是否是自己的ubuntu编译出来的（因为不同用户设置的ubuntu用户名会不一样），最后一串"MonMar 29 16:02:02 CST2021"可以显示编译kernel的时间，我们在替换kernel的时候也可以通过这个时间信息来区分此时板子上的kernel是否是我们刚替换的kernel。
图2.2.3 开发板kernel启动的信息


2.3 查看加载模块.ko版本信息

    查看所加载的模块.ko版本：

modinfo led.ko


图2.3.1 查看led.ko版本信息

root@ATK-IMX6U:/lib/modules/4.1.15-gad512fa# modinfo led.ko

filename:/lib/modules/4.1.15-gad512fa/led.ko

author: zuozhongkai

license: GPL

srcversion: 597E1DDC8A372707B8FD0DE

depends:

vermagic: 4.1.15 SMP preempt mod_unload modversions ARMv6 p2v8


        对以上基本信息的简单解释：

filename：文件路径

author：显示模块开发人员

license：协议

srcversion：版本信息

depends：依赖文件

vermagic：即 Version Magic String，也就是编译模块时的内核版本以及SMP与PREEMPT等配置信息。


      根据打印信息可以知道，在led.ko文件的版本信息中，发行版号是"4.1.15"，和前面kernel发行版本号"4.1.15-gad512fa"不一致，而且显示ARMv6，不是ARMv7，这就会导致模块.ko文件无法加载，所以会报前文的错误。


03 kernel版本由什么控制

  下面，笔者以自己移植的kernel为例进行讲解，笔者使用的内核版本是4.1.15。
3.1 kernel基础版本号

      在内核源码根目录的Makefile下可以看到Linux kernel的基础版本号，如下，Linux kernel基础版本号为4.1.15：


图3.1.1 kernel源码根目录下Makefile文件


3.2 kernel拓展版本号

       kernel引入了一些配置来增强版本信息，在内核源码的"include/linux/vermagic.h"下我们可以看到模块的健全版本信息，如下默认配置的有：

MODULE_VERMAGIC_SMP=SMP；

MODULE_VERMAGIC_PREEMPT= preempt；

MODULE_VERMAGIC_MODULE_UNLOAD=mod_unload；

MODULE_VERMAGIC_MODVERSIONS= modversions；


       其余选项的宏是空的，还有一个"VERMAGIC_STRING"宏：

#define VERMAGIC_STRING        \

UTS_RELEASE " "     \

MODULE_VERMAGIC_SMP  MODULE_VERMAGIC_PREEMPT      \

MODULE_VERMAGIC_MODULE_UNLOAD  MODULE_VERMAGIC_MODVERSIONS  \

MODULE_ARCH_VERMAGIC



图片图3.2.1 vermagic.h文件内容

其中，"UTS_RELEASE"的配置在内核源码的"include/generated/utsrelease.h"下可以看到，utsrelease.h的内容是由Makefile和.config的内容来生成的，当成功编译kernel以后，utsrelease.h得到更新，如下图3.2.2：

#defineUTS_RELEASE "4.1.15"



图片图3.2.2 utsrelease.h文件内容

图3.2.2 utsrelease.h文件内容

      那么前文"4.1.15-gad512fa"中的"-gad512fa"是如何得来的呢？其实这个是开发者使用了Git来管理代码（也有使用SVN来管理代码）。如果你在ubuntu安装了Git，在代码维护上就可以使用Git来维护，当你修改或者提交代码以后，每次编译内核后，在"UTS_RELEASE"后面就会看到一串数字，例如"4.1.15-gad512fa"，其中"-gad512fa"这个就是Git版本控制而产生的。

       "4.1.15-gad512fa"这一串就组成了kernel发行版本号，发行版本号只是各个厂商用于区别自己发布的不同时期的kernel版本或者产品版本而产生的编号，完全由各厂商自己定义。

      这么一来，我移植的内核模块的部分版本信息就确定了，也就是：

4.1.15 SMP  preempt   mod_unload  modversions


        led.ko模块在编译时，kernel的版本信息会被添加到模块的modinfo段中，所以我们可以通过指令"modinfo led.ko"来查看模块的version magic信息，当在加载模块的时候，装载器会比较kernel的version magic与模块modinfo段里的version magic信息，如若它们的信息一致，模块才可以被加载，否则就会报错前文的错误提示信息，或者报错找不到模块，如："couldnot search modules: No such file or directory"，又或者报错"Invalidmodule format"。


3.3 CPU核心家族选择

       在前文提到的报错提示中，还注意到ARMv7和ARMv6不一样，它们是CPU核心家族，ARM11是ARMv6架构，Cortex-A、Cortex-R和Cortex-M这三个系列是ARMv7架构，我们使用的阿尔法开发板的CPU是Cortex-A7内核，所以是基于ARMv7的平台，ARMv7是向下兼容ARMv6的。我们可以在配置内核的时候选择CPU核心家族选项。


04 问题解决

     针对前文出现的问题，下面我给出两种解决办法，我个人比较推荐使用第一种解决办法，第二种解决办法是需要修改内核源码的配置，一般不太建议去修改内核源码的这些配置，以免出现其它问题。
4.1 同步更新kernel、内核模块（推荐）

       使用同一个内核源码重新编译kernel、设备树和内核模块，然后将开发板上的kernel和设备树一起更新，并安装内核模块，做好这一步以后再去做驱动实验部分。

/* 编译内核 */

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-zImage

/* 编译设备树（设备树名字根据自己的实际情况来写） */

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-imx6ull-alientek-emmc.dtb

/* 编译内核模块 */

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-modules

/* 安装内核模块 */

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules_install INSTALL_MOD_PATH=后面加上文件系统的路径


      在最后的一条指令中，如果开发板挂载的是ubuntu上的文件系统，此路径就是ubuntu上对应的文件系统路径。如果开发板上挂载的是EMMC或者NAND FLASH里的文件系统，那么可以在ubuntu上新建一个目录，再将内核模块安装到这个目录下，然后再将这个目录里的文件打包。 如下图，我在内核源码下新建了一个目录mymodules，执行如下指令将内核模块安装到这个目录下，然后看到在 "mymodules/lib/modules/4.1.15/"下生成一些文件（ubuntu下最好切换到root用户下来操作）：

make ARCH=armCROSS_COMPILE=arm-linux-gnueabihf- modules_install INSTALL_MOD_PATH=mymodules


图4.1.1 安装内核模块


    然后将生成的文件进行打包，注意这里打包的指令和是在哪个目录下打包的！

tar -vcjf modules.tar.bz2 *


图4.1.2 打包文件
图4.1.2 打包好的压缩包


    如上图，最后得到modules.tar.bz2这个压缩包。如果是EMMC版本的核心板，直接将这个压缩包解压在EMMC文件系统的"/lib/modules"下。如果是NAND FLASH版本的核心板，将内核模块解压到"/mnt/mtd5/lib/modules"下。更新好内核和设备树以后，接下来就可以做驱动实验了。


4.2 修改源码（不推荐）

       如果不想更新开发板上的kernel，也可以通过修改源码来临时解决问题，但一般不太建议去修改源码。

       修改内核源码版本相关信息，如图4.2.1，将内核源码根目录下的Makefile文件修改如下：

EXTRAVERSION=-gad512fa



图片图4.2.1修改内核源码根目录Makefile下的EXTRAVERSION


       执行如下指令重新配置内核：

makeARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig


     配置如图4.2.2，在System Typeà Multiple platform selectio下将CPU核心家族的ARMv6 based platforms (ARM11)选项去掉，只留下ARMv7 based platforms (Cortex-A, PJ4, Scorpion, Krait)这项：
图片图4.2.2 选择CPU核心家族


       保存配置，然后重新编译kernel，在这个过程汇总，最终kernel会根据Makefile和.config的内容来生成内核发行版本"4.1.15-gad512fa"信息，并记录在内核源码的"include/generated/utsrelease.h"下，如图4..2.3是我编译kernel以后打开utsrelease.h文件查看到的内容：

#defineUTS_RELEASE "4.1.15-gad512fa"



图片图4.2.3 编译后查看utsrelease.h文件的内容


         接着我们再编译led.ko文件，再次使用modinfo led.ko查看模块信息：
图片图4.2.4 查看重新编译的led.ko的版本信息


     查看led.ko版本信息是"4.1.15-gad512faSMP preempt mod_unload modversions ARMv7p2v8"，已经和前文报错提示的一致，将led.ko重新拷贝到开发板上（开发板上的kernel不必更新），再次加载led.ko时不再报错，如图4.2.5：
图片图4.2.5 加载led.ko不再报错



      经过前面的讲解，在4.1小节中可能有部分小伙伴不明白为什么要执行如下两个指令，这个问题咱们下次一起来分析，喜欢的小伙伴记得转发、留言和关注“正点原子”公众号哦。

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-modules

/* 安装内核模块 */

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules_install INSTALL_MOD_PATH=后面加上文件系统的路径

 作者：正点原子官方 https://www.bilibili.com/read/cv12137195/ 出处：bilibili