---
title: SD 系统编译烧写
---

前言
===

本文介绍Licheepi nano SPI Flash镜像的制作过程。主要为小白首次入门做讲解！  
制作嵌入式linux系统主要分四部分：  
第一部分：启动引导U-BOOT的适配编译！  
第二部分：linux设备树的适配编译！  
第三部分：linux内核的适配编译！  
第四部分：rootfs根文件系统的编译制作！  
在下面的过程中，我们的目的是从头开始构建一个linux镜像，让我们的Licheepi nano运行起来！

安装交叉编译工具链
===============

工具链下载网址：http://releases.linaro.org/components/toolchain/binaries/7.2-2017.11/arm-linux-gnueabi/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi.tar.xz
~~~
# 此处为获取7.2.1版本，您可获取其他版本或者通过链接直接下载
wget http://releases.linaro.org/components/toolchain/binaries/7.2-2017.11/arm-linux-gnueabi/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi.tar.xz

tar -vxJf gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi.tar.xz
sudo cp -r ./gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi /opt/

sudo vim /etc/bash.bashrc

# 在文件末尾 添加以下内容
PATH="$PATH:/opt/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/bin"
# 添加完毕

# 使路径生效
source /etc/bash.bashrc
~~~
终端输入:arm-linux-gnueabi-gcc -v  
如果显示出版本信息，那么工具链就安装成功了。  

U-BOOT编译
=========

目前Uboot环境变量固定存放在1MB位置之内，所有留给uboot的空间固定到flash前1MB的位置不变。  
我的nano使用的flash型号是xt25f128b  
每个分区的大小必须是擦除块大小的整数倍，xt25f128b的擦除块大小是64KB。

克隆u-boot
---------
~~~
git clone https://github.com/Lichee-Pi/u-boot.git
#克隆u-boot源代码

#git clone https://github.com/Lichee-Pi/u-boot.git -b nano-v2018.01 克隆nano-v2018.01分支源码

cd u-boot

git branch -a
#查看u-boot分支

git checkout nano-v2018.01
#切换到nano分支

~~~
配置
----

~~~
# 此处告知make采用arm-linux-gnueabi下的所有交叉编译工具，目标架构为Arm，设定各项默认配置为 nano 的spiflash支持版
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- licheepi_nano_defconfig
#nano flash的默认配置


# 进行可视化配置
make ARCH=arm menuconfig

~~~
在这个u-boot中，我们可以选择另一种方式配置u-boot的启动参数。  
在可视化menuconfig菜单中：  
选中**Enable boot arguments**选项，然后在下面会出现**( )Boot arguments (NEW) **选项，我们需要在这个新出现的选项中填入我们给内核的启动命令。
> **console=ttyS0,115200 panic=5 rootwait root=/dev/mmcblk0p2 earlyprintk rw  vt.global_cursor_default=0**

* console=ttyS0,115200 earlyprintk panic=5 rootwait //在串口0上输出信息  
* root=/dev/mmcblk0p2 earlyprintk rw  vt.global_cursor_default=0 //mmcblk0p2是设备名，后面是分区大小，名字，读写属性。  上述命令是指从SD卡启动，根文件系统是ext4

**配置Boot arguments的作用是用来给内核传递启动命令的。告诉内核在将启动信息打印到哪里，挂载的根文件系统是哪个！**  

选中**Enable a default value for bootcmd**选项，同样会在下面出现**bootcmd value (NEW)**的选项，同样的，我们需要在这个新出现的选项中填入我们u-boot的引导命令。  
> ***setenv bootm_boot_mode sec;load mmc 0:1 0x80008000 zImage;load mmc 0:1 0x80C00000 suniv-f1c100s-licheepi-nano.dtb; bootz 0x80008000 - 0x80C00000;***
* setenv bootm_boot_mode sec#初始化sd卡
* load mmc 0:1 0x80008000 zImage     #读取linux内核到内存0x80008000地址处
* load mmc 0:1 0x80C00000 suniv-f1c100s-licheepi-nano.dtb#读取设备树到内存0x80C00000地址处
* bootz 0x80008000 - 0x80C00000#启动内核专有命令，从 0x80008000 启动内核，从 0x80C00000 读取设备树配置  

