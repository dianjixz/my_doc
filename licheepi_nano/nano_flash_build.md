---
title: nano spi flash linux构建指南
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

sudo apt install flex python2 swig python2-dev bison libncurses5-dev libncursesw5-dev
sudo apt-get install libssl-dev bc gawk
#安装依赖包
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
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- licheepi_nano_spiflash_defconfig
#nano flash的默认配置

# 若不带spi-flash的板子，可以换成licheepi_nano_defconfig

# 进行可视化配置
make ARCH=arm menuconfig

~~~
在这个u-boot中，我们可以选择另一种方式配置u-boot的启动参数。  
在可视化menuconfig菜单中：  
选中**Enable boot arguments**选项，然后在下面会出现**( )Boot arguments (NEW) **选项，我们需要在这个新出现的选项中填入我们给内核的启动命令。
> **console=ttyS0,115200 panic=5 rootwait root=/dev/mtdblock3 rw rootfstype=jffs2**

* console=ttyS0,115200 #信息输出到串口0，串口0的波特率为115200
* root=/dev/mtdblock3  #指的是mtd设备第三分区，分区指定在dts中声明，是系统挂载的分区
* rootfstype=jffs2     #挂载的文件系统为jffs2，这个需要linux内核开启jffs2支持  

选中**Enable a default value for bootcmd**选项，同样会在下面出现**bootcmd value (NEW)**的选项，同样的，我们需要在这个新出现的选项中填入我们u-boot的引导命令。  
> ***sf probe 0 50000000;sf read 0x80C00000 0x100000 0x4000;sf read 0x80008000 0x110000 0x400000;bootz 0x80008000 - 0x80C00000;***
* sf是uboot的spi flash设备命令
* sf probe 0 50000000     #选择flash0，flash时钟频率是50MHz
* sf read 0x80C00000 0x100000 0x4000#读取 spi-flash 1M（0x100000）位置 64KB(0x4000)大小的 dtb 到内存地址 0x80C00000
* sf read 0x80008000 0x110000 0x400000#读取 spi-flash 1M+64K（0x110000）位置 4MB(0x400000)大小的 zImage 到内存地址 0x80008000
* bootz 0x80008000 - 0x80C00000#启动内核专有命令，从 0x80008000 启动内核，从 0x80C00000 读取设备树配置

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
git clone https://github.com/Icenowy/linux.git
#克隆linux分支

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

用dd命令制作镜像
-------------
在这里我们可以写一个脚本来完成该部分的操作
~~~
#!/bin/bash
dd if=/dev/zero of=flashimg.bin bs=1M count=16
#创建一个16M的空二进制文件  

dd if=u-boot-sunxi-with-spl.bin of=flashimg.bin bs=1K conv=notrunc
#将uboot文件写入镜像中  

dd if=suniv-f1c100s-licheepi-nano.dtb of=flashimg.bin bs=1K seek=1024  conv=notrunc  
#将*.dtb文件写入镜像中  

dd if=zImage of=flashimg.bin bs=1K seek=1088  conv=notrunc  
#将linux内核zImage文件写入镜像中  

mkdir rootfs  

sudo tar xvf rootfs.tar -C ./rootfs
#将生成的rootfs.tar解压到rootfs文件夹内

sudo mkfs.jffs2 -s 0x100 -e 0x10000 --pad=0xAF0000 -d rootfs/ -o jffs2.img
# 为根文件系统制作jffs2镜像包
# --pad参数指定 jffs2大小
# 由此计算得到 0x1000000(16M)-0x10000(64K)-0x100000(1M)-0x400000(4M)=0xAF0000

sudo chmod 666 jffs2.img
#为生成的镜像赋予读写权限

dd if=jffs2.img of=flashimg.bin  bs=1K seek=5184  conv=notrunc
#将生成jffs2.img二进制文件写入镜像内

~~~
我们可以逐条执行命令，也可以将这些内容复制到脚本文件内，执行脚本。  
命令执行完毕后flashimg.bin文件就是我们制作好的镜像文件。我们可以用烧写工具来将这个镜像烧写到licheepi nano板子内。

烧写镜像
======

准备烧写工具
----------
> `git clone -b spi-rebase https://github.com/Icenowy/sunxi-tools.git`

编译安装烧写工具
---------

下载依赖包  
> sudo apt-get install libusb-1.0-0-dev  

进入工具包内
> make && sudo make install
进行安装工具包

烧写
----
烧写的时候我们首先要进入芯片的fel模式  
nano有一个usb下载模式称为fel模式，进入fel模式有下面几种方式：  
* TF卡和spi flash 同时没有可启动镜像;  
也就是说你不插卡，且焊接的是新的或者没有有效镜像的spi flash，那就上电自动进入fel下载模式  
* TF卡中有进入fel模式的特殊固件 fel-sdboot.sunxi  
如果你的spiflash已经有了启动镜像，那么需要在TF卡中烧入一个sunxi提供的 启动工具 （ dd if=fel-sdboot.sunxi of=/dev/mmcblk0 bs=1024 seek=8 ）， 那么插入该TF卡启动会进入fel模式；
* 上电时SPI_MISO拉低到地  
该引脚为boot引脚，上电时出于低电平即会进入fel下载模式。  

进入fel模式后使用usb数据线连接pc和zero,即可进行操作。  
~~~
sudo sunxi-fel version              #查看连接的cpu信息
AWUSBFEX soc=00001681(V3s) 00000001 ver=0001 44 08 scratchpad=00007e00 00000000 00000000
sudo sunxi-fel spiflash-info        #显示flash信息
Manufacturer: Unknown (C2h), model: 20h, size: 33554432 bytes.
~~~
执行如下命令烧入我们前边制作好的镜像文件
~~~
sudo sunxi-fel -p spiflash-write 0 flashimg.bin
# -p 显示进度条
#   spiflash-info                   Retrieves basic information
#   spiflash-hex[dump] addr length  Dumps SPI flash region in hex
#   spiflash-read addr length file  Write SPI flash contents into file
#   spiflash-write addr file        Store file contents into SPI flash
~~~





