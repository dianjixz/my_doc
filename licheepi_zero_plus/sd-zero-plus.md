---
title : Licheepi zero plus sd启动指南
---
手动制作Licheepi zero plus sd的启动镜像分五步：  
第一步：安装交叉编译工具链。  
第二步：配置编译u-boot  
第三步：配置编译linux  
第四步：配置编译buildroot  
第五步：制作sd启动盘

安装交叉编译器和依赖包
============

网盘地址：http://pan.baidu.com/s/1hsf22fq
国外用户：https://releases.linaro.org/components/toolchain/binaries/6.3-2017.05/arm-linux-gnueabihf/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf.tar.xz  

~~~~ {.sourceCode .sh}
wget https://releases.linaro.org/components/toolchain/binaries/6.3-2017.05/arm-linux-gnueabihf/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf.tar.xz

tar xvf gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf.tar.xz

sudo mv gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf /opt/

vim /etc/bash.bashrc

# add: PATH="$PATH:/opt/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin"

source /etc/bash.bashrc

arm-linux-gnueabihf-gcc -v

sudo apt-get install device-tree-compiler

sudo apt install flex python2 swig python2-dev bison libncurses5-dev libncursesw5-dev
sudo apt-get install libssl-dev bc gawk
#安装依赖包
~~~~

下载编译Uboot
============
~~~~ {.sourceCode .sh}
git clone https://github.com/Lichee-Pi/u-boot.git
#拉取git仓库
#git clone https://github.com/Lichee-Pi/u-boot.git -b s3-l0p-exp

cd u-boot

git checkout s3-l0p-exp
#切换分支
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- Lichee_Zero_Plus_defconfig

make ARCH=arm menuconfig

time make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- 2>&1 | tee build.log
~~~~
在执行**time make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- 2>&1 | tee build.log**命令前也就是在编译之前，我们需要添加u-boot命令和传递内核命令, 使u-boot可以直接从tf卡启动，并传递命令给linux内核：  
在make ARCH=arm menuconfig界面选中第一项：  
> **[*]Enable boot arguments**  
> **()   Boot arguments**  

在**Boot arguments**选项中填入如下命令：
> console=ttyS0,115200 panic=5 rootwait root=/dev/mmcblk0p2 earlyprintk rw rootfstype=ext4 vt.global_cursor_default=0  

在make ARCH=arm menuconfig界面选中第二项：
> **[*]Enable a default value for bootcmd**  
> **()bootcmd value**  

在**bootcmd value**选项中填入以下命令：
> setenv bootm_boot_mode sec;load mmc 0:1 0x41000000 zImage;load mmc 0:1 0x41800000 sun8i-s3-lichee-zero-plus.dtb;bootz 0x41000000 - 0x41800000;  
~~~
    环境命令解析：
    :   -   setenv bootm_boot_mode sec; //初始化boot模式
        -   load mmc 0:1 0x41000000 zImage
            //从SD卡的boot分区内拉取zImage linux镜像到内存0x41000000处
        -   load mmc 0:1 0x41800000 sun8i-s3-lichee-zero-plus.dtb;
            //从SD卡的boot分区内拉取sun8i-s3-lichee-zero-plus.dtb到内存0x41000000处
        -   bootz 0x41000000 （内核地址）- 0x41800000（dtb地址） 启动内核

    启动参数解析
    :   -   console=ttyS0,115200 earlyprintk panic=5 rootwait
            //在串口0上输出信息
        -   root=/dev/mmcblk0p2 earlyprintk rw  vt.global_cursor_default=0
            //mmcblk0p2是设备名，后面是分区大小，名字，读写属性。
        -   //上述命令是指从SD卡启动，根文件系统是ext4
~~~
输入指令：  
> time make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- 2>&1 | tee build.log

开始编译。

编译完成后，在当前目录下生成了u-boot-sunxi-with-spl.bin，可以烧录到sd卡8K偏移处启动。

Kernel编译
=========
下载编译linux源码
---------------
(默认是zero-4.10.y分支),全部拉区可能比较大，可以选用单分支拉取：

~~~~ {.sourceCode .sh}
git clone https://github.com/Lichee-Pi/linux.git  

#git clone https://github.com/Lichee-Pi/linux.git -b s3-5.3-rc2  

cd linux  

git checkout s3-5.3-rc2

make ARCH=arm licheepi_zero_plus_defconfig  

make ARCH=arm menuconfig   #add bluethooth, etc.  

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j16  

#make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j16 INSTALL_MOD_PATH=out modules  

#make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j16 INSTALL_MOD_PATH=out modules_install  

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs

~~~~
编译完成后，zImage在arch/arm/boot/下，驱动模块在out/下

rootfs编译
=========
buildroot可用于构建小型的linux根文件系统。

大小最小可低至2M，与内核一起可以放入最小8M的spi flash中。

buildroot中可以方便地加入第三方软件包（其实已经内置了很多），省去了手工交叉编译的烦恼。

美中不足的是不支持包管理系统，没有gcc等。

下载安装
-------
首先安装一些依赖，比如linux头文件：

>sudo apt-get install linux-headers-$(uname -r)

然后下载安装：
~~~~ {.sourceCode .sh}
wget https://buildroot.org/downloads/buildroot-2017.08.tar.gz
tar xvf buildroot-2017.08.tar.gz
cd buildroot-2017.08/
make menuconfig
~~~~

配置
---
首先配置工具链，因为之前开发uboot和内核都用到了自己下载的工具链，所以这里也配置成外部工具链。


* 1 在本机上外部工具链配置为： /opt/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/
* 2 工具链前缀是： arm-linux-gnueabihf
* 3 外部工具链gcc版本：我们使用的是最新的6.3版本
* 4 外部工具链内核头文件：是在 arm-linux-gnueabi/libc/usr/include/linux/version.h 里读取内核版本信息。本机的版本是4.6
* 5 C库还是选择传统的glibc。需要小体积可以选uclibc（需要自行编译安装）。
* 6 再在system 设置下主机名，root密码等。
* 7 最后就是配置自己需要的软件包，在menuconfig中选中即可。
* 8 有时候下载速度慢，可以复制下载链接，使用迅雷等下载好后，拷贝到dl目录下，会自动识别。

编译
---
> make menuconfig

![](https://img-blog.csdnimg.cn/20190511132948135.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)  
![](https://img-blog.csdnimg.cn/2019051113304589.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0p1bjYyNg==,size_16,color_FFFFFF,t_70)

> make

$ 有时候构建会出现莫名其妙的错误，make clean下会ok？

编译完成后，会生成 output/images/rootfs.tar，此即所需的根文件系统

默认失能串口登录，需要修改 /etc/inittab :
> ttyS0::respawn:/sbin/getty -L ttyS0 115200 vt100 # GENERIC_SERIAL

烧录
===
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
