---
title: Licheepi zero SPI Flash 系统编译,小白入门！
---

在一些低成本应用场景，需要在SPI
flash上启动系统，这需要对Uboot和系统镜像做些适配。

本文介绍Licheepi zero SPI Flash镜像的制作过程。主要为小白首次入门做讲解！  
制作嵌入式linux系统主要分四部分：  
第一部分：启动引导U-BOOT的适配编译！  
第二部分：linux设备树的适配编译！  
第三部分：linux内核的适配编译！  
第四部分：rootfs根文件系统的编译制作！  
在下面的过程中，我们的目的是从头开始构建一个linux镜像，让我们的Licheepi zero运行起来！


安装交叉编译工具链
===============

网盘地址：http://pan.baidu.com/s/1hsf22fq  
国外用户：https://releases.linaro.org/components/toolchain/binaries/6.3-2017.05/arm-linux-gnueabihf/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf.tar.xz  
~~~ {.sourceCode .cpp}
wget https://releases.linaro.org/components/toolchain/binaries/6.3-2017.05/arm-linux-gnueabihf/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf.tar.xz
tar xvf gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf.tar.xz
mv gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf /opt/
vim /etc/bash.bashrc
# add: PATH="$PATH:/opt/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin"
source /etc/bash.bashrc
arm-linux-gnueabihf-gcc -v
sudo apt-get install device-tree-compiler
~~~
当输入arm-linux-gnueabihf-gcc -v后有版本信息，那就说明交叉编译工具链安装成功了，然后就可以开始接下来的旅程了。

Uboot编译配置
=============

目前Uboot环境变量固定存放在1MB位置之内，所有留给uboot的空间固定到flash前1MB的位置不变。

每个分区的大小必须是擦除块大小的整数倍，xt25f128b的擦除块大小是64KB。

准备uboot
---------

下载包含spi驱动的体验版本uboot，该驱动目前尚未合并到主线

> `git clone -b v3s-spi-experimental https://github.com/Lichee-Pi/u-boot.git`

配置Flash支持型号
-----------------

执行 `make ARCH=arm menuconfig` 打开uboot菜单配置，进入到
Device Drivers --\> SPI Flash Support

注意看一下自己flash的厂家名称，例如选上:  
winbond SPI flash support  
这个选项的驱动可以用来支持测试用的flash：xt25f128b。  
如果使用的是16MB以上的flash，需要勾选flashbank支持选项：  
<*>SPI flash Bank/Extended address register support  
否则最多只能读到16MB。