**配置bootcmd的目的是让我们的u-boot启动完成后需要运行的命令。虽然这些命令能够手动运行，但是每次开机都要手动输入这么命令来启动内核，岂不是很麻烦。当然调试的时候可以手动启动查看错误信息**  
配置好后相关启动命令就可以编译了。

编译
---
~~~
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -j8
#使用arm-linux-gnueabi编译工具进行编译，目标架构为arm
~~~
编译生成u-boot-sunxi-with-spl.bin就是我们需要的文件
稍后制作镜像的时候需要使用。

linux编译
========

克隆源代码
--------
~~~
git clone https://gitee.com/LicheePiNano/Linux.git
#克隆linux分支
cd Linux
git branch -a
#查看全部的分支

~~~


配置
---
首先选用默认配置  
> make ARCH=arm f1c100s_nano_linux_defconfig

编译
---
> make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -j32
> make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- dtbs

便已完成后会生成zImage文件和suniv-f1c100s-licheepi-nano.dtb文件，这两个文件是我们制作镜像所需要的文件。

buildroot编译
============

安装依赖
------

> `apt-get install linux-headers-$(uname -r)`  
> #安装一些linux内核头文件

克隆源码包
--------
~~~
wget https://buildroot.org/downloads/buildroot-2017.08.tar.gz
tar xvf buildroot-2017.08.tar.gz
cd buildroot-2017.08/
make menuconfig
~~~
配置
---
~~~
Target options  --->
    Target Architecture (ARM (little endian))  ---> //linux小端模式
    Target Architecture Variant (arm926t)  --->   // arm926ejs架构
[ ] Enable VFP extension support                  // Nano 没有 VFP单元，勾选会导致某些应用无法运行
    Target ABI (EABI)  --->
    Floating point strategy (Soft float)  --->    // 软浮点



System configuration  --->

    (Lichee Pi) System hostname                   // hostname
    (licheepi) Root password                      // 默认账户为root 密码为licheepi
    [*] remount root filesystem read-write during boot  // 启动时重新挂在文件系统使其可读写
~~~
> note:在这里我们不使用本机的交叉编译工具链，不使用glibc，主要原因是使用这两个时会出现非法指令的情况。  

之后你可以在**System configuration**中设置你的主机名和root密码。  

编译
---
> `make -j32`  

编译的过程中会下载主机编译工具链，同时下载选中的软件包，这阶段比较慢，大家可以先下载好相关软件包放在dl文件夹内，这样编译时就不会再下载了。  
编译完成后会在output/images文件夹内生成rootfs.tar文件，这个文件就是我们需要的rootfs系统文件。

构建系统镜像
==========
有了前面我们编译生成的文件，我们现在就可以开始制作linux镜像系统了。  

创建img文件夹
-----------
> `mkdir img`  
> `cd img`  

拷贝相关文件到该文件夹内
--------------------
> `cp your-uboot-dir/u-boot-sunxi-with-spl.bin ./`  
> `cp you-linux-dir/arch/arm/boot/dts/suniv-f1c100s-licheepi-nano.dtb ./`  
> `cp your-linux-dir/arch/arm/boot/zImage ./`  
> `cp your-buildroot-dir/output/images/rootfs.tar ./`  


插上SD卡

![](https://img-blog.csdnimg.cn/20190510220723377.png)

用分区软件进行分区

![](https://img-blog.csdnimg.cn/20190510220759427.png)

![](https://img-blog.csdnimg.cn/20190510220819505.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510220841298.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510220925512.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510220947383.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510221013249.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510221031234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510221103462.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510221115664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510221144802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

将uboot引导文件写入SD相应位置
> sudo dd if=u-boot-sunxi-with-spl.bin of=/dev/sdX bs=1024 seek=8

![](https://img-blog.csdnimg.cn/20190510221336847.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510221437146.png)

将linux镜像和设备树文件复制进boot分区  
图片内的文件名字可能不对，大家不必在意  
![](https://img-blog.csdnimg.cn/20190510221510705.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

将我们生成的根文件包解压到主分区

![](https://img-blog.csdnimg.cn/20190510222318944.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190510222350188.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

弹出SD卡，系统盘启动完成。

插卡起飞！
=======

串口登陆
------
这里要留意下，板子用的是串口0，不是串口I1，板子上串口1和串口2放在一起，之前一直忽略了串口0，导致一直以为设备没启动

![](https://img-blog.csdnimg.cn/20190511133122919.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190511133202574.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)