配置uboot支持xt25f128b
---------------------
由于需要移植的 flash 芯片特性和 w25qxxx 系列的 flash 相似，所以可以直接复制过来，修改后如下
uboot工程：drivers/mtd/spi/spi_flash_ids.c
添加参数：
~~~~ {.sourceCode .cpp}
onst struct spi_flash_info spi_flash_ids[] = {
    ...
    {"w25q128fw",      INFO(0xef6018, 0x0,  64 * 1024,   256, RD_FULL | WR_QPP | SECT_4K) },
    {"xt25f128b",      INFO(0x0b4018, 0x0,  64 * 1024,   256, RD_FULL | WR_QPP | SECT_4K) },
    ...
};
~~~~
上面的数据格式为：
~~~~ {.sourceCode .cpp}
/* Used when the "_ext_id" is two bytes at most */
#define INFO(_jedec_id, _ext_id, _sector_size, _n_sectors, _flags)    \
        .id = {                         \
            ...
        .id_len = (!(_jedec_id) ? 0 : (3 + ((_ext_id) ? 2 : 0))),   \
        .sector_size = (_sector_size),              \
        .n_sectors = (_n_sectors),              \
        .page_size = 256,                   \
        .flags = (_flags),
~~~~
修改之后的文件
~~~~ {.sourceCode .cpp}
#ifdef CONFIG_SPI_FLASH_WINBOND     /* WINBOND */
    {"w25p80",     INFO(0xef2014, 0x0,  64 * 1024,    16, 0) },
    {"w25p16",     INFO(0xef2015, 0x0,  64 * 1024,    32, 0) },
    {"w25p32",     INFO(0xef2016, 0x0,  64 * 1024,    64, 0) },
    {"w25x40",     INFO(0xef3013, 0x0,  64 * 1024,     8, SECT_4K) },
    {"w25x16",     INFO(0xef3015, 0x0,  64 * 1024,    32, SECT_4K) },
    {"w25x32",     INFO(0xef3016, 0x0,  64 * 1024,    64, SECT_4K) },
    {"w25x64",     INFO(0xef3017, 0x0,  64 * 1024,   128, SECT_4K) },
    {"w25q80bl",       INFO(0xef4014, 0x0,  64 * 1024,    16, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q16cl",       INFO(0xef4015, 0x0,  64 * 1024,    32, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q32bv",       INFO(0xef4016, 0x0,  64 * 1024,    64, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q64cv",       INFO(0xef4017, 0x0,  64 * 1024,   128, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q128bv",      INFO(0xef4018, 0x0,  64 * 1024,   256, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q256",    INFO(0xef4019, 0x0,  64 * 1024,   512, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q80bw",       INFO(0xef5014, 0x0,  64 * 1024,    16, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q16dw",       INFO(0xef6015, 0x0,  64 * 1024,    32, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q32dw",       INFO(0xef6016, 0x0,  64 * 1024,    64, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q64dw",       INFO(0xef6017, 0x0,  64 * 1024,   128, RD_FULL | WR_QPP | SECT_4K) },
    {"w25q128fw",      INFO(0xef6018, 0x0,  64 * 1024,   256, RD_FULL | WR_QPP | SECT_4K) },
    {"xt25f128b",      INFO(0x0b4018, 0x0,  64 * 1024,   256, RD_FULL | WR_QPP | SECT_4K) },
#endif
~~~~

配置uboot默认环境变量
---------------------

在文件 *include/configs/sun8i.h*
中添加默认bootcmd和bootargs的环境变量设置，注意添加的位置在“ *\#include
\<configs/sunxi-common.h\>* ”的前边。

![](https://box.kancloud.cn/b4cce3d6f353a3aabb326dab402d58a3_1642x622.jpg)


~~~~ {.sourceCode .cpp}
#define CONFIG_BOOTCOMMAND   "sf probe 0; "                           \
                            "sf read 0x41800000 0x100000 0x10000; "  \
                            "sf read 0x41000000 0x110000 0x400000; " \
                            "bootz 0x41000000 - 0x41800000"

#define CONFIG_BOOTARGS      "console=ttyS0,115200 earlyprintk panic=5 rootwait " \
                            "mtdparts=spi32766.0:1M(uboot)ro,64k(dtb)ro,4M(kernel)ro,-(rootfs) root=31:03 rw rootfstype=jffs2"
~~~~

环境命令解析：
:   -   sf probe 0; //初始化Flash设备（CS拉低）
    -   sf read 0x41800000 0x100000 0x10000;
        //从flash0x100000（1MB）位置读取dtb放到内存0x41800000偏移处。
        //如果是bsp的bin，则是0x41d00000
    -   sf read 0x41000000 0x110000 0x400000;
        //从flash0x110000（1MB+64KB）位置读取dtb放到内存0x41000000偏移处。
    -   bootz 0x41000000 （内核地址）- 0x41800000（dtb地址） 启动内核

启动参数解析
:   -   console=ttyS0,115200 earlyprintk panic=5 rootwait
        //在串口0上输出信息
    -   mtdparts=spi32766.0:1M(uboot)ro,64k(dtb)ro,4M(kernel)ro,-(rootfs)
        root=31:03 rw rootfstype=jffs2
        //spi32766.0是设备名，后面是分区大小，名字，读写属性。
    -   root=31:03表示根文件系统是mtd3；jffs2格式

编译uboot
---------

~~~~ {.sourceCode .bash}
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- LicheePi_Zero_defconfig
#or make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- LicheePi_Zero_800x480LCD_defconfig
#or make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- LicheePi_Zero480x272LCD_defconfig
time make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- 2>&1 | tee build.log
~~~~

会在目录下生成 *u-boot-sunxi-with-spl.bin*

linux内核编译配置
=================

linux内核基于github上的版本,[分支为最新的zero-4.13.y](https://github.com/Lichee-Pi/linux.git)  
https://github.com/Lichee-Pi/linux.git
国内用户拉取这个仓库可能会比较慢，也可以指拉取但个分支：

内核选项配置
------------
首先执行`make ARCH=arm licheepi_zero_defconfig`定义常量菜单

然后执行 `make ARCH=arm menuconfig` 打开内核菜单配置，

进入到 Device Drivers --\> Memory Technology Device (MTD) support ，

确保选择上mtd的 **\<*\> Command line partition table parsing*\*
支持，该项目用来解析uboot传递过来的flash分区信息。

以及SPI-NOR 设备的支持。

![](https://box.kancloud.cn/3ed4fd5d601aceb7f896521ba4c67cf6_1430x862.jpg)


添加对jffs2文件系统的支持，路径在  
File systems --\> Miscellaneous filesystems --\> Journalling Flash File System v2 (JFFS2) support

![](https://box.kancloud.cn/3be64c60667c0aa3a906f095171d1fda_1396x746.png)


设备树配置
----------

修改dts配置添加spi flash节点

> `vi arch/arm/boot/dts/sun8i-v3s-licheepi-zero.dts`

添加spi节点配置:

> ~~~~ {.sourceCode .bash}
> &spi0 {
>         status ="okay";
>
>         xt25f128b:xt25f128b@0 {
>                 compatible = "jedec,spi-nor";
>                 reg = <0x0>;
>                 spi-max-frequency = <50000000>;
>                 #address-cells = <1>;
>                 #size-cells = <1>;
>         };
>
> };
> ~~~~

![](https://box.kancloud.cn/611c8c327abb212991c3d0c02b0cf6d8_954x809.jpg)


型号列表在drivers/mtd/devices/m25p80.c文件中
这里的flash型号需要在下表之中，否则将无法识别：（注意容量也一定要对应）

~~~~ {.sourceCode .c}
static const struct spi_device_id m25p_ids[] = {
        /*
        * Allow non-DT platform devices to bind to the "spi-nor" modalias, and
        * hack around the fact that the SPI core does not provide uevent
        * matching for .of_match_table
        */
        {"spi-nor"},

        /*
        * Entries not used in DTs that should be safe to drop after replacing
        * them with "spi-nor" in platform data.
        */
        {"s25sl064a"},  {"w25x16"},     {"m25p10"},     {"m25px64"},

        /*
        * Entries that were used in DTs without "jedec,spi-nor" fallback and
        * should be kept for backward compatibility.
        */
        {"at25df321a"}, {"at25df641"},  {"at26df081a"},
        {"mx25l4005a"}, {"mx25l1606e"}, {"mx25l6405d"}, {"mx25l12805d"},
        {"mx25l25635e"},{"mx66l51235l"},
        {"n25q064"},    {"n25q128a11"}, {"n25q128a13"}, {"n25q512a"},
        {"s25fl256s1"}, {"s25fl512s"},  {"s25sl12801"}, {"s25fl008k"},
        {"s25fl064k"},
        {"sst25vf040b"},{"sst25vf016b"},{"sst25vf032b"},{"sst25wf040"},
        {"m25p40"},     {"m25p80"},     {"m25p16"},     {"m25p32"},
        {"m25p64"},     {"m25p128"},
        {"w25x80"},     {"w25x32"},     {"w25q32"},     {"w25q32dw"},
        {"w25q80bl"},   {"w25q128"},    {"w25q256"},

        /* Flashes that can't be detected using JEDEC */
        {"m25p05-nonjedec"},    {"m25p10-nonjedec"},    {"m25p20-nonjedec"},
        {"m25p40-nonjedec"},    {"m25p80-nonjedec"},    {"m25p16-nonjedec"},
        {"m25p32-nonjedec"},    {"m25p64-nonjedec"},    {"m25p128-nonjedec"},

        /* Everspin MRAMs (non-JEDEC) */
        { "mr25h256" }, /* 256 Kib, 40 MHz */
        { "mr25h10" },  /*   1 Mib, 40 MHz */
        { "mr25h40" },  /*   4 Mib, 40 MHz */

        { },
};
~~~~

在上面我们没有找到相应的型号，所以我们需要手动添加上去。添加完如下

~~~~ {.sourceCode .c}
static const struct spi_device_id m25p_ids[] = {
        /*
        * Allow non-DT platform devices to bind to the "spi-nor" modalias, and
        * hack around the fact that the SPI core does not provide uevent
        * matching for .of_match_table
        */
        {"spi-nor"},

        /*
        * Entries not used in DTs that should be safe to drop after replacing
        * them with "spi-nor" in platform data.
        */
        {"s25sl064a"},  {"w25x16"},     {"m25p10"},     {"m25px64"},

        /*
        * Entries that were used in DTs without "jedec,spi-nor" fallback and
        * should be kept for backward compatibility.
        */
        {"at25df321a"}, {"at25df641"},  {"at26df081a"},
        {"mx25l4005a"}, {"mx25l1606e"}, {"mx25l6405d"}, {"mx25l12805d"},
        {"mx25l25635e"},{"mx66l51235l"},
        {"n25q064"},    {"n25q128a11"}, {"n25q128a13"}, {"n25q512a"},
        {"s25fl256s1"}, {"s25fl512s"},  {"s25sl12801"}, {"s25fl008k"},
        {"s25fl064k"},
        {"sst25vf040b"},{"sst25vf016b"},{"sst25vf032b"},{"sst25wf040"},
        {"m25p40"},     {"m25p80"},     {"m25p16"},     {"m25p32"},
        {"m25p64"},     {"m25p128"},
        {"w25x80"},     {"w25x32"},     {"w25q32"},     {"w25q32dw"},
        {"w25q80bl"},   {"w25q128"},    {"xt25f128b"},  {"w25q256"},

        /* Flashes that can't be detected using JEDEC */
        {"m25p05-nonjedec"},    {"m25p10-nonjedec"},    {"m25p20-nonjedec"},
        {"m25p40-nonjedec"},    {"m25p80-nonjedec"},    {"m25p16-nonjedec"},
        {"m25p32-nonjedec"},    {"m25p64-nonjedec"},    {"m25p128-nonjedec"},

        /* Everspin MRAMs (non-JEDEC) */
        { "mr25h256" }, /* 256 Kib, 40 MHz */
        { "mr25h10" },  /*   1 Mib, 40 MHz */
        { "mr25h40" },  /*   4 Mib, 40 MHz */

        { },
};
~~~~
然后我们添加xt25f128b flash的相关信息。  
编辑drivers/mtd/spi-nor/spi-nor.c文件  
添加信息如下：  
>    {"xt25f128b",INFO(0x0b4018, 0x0,  64 * 1024,   256, SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ) },
~~~~ {.sourceCode .bash}
static const struct flash_info spi_nor_ids[] = {
	/* Atmel -- some are (confusingly) marketed as "DataFlash" */
    ...
	{ "w25q80", INFO(0xef5014, 0, 64 * 1024,  16, SECT_4K) },
	{ "w25q80bl", INFO(0xef4014, 0, 64 * 1024,  16, SECT_4K) },
	{ "w25q128", INFO(0xef4018, 0, 64 * 1024, 256, SECT_4K) },
	{"xt25f128b",INFO(0x0b4018, 0x0,  64 * 1024,   256, SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ) },
	{ "w25q256", INFO(0xef4019, 0, 64 * 1024, 512, SECT_4K) },

	/* Catalyst / On Semiconductor -- non-JEDEC */
	...
};
~~~~
开始编译
~~~~ {.sourceCode .bash}
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j32
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs
~~~~

rootfs根文件系统制作  
=================
到这里其实就是不同的发行版之间的差别所在了，基本上所有的linux发行版之间使用的内核大多数都是相同的，主要不同的地方就在于内核启动后在用户空间内的服务程序了。  
我们今天使用的rootfs构建软件是buildroot。  
buildroot可用于构建小型的linux根文件系统。  
大小最小可低至2M，与内核一起可以放入最小8M的spi flash中。  
buildroot中可以方便地加入第三方软件包（其实已经内置了很多），省去了手工交叉编译的烦恼。  
美中不足的是不支持包管理系统，没有gcc等。但是这并不影响我们构建的flash系统镜像。想想在32M运行内存的环境下，你怎么可能使用gcc和包管理程序。  

安装依赖
-------
首先安装一些依赖，比如linux头文件：  
> apt-get install linux-headers-$(uname -r)  

然后下载buildroot
----------------
在此我们选用buildroot-2017.08老版本，老版本的优势在于稳定性。
~~~  {.sourceCode .bash}
wget https://buildroot.org/downloads/buildroot-2017.08.tar.gz
tar xvf buildroot-2017.08.tar.gz
cd buildroot-2017.08/
make menuconfig
~~~
配置
----
首先配置工具链，因为之前开发uboot和内核都用到了自己下载的工具链，所以这里也配置成外部工具链。配置工具链的目的是不用在重复下载工具链了，如果你使用buildroot自带的工具链，那么buildroot在编译的过程中会下载工具链，对于国内用户来说，这个过程可能比较漫长，所以我们直接配置好本地的工具链。
+ 在本机上外部工具链配置为： /opt/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf
+ 工具链前缀是： arm-linux-gnueabihf  
+ 外部工具链gcc版本：我们使用的是6.3版本  
+ 外部工具链内核头文件：是在 arm-linux-gnueabi/libc/usr/include/linux/version.h 里读取内核版本信息。本机的版本是4.6  
+ C库还是选择传统的glibc。需要小体积可以选uclibc（需要自行编译安装）。
再在system 设置下主机名，root密码等。  
+ 最后就是配置自己需要的软件包，在menuconfig中选中即可。  
+ 有时候下载速度慢，可以复制下载链接，使用迅雷等下载好后，拷贝到dl目录下，会自动识别。  

编译
---

make menuconfig
![](/home/nihao/work/my_doc/image/2165161615616516516165)
![](/home/nihao/work/my_doc/image/5616515610321615616156156)


make  
多线程编译：make -j16
> \>>>有时候构建会出现莫名其妙的错误，make clean下会ok？  

编译完成后，会生成 output/images/rootfs.tar，此即所需的根文件系统  

默认失能串口登录，需要修改 /etc/inittab :
> ttyS0::respawn:/sbin/getty -L ttyS0 115200 vt100 # GENERIC_SERIAL  

修改串口这步可以不用，在构建u-boot的时候我们已经给内核传递了参数了。

Flash分区规划
=============

这里 使用 xt25f128b, **16M SPI flash** 作为启动介质，规划分区如下：  
| 分区序号 | 分区大小 | 分区作用 | 地址空间及分区名 |
|   ----  |  ----  |  ----   |  -----------  |
|mtd0     | 1MB    |spl+uboot|0x0000000-0x0100000 : “uboot”|
|mtd1     |64KB    |dtb文件  |0x0100000-0x0110000: “dtb”|
|mtd2     |4MB     |linux内核|0x0110000-0x0510000 : “kernel”|
|mtd3     |剩余     |根文件系统|0x0510000-0x2000000 : “rootfs”|

根据上面的这个表，我门在下面的镜像制作时要用。如果使用到了其它的SPI flash也可以参考这个分区表。


准备镜像文件
============

制作jffs2文件系统
-----------------

Flash支持jffs2文件系统格式，所以需要使用此该rootfs制作jffs2文件系统镜像、

下载jffs2文件系统制作工具

> `apt-get install mtd-utils`

创建rootfs文件夹  

> `mkdir rootfs`

解压 rootfs.tar

> `sudo tar xzvf rootfs.tar -C ./rootfs`

计算好jffs的大小，可以使用 **mkfs.jffs2** 生成

总空间是16M-1M-64K-4M=0xAF0000

> `sudo mkfs.jffs2 -s 0x100 -e 0x10000 -p 0xAF0000 -d rootfs/ -o jffs2.img`

给文件读写权限

> `sudo chmod 666 jffs2.img`

-   页大小0x100 256字节
-   块大小0x10000 64k
-   jffs2分区总空间0xAF0000
-   jffs2.img是生成的文件系统镜像。

最后将uboot，dtb，kernel，rootfs打包成一个系统镜像，命令如下；


~~~~ {.sourceCode .bash}
#!/bin/sh
dd if=/dev/zero of=flashimg.bin bs=1M count=16
dd if=u-boot-sunxi-with-spl.bin of=flashimg.bin bs=1K conv=notrunc
dd if=sun8i-v3s-licheepi-zero.dtb of=flashimg.bin bs=1K seek=1024  conv=notrunc
dd if=zImage of=flashimg.bin bs=1K seek=1088  conv=notrunc
dd if=jffs2.img of=flashimg.bin  bs=1K seek=5184  conv=notrunc
~~~~

第一步： 生成一个空文件，大小是16MB\
第二步： 将uboot添加到文件开头\
第三步： 将dtb放到1M偏移处\
第四步： 将kernel放到1M+64K偏移处

> 第五步： 将rootfs放到1M+64K+4M偏移处

偏移大小是seek，单位是KB。

执行完毕后生成镜像文件 *flashimg.bin*

烧写镜像
========

下载sunxiflash烧写工具
----------------------

> `git clone -b spi-rebase https://github.com/Icenowy/sunxi-tools.git`

> **note**
>
> 注意是spi-rebase分支。

进入工具目录执行 `make && sudo make install`

如果出现：\*fel\_lib.c:26:20: fatal error: libusb.h: No such file or
directory\*，那需要安装libusb：

> `sudo apt-get install libusb-1.0-0-dev`

进入fel模式
-----------

Zero有一个usb下载模式称为fel模式，进入fel模式有下面几种方式：

1.  TF卡和spi flash 同时没有可启动镜像;
    :   也就是说你不插卡，且焊接的是新的或者没有有效镜像的spi
        flash，那就上电自动进入fel下载模式

2.  TF卡中有进入fel模式的特殊固件 *fel-sdboot.sunxi*
    :   如果你的spiflash已经有了启动镜像，那么需要在TF卡中烧入一个sunxi提供的
        启动工具 （
        `dd if=fel-sdboot.sunxi of=/dev/mmcblk0 bs=1024 seek=8` ），
        那么插入该TF卡启动会进入fel模式；

3.  上电时SPI\_MISO拉低到地
    :   该引脚为boot引脚，上电时出于低电平即会进入fel下载模式。

sunxi-fel的操作
---------------

进入fel模式后使用usb数据线连接pc和zero,即可进行操作。

~~~~ {.sourceCode .bash}
sudo sunxi-fel version      #查看连接的cpu信息
AWUSBFEX soc=00001681(V3s) 00000001 ver=0001 44 08 scratchpad=00007e00 00000000 00000000
sudo sunxi-fel spiflash-info    #显示flash信息
Manufacturer: Unknown (C2h), model: 20h, size: 33554432 bytes.
~~~~

执行如下命令烧入我们前边制作好的镜像文件

~~~~ {.sourceCode .bash}
sudo sunxi-fel -p spiflash-write 0 flashimg.bin
# -p 显示进度条
#   spiflash-info           Retrieves basic information
#   spiflash-hex[dump] addr length  Dumps SPI flash region in hex
#   spiflash-read addr length file  Write SPI flash contents into file
#   spiflash-write addr file    Store file contents into SPI flash
~~~~

![](https://box.kancloud.cn/30a15ac70a49ffa8e966700b72d91478_1088x83.jpg)


SPI
flash下载速度约50KB/s，等待5分钟（16MB）或者10分钟（32MB），烧写完成，如果一切顺利，重新上电zero那么就会进入linux系统了，账号是root没有密码。

![](https://box.kancloud.cn/94cba1c9e4539c2e54836d28a8bbe12b_1281x1002.jpg)

> align
> :   center
>
附录： 启动日志
===============

    U-Boot SPL 2017.01-rc2-00073-gdd6e8740dc-dirty (Mar 28 2021 - 01:54:30)
    DRAM: 64 MiB
    Trying to boot from sunxi SPI
    
    U-Boot 2017.01-rc2-00073-gdd6e8740dc-dirty (Mar 28 2021 - 01:54:30 +0800) Allwinner Technology
    
    CPU:   Allwinner V3s (SUN8I 1681)
    Model: Lichee Pi Zero
    DRAM:  64 MiB
    MMC:   SUNXI SD/MMC: 0
    SF: Detected xt25f128b with page size 256 Bytes, erase size 4 KiB, total 16 MiB
    *** Warning - bad CRC, using default environment
    
    In:    serial@01c28000
    Out:   serial@01c28000
    Err:   serial@01c28000


    U-Boot 2017.01-rc2-00073-gdd6e8740dc-dirty (Mar 28 2021 - 01:54:30 +0800) Allwinner Technology
    
    CPU:   Allwinner V3s (SUN8I 1681)
    Model: Lichee Pi Zero
    DRAM:  64 MiB
    MMC:   SUNXI SD/MMC: 0
    SF: Detected xt25f128b with page size 256 Bytes, erase size 4 KiB, total 16 MiB
    *** Warning - bad CRC, using default environment
    
    In:    serial@01c28000
    Out:   serial@01c28000
    Err:   serial@01c28000
    Net:   No ethernet found.
    starting USB...
    No controllers found
    Hit any key to stop autoboot:  0 
    SF: Detected xt25f128b with page size 256 Bytes, erase size 4 KiB, total 16 MiB
    device 0 offset 0x100000, size 0x10000
    SF: 65536 bytes @ 0x100000 Read: OK
    device 0 offset 0x110000, size 0x400000
    SF: 4194304 bytes @ 0x110000 Read: OK
    ## Flattened Device Tree blob at 41800000
       Booting using the fdt blob at 0x41800000
       Loading Device Tree to 42dfa000, end 42dff206 ... OK
    
    Starting kernel ...
    
    [    0.000000] Booting Linux on physical CPU 0x0
    [    0.000000] Linux version 4.10.15-licheepi-zero+ (nihao@nihao-XiaoXin) (gcc version 6.3.1 20170404 (Linaro GCC 6.3-2017.05) ) #6 SMP Sun Mar 28 08:51:06 CST     2021
    [    0.000000] CPU: ARMv7 Processor [410fc075] revision 5 (ARMv7), cr=10c5387d
    [    0.000000] CPU: div instructions available: patching division code
    [    0.000000] CPU: PIPT / VIPT nonaliasing data cache, VIPT aliasing instruction cache
    [    0.000000] OF: fdt:Machine model: Lichee Pi Zero
    [    0.000000] Memory policy: Data cache writealloc
    [    0.000000] percpu: Embedded 14 pages/cpu @c3f63000 s24716 r8192 d24436 u57344
    [    0.000000] Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 16256
    [    0.000000] Kernel command line: console=ttyS0,115200 earlyprintk panic=5 rootwait mtdparts=spi32766.0:1M(uboot)ro,64k(dtb)ro,4M(kernel)ro,-(rootfs)     root=31:03 rw rootfstype=jffs2
    [    0.000000] PID hash table entries: 256 (order: -2, 1024 bytes)
    [    0.000000] Dentry cache hash table entries: 8192 (order: 3, 32768 bytes)
    [    0.000000] Inode-cache hash table entries: 4096 (order: 2, 16384 bytes)
    [    0.000000] Memory: 55152K/65536K available (6144K kernel code, 199K rwdata, 1412K rodata, 1024K init, 262K bss, 10384K reserved, 0K cma-reserved, 0K highmem)
    [    0.000000] Virtual kernel memory layout:
    [    0.000000]     vector  : 0xffff0000 - 0xffff1000   (   4 kB)
    [    0.000000]     fixmap  : 0xffc00000 - 0xfff00000   (3072 kB)
    [    0.000000]     vmalloc : 0xc4800000 - 0xff800000   ( 944 MB)
    [    0.000000]     lowmem  : 0xc0000000 - 0xc4000000   (  64 MB)
    [    0.000000]     pkmap   : 0xbfe00000 - 0xc0000000   (   2 MB)
    [    0.000000]     modules : 0xbf000000 - 0xbfe00000   (  14 MB)
    [    0.000000]       .text : 0xc0008000 - 0xc0700000   (7136 kB)
    [    0.000000]       .init : 0xc0900000 - 0xc0a00000   (1024 kB)
    [    0.000000]       .data : 0xc0a00000 - 0xc0a31f00   ( 200 kB)
    [    0.000000]        .bss : 0xc0a33000 - 0xc0a7484c   ( 263 kB)
    [    0.000000] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
    [    0.000000] Hierarchical RCU implementation.
    [    0.000000] 	Build-time adjustment of leaf fanout to 32.
    [    0.000000] 	RCU restricting CPUs from NR_CPUS=8 to nr_cpu_ids=1.
    [    0.000000] RCU: Adjusting geometry for rcu_fanout_leaf=32, nr_cpu_ids=1
    [    0.000000] NR_IRQS:16 nr_irqs:16 16
    [    0.000000] arm_arch_timer: Architected cp15 timer(s) running at 24.00MHz (virt).
    [    0.000000] clocksource: arch_sys_counter: mask: 0xffffffffffffff max_cycles: 0x588fe9dc0, max_idle_ns: 440795202592 ns
    [    0.000007] sched_clock: 56 bits at 24MHz, resolution 41ns, wraps every 4398046511097ns
    [    0.000018] Switching to timer-based delay loop, resolution 41ns
    [    0.000138] clocksource: timer: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 79635851949 ns
    [    0.000343] Console: colour dummy device 80x30
    [    0.000379] Calibrating delay loop (skipped), value calculated using timer frequency.. 48.00 BogoMIPS (lpj=240000)
    [    0.000393] pid_max: default: 32768 minimum: 301
    [    0.000528] Mount-cache hash table entries: 1024 (order: 0, 4096 bytes)
    [    0.000539] Mountpoint-cache hash table entries: 1024 (order: 0, 4096 bytes)
    [    0.001231] CPU: Testing write buffer coherency: ok
    [    0.001634] /cpus/cpu@0 missing clock-frequency property
    [    0.001658] CPU0: thread -1, cpu 0, socket 0, mpidr 80000000
    [    0.002018] Setting up static identity map for 0x40100000 - 0x40100058
    [    0.002763] smp: Bringing up secondary CPUs ...
    [    0.002781] smp: Brought up 1 node, 1 CPU
    [    0.002790] SMP: Total of 1 processors activated (48.00 BogoMIPS).
    [    0.002797] CPU: All CPU(s) started in SVC mode.
    [    0.003584] devtmpfs: initialized
    [    0.006342] VFP support v0.3: implementor 41 architecture 2 part 30 variant 7 rev 5
    [    0.006638] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19112604462750000 ns
    [    0.006665] futex hash table entries: 256 (order: 2, 16384 bytes)
    [    0.006819] pinctrl core: initialized pinctrl subsystem
    [    0.007825] NET: Registered protocol family 16
    [    0.008277] DMA: preallocated 256 KiB pool for atomic coherent allocations
    [    0.009494] hw-breakpoint: found 5 (+1 reserved) breakpoint and 4 watchpoint registers.
    [    0.009510] hw-breakpoint: maximum watchpoint size is 8 bytes.
    [    0.020909] SCSI subsystem initialized
    [    0.021188] usbcore: registered new interface driver usbfs
    [    0.021249] usbcore: registered new interface driver hub
    [    0.021344] usbcore: registered new device driver usb
    [    0.021581] pps_core: LinuxPPS API ver. 1 registered
    [    0.021591] pps_core: Software ver. 5.3.6 - Copyright 2005-2007 Rodolfo Giometti <giometti@linux.it>
    [    0.021614] PTP clock support registered
    [    0.021817] Advanced Linux Sound Architecture Driver Initialized.
    [    0.023618] clocksource: Switched to clocksource arch_sys_counter
    [    0.034049] NET: Registered protocol family 2
    [    0.034678] TCP established hash table entries: 1024 (order: 0, 4096 bytes)
    [    0.034710] TCP bind hash table entries: 1024 (order: 1, 8192 bytes)
    [    0.034734] TCP: Hash tables configured (established 1024 bind 1024)
    [    0.034821] UDP hash table entries: 256 (order: 1, 8192 bytes)
    [    0.034868] UDP-Lite hash table entries: 256 (order: 1, 8192 bytes)
    [    0.035077] NET: Registered protocol family 1
    [    0.035696] RPC: Registered named UNIX socket transport module.
    [    0.035716] RPC: Registered udp transport module.
    [    0.035721] RPC: Registered tcp transport module.
    [    0.035727] RPC: Registered tcp NFSv4.1 backchannel transport module.
    [    0.037888] workingset: timestamp_bits=30 max_order=14 bucket_order=0
    [    0.046839] NFS: Registering the id_resolver key type
    [    0.046892] Key type id_resolver registered
    [    0.046899] Key type id_legacy registered
    [    0.046946] jffs2: version 2.2. (NAND) © 2001-2006 Red Hat, Inc.
    [    0.051417] Block layer SCSI generic (bsg) driver version 0.4 loaded (major 249)
    [    0.051438] io scheduler noop registered
    [    0.051445] io scheduler deadline registered
    [    0.051651] io scheduler cfq registered (default)
    [    0.055818] sun8i-v3s-pinctrl 1c20800.pinctrl: initialized sunXi PIO driver
    [    0.123327] Serial: 8250/16550 driver, 8 ports, IRQ sharing disabled
    [    0.126634] console [ttyS0] disabled
    [    0.146912] 1c28000.serial: ttyS0 at MMIO 0x1c28000 (irq = 31, base_baud = 1500000) is a U6_16550A
    [    0.711492] console [ttyS0] enabled
    [    0.715681] [drm] Initialized
    [    0.721398] m25p80 spi32766.0: xt25f128b (16384 Kbytes)
    [    0.726750] 4 cmdlinepart partitions found on MTD device spi32766.0
    [    0.733012] Creating 4 MTD partitions on "spi32766.0":
    [    0.738182] 0x000000000000-0x000000100000 : "uboot"
    [    0.743783] 0x000000100000-0x000000110000 : "dtb"
    [    0.748907] 0x000000110000-0x000000510000 : "kernel"
    [    0.754338] 0x000000510000-0x000001000000 : "rootfs"
    [    0.760036] libphy: Fixed MDIO Bus: probed
    [    0.764482] ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
    [    0.771009] ehci-platform: EHCI generic platform driver
    [    0.776367] ohci_hcd: USB 1.1 'Open' Host Controller (OHCI) Driver
    [    0.782564] ohci-platform: OHCI generic platform driver
    [    0.788269] udc-core: couldn't find an available UDC - added [g_cdc] to list of pending drivers
    [    0.797943] sun6i-rtc 1c20400.rtc: rtc core: registered rtc-sun6i as rtc0
    [    0.804824] sun6i-rtc 1c20400.rtc: RTC enabled
    [    0.809359] i2c /dev entries driver
    [    0.814225] input: ns2009_ts as /devices/platform/soc/1c2ac00.i2c/i2c-0/0-0048/input/input0
    [    0.823891] sunxi-wdt 1c20ca0.watchdog: Watchdog enabled (timeout=16 sec, nowayout=0)
    [    0.883660] sunxi-mmc 1c0f000.mmc: base:0xc48ae000 irq:23
    [    0.889996] usbcore: registered new interface driver usbhid
    [    0.895655] usbhid: USB HID core driver
    [    0.901162] NET: Registered protocol family 17
    [    0.905844] Key type dns_resolver registered
    [    0.910261] Registering SWP/SWPB emulation handler
    [    0.921360] usb_phy_generic.0.auto supply vcc not found, using dummy regulator
    [    0.929304] musb-hdrc musb-hdrc.1.auto: MUSB HDRC host driver
    [    0.935148] musb-hdrc musb-hdrc.1.auto: new USB bus registered, assigned bus number 1
    [    0.946351] hub 1-0:1.0: USB hub found
    [    0.950215] hub 1-0:1.0: 1 port detected
    [    0.955007] using random self ethernet address
    [    0.959500] using random host ethernet address
    [    0.965051] usb0: HOST MAC 0a:4c:0a:fb:ad:c7
    [    0.969383] usb0: MAC 36:23:1c:5c:34:1c
    [    0.973259] g_cdc gadget: CDC Composite Gadget, version: King Kamehameha Day 2008
    [    0.980840] g_cdc gadget: g_cdc ready
    [    0.984859] sun6i-rtc 1c20400.rtc: setting system clock to 1970-01-01 00:00:23 UTC (23)
    [    0.993097] vcc3v0: disabling
    [    0.996144] vcc5v0: disabling
    [    0.999113] ALSA device list:
    [    1.002075]   No soundcards found.
    [    1.007541] random: fast init done
    [    1.049938] random: crng init done
    [    1.580278] VFS: Mounted root (jffs2 filesystem) on device 31:3.
    [    1.587278] devtmpfs: mounted
    [    1.591420] Freeing unused kernel memory: 1024K
    Starting logging: OK
    Starting mdev...
    modprobe: can't change directory to '/lib/modules': No such file or directory
    [    5.688048] ------------[ cut here ]------------
    [    5.692711] WARNING: CPU: 0 PID: 86 at drivers/mtd/spi-nor/spi-nor.c:1183 spi_nor_write+0x138/0x1b4
    [    5.701820] Writing at offset 216 into a NOR page. Writing partial pages may decrease reliability and increase wear of NOR flash.
    [    5.701825] Modules linked in:
    [    5.716550] CPU: 0 PID: 86 Comm: touch Not tainted 4.10.15-licheepi-zero+ #6
    [    5.723585] Hardware name: Allwinner sun8i Family
    [    5.728313] [<c010e2fc>] (unwind_backtrace) from [<c010b098>] (show_stack+0x10/0x14)
    [    5.736055] [<c010b098>] (show_stack) from [<c03465c4>] (dump_stack+0x84/0x98)
    [    5.743277] [<c03465c4>] (dump_stack) from [<c011b634>] (__warn+0xe8/0x100)
    [    5.750235] [<c011b634>] (__warn) from [<c011b684>] (warn_slowpath_fmt+0x38/0x48)
    [    5.757715] [<c011b684>] (warn_slowpath_fmt) from [<c0452fbc>] (spi_nor_write+0x138/0x1b4)
    [    5.765973] [<c0452fbc>] (spi_nor_write) from [<c044b454>] (mtd_writev+0xa4/0xec)
    [    5.773456] [<c044b454>] (mtd_writev) from [<c02fbec8>] (jffs2_flash_writev+0x414/0x4b0)
    [    5.781541] [<c02fbec8>] (jffs2_flash_writev) from [<c02f4d48>] (jffs2_write_dnode+0xd4/0x328)
    [    5.790145] [<c02f4d48>] (jffs2_write_dnode) from [<c02f9458>] (jffs2_do_setattr+0x280/0x558)
    [    5.798661] [<c02f9458>] (jffs2_do_setattr) from [<c02f9754>] (jffs2_setattr+0x24/0x48)
    [    5.806660] [<c02f9754>] (jffs2_setattr) from [<c020d990>] (notify_change+0x1c4/0x3d0)
    [    5.814572] [<c020d990>] (notify_change) from [<c021e65c>] (utimes_common+0xa8/0x170)
    [    5.822394] [<c021e65c>] (utimes_common) from [<c021e7d4>] (do_utimes+0xb0/0x144)
    [    5.829869] [<c021e7d4>] (do_utimes) from [<c021ea10>] (SyS_futimesat+0xd0/0xfc)
    [    5.837259] [<c021ea10>] (SyS_futimesat) from [<c0107580>] (ret_fast_syscall+0x0/0x3c)
    [    5.845234] ---[ end trace d947ef19d0d7cd8d ]---
    Initializing random number generator... done.
    Starting network: OK
    
    Welcome to Lichee Pi
    Lichee login: 


附录：sunxi-fel帮助说明
=======================

~~~~ {.sourceCode .bash}
sunxi-fel v1.4.1-87-g78a7566

Usage: sunxi-fel [options] command arguments--- [command---]
    -h, --help          Print this usage summary and exit
    -v, --verbose           Verbose logging
    -p, --progress          "write" transfers show a progress bar
    -l, --list          Enumerate all (USB) FEL devices and exit
    -d, --dev bus:devnum        Use specific USB bus and device number
        --sid SID           Select device by SID key (exact match)

    spl file            Load and execute U-Boot SPL
        If file additionally contains a main U-Boot binary
        (u-boot-sunxi-with-spl.bin), this command also transfers that
        to memory (default address from image), but won't execute it.

    uboot file-with-spl     like "spl", but actually starts U-Boot
        U-Boot execution will take place when the fel utility exits.
        This allows combining "uboot" with further "write" commands
        (to transfer other files needed for the boot).

    hex[dump] address length    Dumps memory region in hex
    dump address length     Binary memory dump
    exe[cute] address       Call function address
    reset64 address         RMR request for AArch64 warm boot
    memmove dest source size    Copy <size> bytes within device memory
    readl address           Read 32-bit value from device memory
    writel address value        Write 32-bit value to device memory
    read address length file    Write memory contents into file
    write address file      Store file contents into memory
    write-with-progress addr file   "write" with progress bar
    write-with-gauge addr file  Output progress for "dialog --gauge"
    write-with-xgauge addr file Extended gauge output (updates prompt)
    multi[write] # addr file ---    "write-with-progress" multiple files,
                    sharing a common progress status
    multi[write]-with-gauge --- like their "write-with-*" counterpart,
    multi[write]-with-xgauge ---      but following the 'multi' syntax:
                    <#> addr file [addr file [---]]
    echo-gauge "some text"      Update prompt/caption for gauge output
    ver[sion]           Show BROM version
    sid             Retrieve and output 128-bit SID key
    clear address length        Clear memory
    fill address length value   Fill memory
    spiflash-info           Retrieves basic information
    spiflash-hex[dump] addr length  Dumps SPI flash region in hex
    spiflash-read addr length file  Write SPI flash contents into file
    spiflash-write addr file    Store file contents into SPI flash
~~~~

参考资料
========

<http://blog.sina.com.cn/s/blog_5ed5a1f40100f3qq.html>
