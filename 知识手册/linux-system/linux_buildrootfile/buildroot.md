 

# BuildRoot使用一

build中文手册:https://blog.csdn.net/pwl999/article/details/118766987

三月四日图

[![img](https://upload.jianshu.io/users/upload_avatars/13094704/adecdc94-943d-402f-ab51-edd2c51f8f51?imageMogr2/auto-orient/strip|imageView2/1/w/96/h/96/format/webp)](https://www.jianshu.com/u/b5eb095716af)

[Bug2Coder](https://www.jianshu.com/u/b5eb095716af)

2020.05.09 11:49:02字数 2,139阅读 3,571

### 1. 简介

[Buildroot](https://links.jianshu.com/go?to=https%3A%2F%2Fbuildroot.org%2F)是Linux平台上一个构建嵌入式Linux系统的框架。
 整个Buildroot是由Makefile(*.mk)脚本和Kconfig(Config.in)配置文件构成的，因此可以像配置Linux内核一样执行`make menuconfig`进行配置，编译出一个完整的、可以直接烧写到机器上运行的Linux系统文件(包含bootloader、kernel、rootfs以及rootfs中的各种库和应用程序)。
 工作流程

> - 获取：获取源代码
> - 解压：解压源代码
> - 补丁：针对缺陷修复和增加的功能应用补丁
> - 配置：根据环境准备构建过程
> - 安装：复制二进制和辅助文件到它们的目标目录
> - 打包：为在其它系统上安装而打包二进制和辅助文件

### 2. 配置选项

执行`make menuconfig`进入配置页面、Y选中项目、N删除选中，Tab切换功能选项

```bash
Target options  --->              #目标属性                                                                           
Build options  --->               # 构建属性                                                                        
Toolchain  --->                    # 工具链                                                                             
System configuration  --->      # 系统配置项                                                                          
Kernel  --->                        # 内核                                                                 
Target packages  --->        # 目标包                                                                    
Filesystem images  --->     # 文件系统镜像                                                                         
Bootloaders  --->               #   引导程序                                                                           
Host utilities  --->               # 主机程序                                                                       
Legacy config options  --->  # 传统配置选项
```

### 2.1 Target options(目标属性)

```bash
Target options  ---> 
    Target Architecture (ARM (little endian))  --->  //目标处理器的架构和大小端模式 [ARM (little endian)]
    Target Binary Format (ELF)  --->                 //目标二进制格式 [ELF]
    Target Architecture Variant (cortex-A7)  --->    //目标处理器核心类型 [cortex-A7]
    Target ABI (EABIhf)  --->                        //目标应用程序二进制接口(Application Binary Interface) [EABIhf] ①
    Floating point strategy (NEON/VFPv4)  --->       //浮点运算策略 [NEON/VFPv4] ②
    ARM instruction set (ARM)  --->   
```

①：ABI是X86计算机上的，EABI是嵌入式平台上; EABI/EABIhf分别适用于armel和armhf两个不同的架构，armel和armh在对待浮点运算采取了不同的策略(有fpu的arm才能选择EABIhf)；
 ②：ARM浮点体系结构(VFP)，VFPvX为历史各版本，比如浮点运算指定为VFP4(vector floating point4)指令或neon向量浮点指令；

### 2.2 Build options(编译选项)

```bash
Build options  --->  
        Commands  --->                                           //指定下载、解压命令参数选项 
            (wget --passive-ftp -nd -t 3) Wget command //用于常规FTP/HTTP下载压缩包 [被动传输模式;不创建目录;超时重试次数为3]
            (svn --non-interactive) Subversion (svn) command     //通过SSH下载压缩包 [禁用所有交互式提示]
            (bzr) Bazaar (bzr) command                           //版本控制工具Bazaa
            (git) Git command                                    //版本控制工具Git
            (cvs) CVS command                                    //版本控制工具CVS
            (cp) Local files retrieval command                   //本地文件拷贝命令
            (scp) Secure copy (scp) command                      //基于ssh的安全的远程文件拷贝命令
            (hg) Mercurial (hg) command                          //版本控制工具hg
            (gzip -d -c) zcat command                            //zip包解压缩查看 [解压zip文件 解压到指定路径，源文件不消失]
            (bzcat) bzcat command                                //bz2包解压缩查看
            (xzcat) xzcat command                                //xz包解压缩查看
            (lzip -d -c) lzcat command                           //lz包解压缩查看
            ()  Tar options                                      //bz2包解压缩查看
    (/home/hceng/imx6ul_buildroot/configs/imx6ulevk_defconfig) Location to save buildroot config  //指定配置文件保存路径
    ($(TOPDIR)/dl) Download dir                                  //指定文件下载保存路径  [./dl/]
    ($(BASE_DIR)/host) Host dir                                  //指定主机编译所需工具安装目录 [./output/host]
        Mirrors and Download locations  --->                     //镜像和下载位置
            ()  Primary download site
            (http://sources.buildroot.net) Backup download site
            (https://cdn.kernel.org/pub) Kernel.org mirror
            (http://ftpmirror.gnu.org) GNU Software mirror
            (http://rocks.moonscript.org) LuaRocks mirror
            (http://cpan.metacpan.org) CPAN mirror (Perl packages)
    (0) Number of jobs to run simultaneously (0 for auto)         //指定编译时运行的CPU核心数 [0自动]
    [ ] Enable compiler cache                                     //使能编译器缓存
    [ ] build packages with debugging symbols                     //启用带调试编译软件包
    [*] strip target binaries                         //binaries和libraries在打包到target目录的时候会被strip命令裁减掉调试信息
    ()    executables that should not be stripped                 //剥离时跳过可执行文件
    ()    directories that should be skipped when stripping       //剥离时跳过的目录
        gcc optimization level (optimize for size)  --->          //GCC优化等级 [优化大小]
    [ ] Enable google-breakpad support                            //启动崩溃日志收集
        libraries (shared only)  --->                             //库类型 [只共享库]
    ($(CONFIG_DIR)/local.mk) location of a package override file  //包覆盖文件的位置
    ()  global patch directories                                  //全局补丁目录
        Advanced  --->
            [*] paranoid check of library/header paths            //检查库/头文件路径
            [ ] Force the building of host dependencies           //强制构建主机依赖
            [ ] Make the build reproducible (experimental)        //构建可重复(实验)
        *** Security Hardening Options ***                        //安全加固选项
        Stack Smashing Protection (None)  --->                    //堆栈粉碎保护 [无]
        RELRO Protection (None)  --->                             //RELRO只读重定位(Relocation Read Only)保护 [无]
        Buffer-overflow Detection (FORTIFY_SOURCE) (None)  --->   //缓冲区溢出检测(强制源) [无]
```

### 2.3 Toolchain(工具链)

```bash
Toolchain  --->   
        Toolchain type (External toolchain)  --->                 //工具链类型 [外部工具链]
        *** Toolchain External Options ***                        //外部工具链选项
        Toolchain (Custom toolchain)  --->                        //工具链 [自定义工具链]
        Toolchain origin (Toolchain to be downloaded and installed)  --->  //工具链来源 [工具链将被下载安装]
    (https://releases.linaro.org/……) Toolchain URL                //工具链下载链接 ①
    (bin) Toolchain relative binary path                          //工具链二进制文件相对路径 [bin目录]
    ($(ARCH)-linux-gnueabihf) Toolchain prefix                    //工具链前缀 [arm-linux-gnueabihf]
        External toolchain gcc version (6.x)  --->                //外部工具链GCC版本 [6.x]
        External toolchain kernel headers series (4.6.x)  --->    //外部工具链内核头文件系列 [4.6.x]
        External toolchain C library (glibc/eglibc)  --->         //外部工具链C库 [glibc/eglibc] ②
    [*] Toolchain has SSP support?                                //工具链是否支持SSP? ③
    [*] Toolchain has RPC support?                                //工具链是否支持RPC? ④
    [*] Toolchain has C++ support?                                //工具链是否支持C++?
    [*] Toolchain has Fortran support?                            //工具链是否支持Fortran? (一种编程语言)
    ()  Extra toolchain libraries to be copied to target          //复制额外工具链库到目标
    [ ] Copy gdb server to the Target                             //复制GDB服务到目标
        *** Host GDB Options ***                                  //主机GDB选项
    [ ] Build cross gdb for the host                              //为主机交叉编译GDB
        *** Toolchain Generic Options ***                         //工具链通用选项
    [ ] Copy gconv libraries                                      //复制gconv库 (gconv库用于在不同的字符集之间进行转换)
    [ ] Enable WCHAR support                                // python等语言需要开启此项
    [*] Enable MMU support                                        //使能MMU支持
    ()  Target Optimizations                                      //目标优化 (需设置前面的GCC优化等级)
    ()  Target linker options                                     //目标链接器选项 (构建目标时传递给链接器的额外选项)
    [ ] Register toolchain within Eclipse Buildroot plug-in       //在Eclipse Buildroot插件中注册工具链
```

### 2.4 System configuration(系统配置)

```bash
System configuration  --->  系统配置
        Root FS skeleton (default target skeleton)  --->          //根文件系统框架 [典型目标框架]
    (ebf6ull) System hostname                                     //系统主机名字(自取任意) [ebf6ull]
    (Welcome to ixm6ull Buildroot!) System banner                 //系统开机提示 [Welcome to ixm6ull Buildroot!]
        Passwords encoding (sha-256)  --->                        //密码编码 [sha-256]
        Init system (systemV)  --->                               //初始化系统方案 [systemV]  ①
        /dev management (Dynamic using devtmpfs + eudev)  --->    //dev管理方案 [Dynamic using devtmpfs + eudev]  ②
    (system/device_table.txt) Path to the permission tables       //权限表路径 
    [ ] support extended attributes in device tables              //支持设备表中的扩展属性
    [ ] Use symlinks to /usr for /bin, /sbin and /lib             //是否将/bin,/sbin,/lib链接到/usr
    [*] Enable root login with password                           //使能root登陆密码
    ()    Root password                                           //设置root密码
        /bin/sh (bash)  --->                                      //选择shell类型 [bash] ③
    [*] Run a getty (login prompt) after boot  --->               //启动后运行getty(登录提示)--->
        (ttymxc0) TTY port                                        //设置TTY硬件端口
              Baudrate (keep kernel default)  --->                //比特率 [与内核保持一致]
        (vt100) TERM environment variable                         //TERM环境变量 (终端类型xterm、vt100)
        ()    other options to pass to getty                      //传递给getty的其他选项
    [*] remount root filesystem read-write during boot            //在引导期间安装根文件系统支持读和写
    (eth0) Network interface to configure through DHCP            //设置DHCP配置的网络接口  [eth0]
    (/bin:/sbin:/usr/bin:/usr/sbin) Set the system's default PATH //设置系统的默认路径  
    [*] Purge unwanted locales                                    //清除不需要的区域设置
    (C en_US) Locales to keep                                     //要保留的语言环境
    ()  Generate locale data                                      //生成区域设置数据
    [ ] Enable Native Language Support (NLS)                      //启用本地语言支持（NLS）
    -*- Install timezone info                                     //安装时区信息
    (default) timezone list                                       //时区清单 [典型]
    (Etc/UTC) default local time                                  //用户表的路径
    ()  Path to the users tables
    (board/hceng/nxp-imx6ull/rootfs-overlay) Root filesystem overlay directories //根文件系统覆盖目录
    ()  Custom scripts to run before creating filesystem images //在创建文件系统映像之前运行的自定义脚本
    ()  Custom scripts to run inside the fakeroot environment //自定义脚本在fakeroot(模拟root权限)环境中运行
    (board/……) Custom scripts to run after creating filesystem images //创建文件系统映像后运行的自定义脚本 ④
    ()    Extra arguments passed to custom scripts //传递给自定义脚本的额外参数
```

①：可选选项有`BusyBox`、`systemV`、`systemd`、`None`：

- `BusyBox init`:
   1.不支持运行等级，设置的运行等级将被忽略，要使用运行等级，请使用sysvinit;
   2.语法格式：`<id>:<runlevels>:<action>:<process>`
   `<id>`:process执行所在的tty设备，内容为/dev目录中tty设备的文件名;
   `<runlevels>`:此字段完全被忽略;
   `<action>`:支持sysinit、respawn、askfirst、wait、once、restart、ctrlaltdel、shutdown;
   `<process>`:指定要执行的进程及其命令行;
   3.BusyBox init程序将在启动时读取`/etc/inittab`文件，以了解该做什么，默认inittab存储在`./package/busybox/inittab`;
   4.inittab除了安装几个重要的文件系统之外，还要启动`/etc/init.d/rcS`中的shell脚本，并启动一个getty程序(提供一个登录提示);
- `systemV`:
   1.使用传统sysvinit程序，之前大多数台式机Linux发行版都使用该方案，现在有些变为了Upstart或Systemd;
   2.在/ect目录下会生成init.d、rc0.d、rc1.d、rc2.d、rc3.d、rc4.d、rc5.d、rc6.d、rc.loacl;
   init.d里面包含的是真正的服务脚本;
   rcN.d里面是链接向init.d里脚本的软链接，N表示运行级别，进入哪个运行级别，就会执行对应rcN.d文件夹的脚本;
   当所有的当前运行级别的脚本都运行完了之后，会运行rc.local;
   3.脚本的命名规则：以`[S|K]+NN+其它`，以S开头的是启动脚本，以K开头的是停止脚本，init进程会按照S或者K后面的数字的顺序来启动或停止服务;
   4.sysvinit还使用`/etc/inittab`文件(与BusyBox的语法略有不同)，默认inittab存储在`./package/sysvinit/inittab`;
- `systemd`:
   1.systemd是Linux的新一代init系统，以前的运行级别(runlevel)的概念被新的运行目标(target)所取代;
   2.支持并行化任务;采用socket式与D-Bus总线式激活服务;按需启动守护进程(daemon);支持快照和系统恢复;
   3.功能强大的同时，也带来了相当大数量的大型依赖：dbus，udev等;
   [Systemd 的简介和特点](https://links.jianshu.com/go?to=https%3A%2F%2Fwww.ibm.com%2Fdeveloperworks%2Fcn%2Flinux%2F1407_liuming_init3%2Findex.html)

②：/dev设备文件的管理方式，可选选项有四个：

- `Static using device table`: 使用静态的设备表，/dev将根据`system/device_table _dev.txt`的内容创建设备，进入系统添加或删除设备时，无法自动更新；
- `Dynamic using devtmpfs only`:在系统启动过程中，会动态生成/dev文件，进入系统添加或删除设备时，无法自动更新；
- `Dynamic using devtmpfs + mdev`:在前面devtmpfs的基础上加入mdev用户空间实用程序，进入系统添加或删除设备时，可以自动更新，自动创建规则在`/etc/mdev.conf`;
- `Dynamic using devtmpfs + eudev`:在前面devtmpfs的基础上加入eudev用户空间守护程序，eudev是udev的独立版本，是Systemd的一部分，提供更多的功能也更占用资源；

③：在Linux下编写shell脚本文件时，经常会看到在第一行中标注`#!/bin/bash`，这句话的意思是告诉系统强制用bash，避免出现一些不兼容的问题。因此，除了bash外，还有很多shell工具，比如这里可选busybox自带的shell、小巧但功能很少的dash、高效紧凑的mksh、功能强大体积也稍大的zsh。此外，可以通过`ls -l /bin/sh`查看当前使用的是何种shell工具。

④：受限每行字数，该处完整路径为`board/freescale/common/imx/post-image.sh`。

这里是**如何产生sdcard.img，用于sd卡启动的原理部分**。
 针对我现在imx6ull的情况，`board/freescale/common/imx`目录下有两个文件值得关注：`genimage.cfg.template`和`post-image.sh`。
 先来看`genimage.cfg.template`：

```bash
# Minimal SD card image for the Freescale boards Template
#
# We mimic the .sdcard Freescale's image format:
# * the SD card must have 1 kB free space at the beginning,
# * U-Boot is dumped as is,
# * a FAT partition at offset 8 MB is containing zImage/uImage and DTB files
# * a single root filesystem partition is required (ext2, ext3 or ext4)
#
image boot.vfat {
  vfat {
    files = {
      %FILES%
    }
  }
  size = 16M
}
image sdcard.img {
  hdimage {
  }
  partition u-boot {
    in-partition-table = "no"
    image = "%UBOOTBIN%"
    offset = 1024
  }
  partition boot {
    partition-type = 0xC
    bootable = "true"
    image = "boot.vfat"
    offset = 8M
  }
  partition rootfs {
    partition-type = 0x83
    image = "rootfs.ext2"
  }
}
```

该配置文件显示会生成两个文件，一个`boot.vfat`，一个`sdcard.img`；
 `boot.vfat`由`"%FILES%"`所表示内容组成(后面会得知是kernel+dtb);
 `sdcard.img`有四个分区，第一个是空，第二个是偏移1024字节(1k)后，内容为`"%UBOOTBIN%"`(u-boot)，第三个为偏移8M后，存放前面生成的`boot.vfat`(kernel+dtb)，最后存放`rootfs.ext2`。
 此时分区情况如下:

![img](https://upload-images.jianshu.io/upload_images/13094704-adf6cc0f6dbcf422.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

分区类型和数值的对应关系可通过该文章查询:[List of partition identifiers for PCs](https://links.jianshu.com/go?to=https%3A%2F%2Fwww.win.tue.nl%2F~aeb%2Fpartitions%2Fpartition_types-1.html) or [Listing of MBR/EBR Partition Types](https://links.jianshu.com/go?to=https%3A%2F%2Fthestarman.pcministry.com%2Fasm%2Fmbr%2FPartTypes.htm)
 此时将sd卡插入Windows电脑，可以发现只能识别存放`boot.vfa`t(kernel+dtb)的分区，因为该分区为FAT32格式，Windows可以识别，而存放`rootfs.ext2`的分区为ext2/3/4，Windows是无法识别的，与生活常识是吻合的。
 另外，如果想在SD卡创建其它自定义分区，可以再加一个partition：

```bash
partition user {
  partition-type = 0xC
  size = 10M
}
```

再来看看post-image.sh是如何解析genimage.cfg.template:

```bash
#!/usr/bin/env bash
#
# dtb_list extracts the list of DTB files from BR2_LINUX_KERNEL_INTREE_DTS_NAME
# in ${BR_CONFIG}, then prints the corresponding list of file names for the
# genimage configuration file
#
dtb_list()
{
    local DTB_LIST="$(sed -n 's/^BR2_LINUX_KERNEL_INTREE_DTS_NAME="\([\/a-z0-9 \-]*\)"$/\1/p' ${BR2_CONFIG})"
    for dt in $DTB_LIST; do
        echo -n "\"`basename $dt`.dtb\", "
    done
}
#
# linux_image extracts the Linux image format from BR2_LINUX_KERNEL_UIMAGE in
# ${BR_CONFIG}, then prints the corresponding file name for the genimage
# configuration file
#
linux_image()
{
    if grep -Eq "^BR2_LINUX_KERNEL_UIMAGE=y$" ${BR2_CONFIG}; then
        echo "\"uImage\""
    elif grep -Eq "^BR2_LINUX_KERNEL_IMAGE=y$" ${BR2_CONFIG}; then
        echo "\"Image\""
    else
        echo "\"zImage\""
    fi
}
genimage_type()
{
    if grep -Eq "^BR2_PACKAGE_FREESCALE_IMX_PLATFORM_IMX8M=y$" ${BR2_CONFIG}; then
        echo "genimage.cfg.template_imx8"
    elif grep -Eq "^BR2_TARGET_UBOOT_SPL=y$" ${BR2_CONFIG}; then
        echo "genimage.cfg.template_spl"
    else
        echo "genimage.cfg.template"
    fi
}
uboot_image()
{
    if grep -Eq "^BR2_TARGET_UBOOT_FORMAT_DTB_IMX=y$" ${BR2_CONFIG}; then
        echo "u-boot-dtb.imx"
    elif grep -Eq "^BR2_TARGET_UBOOT_FORMAT_IMX=y$" ${BR2_CONFIG}; then
        echo "u-boot.imx"
    fi
}
main()
{
    local FILES="$(dtb_list) $(linux_image)"
    local UBOOTBIN="$(uboot_image)"
    local GENIMAGE_CFG="$(mktemp --suffix genimage.cfg)"
    local GENIMAGE_TMP="${BUILD_DIR}/genimage.tmp"
    sed -e "s/%FILES%/${FILES}/" \
        -e "s/%UBOOTBIN%/${UBOOTBIN}/" \
        board/freescale/common/imx/$(genimage_type) > ${GENIMAGE_CFG}
    rm -rf "${GENIMAGE_TMP}"
    genimage \
        --rootpath "${TARGET_DIR}" \
        --tmppath "${GENIMAGE_TMP}" \
        --inputpath "${BINARIES_DIR}" \
        --outputpath "${BINARIES_DIR}" \
        --config "${GENIMAGE_CFG}"
    rm -f ${GENIMAGE_CFG}
    exit $?
}
main $@
```

可以在main看到，FILES为dtb和kernel，UBOOTBIN为u-boot，再传入配置文件。
 最后使用genimage生成，genimage在后面2.9Host utilities(主机工具)部分需要勾选上，它的作用是给定根文件系统树，生成多个文件系统和闪存镜像的工具

### 2.5 Kernel(内核配置)

```bash
[*] Linux Kernel                                                              //使能编译内核
      *** Linux kernel in thumb mode may be broken with binutils >= 2.29 ***  //binutils>=2.29可能会破坏thumb模式下的内核
      Kernel version (Custom Git repository)  --->                            //内核版本 [自定义Git仓库]
(https://git.dev.tencent.com/……) URL of custom repository                     //自定义仓库网址 ①
(origin/master) Custom repository version                                     //自定义仓库版本
()    Custom kernel patches                                                   //自定义内核补丁
      Kernel configuration (Using an in-tree defconfig file)  --->            //内核配置 [使用项目内defconfig文件]
(imx_v7_ebf) Defconfig name                                                   //配置文件名字 (不要_defconfig)
()    Additional configuration fragment files                                 //其他配置片段文件
()    Custom boot logo file path                                              //自定义启动logo文件路径
      Kernel binary format (zImage)  --->                                     //内核二进制文件格式 [zImage] ②
      Kernel compression format (gzip compression)  --->                      //内核压缩格式 [gzip压缩]
[*]   Build a Device Tree Blob (DTB)                                          //构建设备树二进制文件(DTB文件)
(imx6ull-14x14-evk imx6ull-14x14-ebf) In-tree Device Tree Source file names   //项目里设备树源文件名 (不要.dts)
()      Out-of-tree Device Tree Source file paths                             //项目外设备树源文件路径
[ ]     Build Device Tree with overlay support                                //使能设备树文件覆盖支持
[*]   Install kernel image to /boot in target                                 //安装内核镜像到/boot目录
[*]   Needs host OpenSSL                                                      //主机需要OpenSSL
[ ]   Needs host libelf                                                       //主机需要libelf (用于读取，修改或创建ELF文件)
      Linux Kernel Extensions  --->                                           //Linux内核扩展
        [ ] Adeos/Xenomai Real-time patch                                     //Adeos/Xenomai实时时钟补丁
        [ ] RTAI Real-time patch                                              //RTAI实时时钟补丁
        [ ] ev3dev Linux drivers                                              //ev3dev Linux驱动
        [ ] FB TFT drivers                                                    //FB TFT驱动
        [ ] Aufs Filesystem Module patch                                      //Aufs文件系统模块补丁
      Linux Kernel Tools  --->                                                //Linux内核工具
        [ ] cpupower                                                          //用于检查、调整CPU省电相关功能
        [ ] gpio                                                              //用于控制、监控GPIO,仅在4.8版本提供 ③
        [ ] iio                                                               //用于控制、监控iio设备,仅在4.7版本提供
        [ ] pci                                                               //用于测试特定PCI端点, 仅在4.20版本提供
        [ ] perf                                                              //用于Linux性能分析
        [ ] selftests                                                         //用于内核自我测试
        [ ] tmon       
```

①：受限每行字数，该处完整链接为`https://git.dev.tencent.com/weidongshan/imx6ull_kernel.git`；
 ②：分别介绍下这几个内核镜像格式，以及一些其它格式:

- `vmlinuz·`:静态编译出来的最原始的ELF文件，包括了内核镜像、调试信息、符号表等内容；其中 “vm” 代表 “Virtual Memory”;
- `Image`:将所有的符号和重定位信息都删除，只剩下二进制数据的内核代码，此时还没经过压缩;
- `zImage`:是vmlinux加上解压代码(用于自解压)经过gzip压缩后的文件，适用于小内核，常见于ARM;
- `bzImage`:是vmlinux加上解压代码(用于自解压)经过gzip压缩后的文件，适用于大内核，常见于x86，“bz”表示 “big zImage”;
- `uImage`:是U-Boot专用的镜像文件，使用mkimage工具在zImage之前加上一个长度为0x40的头信息(tag)，在头信息内说明了该镜像文件的类型、加载位置、生成时间、大小等信息;

参考资料：[linux内核镜像格式](https://links.jianshu.com/go?to=https%3A%2F%2Fwww.cnblogs.com%2Fhnrainll%2Farchive%2F2011%2F06%2F10%2F2077961.html)

③：使用新的ABI，弃用sysfs；

### 2.6 Target packages(目标包配置)

```bash
Target packages  --->   
    [*] BusyBox                                                          //使能编译BusyBox
    (package/busybox/busybox.config) BusyBox configuration file to use?  //设置BusyBox配置文件路径
    ()    Additional BusyBox configuration fragment files                //其他BusyBox配置片段文件
    -*-   Show packages that are also provided by busybox                //列出部分busybox也提供的包
    [ ]   Enable SELinux support                                         //SELinux支持(安全增强型Security-Enhanced Linux)
    [ ]   Individual binaries                                            //每个应用程序作为单独二进制文件(为SELinux提供支持)
    [ ]   Install the watchdog daemon startup script                     //在启动脚本安装看门狗守护程序
        Audio and video applications  --->                               //音频和视频应用
        Compressors and decompressors  --->                              //压缩和解压
        Debugging, profiling and benchmark  --->                         //调试、分析和基准测试
        Development tools  --->                                          //开发工具
        Filesystem and flash utilities  --->                             //文件系统和闪存实用程序
        Fonts, cursors, icons, sounds and themes  --->                   //字体，游标，图标，声音和主题
        Games  --->                                                      //游戏
        Graphic libraries and applications (graphic/text)  --->          //图形库和应用程序(图形/文本)
        Hardware handling  --->                                          //硬件处理
        Interpreter languages and scripting  --->                        //编程语言和脚本
        Libraries  --->                                                  //库
        Mail  --->                                                       //邮箱
        Miscellaneous  --->                                              //杂项
        Networking applications  --->                                    //网络应用
        Package managers  --->                                           //安装包管理
        Real-Time  --->                                                  //实时时钟
        Security  --->                                                   //安全
        Shell and utilities  --->                                        //Shell和程序
        System tools  --->                                               //系统工具
        Text editors and viewers  --->                                   //文版编辑和浏览
```

#### 2.6.1 Audio and video applications

```bash
Audio and video applications  --->   
    [*] alsa-utils  --->                       //ALSA声卡测试和音频编辑
    [ ] atest                                  //ALSA Asoc驱动测试工具
    [ ] aumix                                  //声卡混音器
    [ ] bellagio                               //基于OpenMAX的多媒体访问
    [ ] bluez-alsa                             //蓝牙音频ALSA后端
    [ ] dvblast                                //MPEG-2/TS解复用和流媒体
    [ ] dvdauthor                              //创作DVD视频光盘文件和目录结构
    [ ] dvdrw-tools                            //创作蓝光光盘和DVD光盘媒体
    [ ] espeak                                 //用于英语和其他语言的语音合成器软件
    [ ] faad2                                  //开源的MPEG-4和MPEG-2 AAC解码器
    [*] ffmpeg  --->                           //录制、转换以及流化音视频的完整解决方案
    [ ] flac                                   //开源无损音频编解码器
    [ ] flite                                  //小型、快速的TTS系统(TextToSpeech),即文字转语音
    [ ] gmrender-resurrect                     //基于gstreamer的UPnP(DLNA)媒体渲染器
    [ ] gstreamer 0.10                         //开源多媒体框架0.10版本
    [ ] gstreamer 1.x                          //开源多媒体框架1.x版本,与前面的0.10不兼容
    [ ] jack2                                  //JACK音频连接套件(包含服务器和示例客户端)
        *** kodi needs python w/ .py modules,  //Kodi开源媒体播放器软件应用程序的需要条件 
            a uClibc or glibc toolchain w/ C++, threads, wchar, dynamic library, gcc >= 4.8, ***
        *** kodi needs an OpenGL EGL backend with OpenGL support *** 
    [ ] lame                                   //高质量的MPEG Audio Layer III(MP3)编码器
    [ ] madplay                                //libmad的命令行前端,一个高质量的MPEG音频解码器
    [ ] mimic                                  //快速，轻量级的文本到语音引擎
        *** miraclecast needs systemd and      //miraclecast无线屏幕投影的需要条件
            a glibc toolchain w/ threads and wchar ***  
    [ ] mjpegtools                             //录制视频和回放,简单的剪切和粘贴编辑以及音频和视频的MPEG压缩
    [ ] modplugtools                           //支持播放MOD、S3M、XM等格式音乐文件
    [ ] motion                                 //监控摄像机视频信号的程序,可以检测物体运动
    [*] mpd  --->                              //用于播放音乐的服务器端应用程序
    [ ] mpd-mpc                                //MPD的简约命令行界面
    [ ] mpg123                                 //MPEG音频播放器
    [ ] mpv                                    //MPlayer的一个分支,有些新功能
    [ ] multicat                               //高效地操作多播流,特别是MPEG-2传输流
    [ ] musepack                               //音频高品质压缩
    [ ] ncmpc                                  //功能齐全的MPD客户端
    [ ] opus-tools                             //Opus编解码器命令行工具
    [ ] pulseaudio                             //声音服务代理,可队声音进行操作再播放
    [ ] sox                                    //可录制、播放、格式转换、修改音频文件
    [ ] squeezelite                            //Logitech媒体服务器客户端
    [ ] tstools                                //处理MPEG数据的跨平台命令行工具
    [ ] twolame                                //优化的MPEG Audio Layer 2(MP2)编码器
    [ ] udpxy                                  //将UDP流量转发到请求HTTP客户端的中继守护程序
    [ ] upmpdcli                               //MPD音乐播放器前端
    [ ] v4l2grab                               //用于从V4L2设备获取JPEG的程序
    [ ] v4l2loopback                           //创建虚拟视频设备
    [ ] vlc                                    //可播放大多数多媒体文件以及DVD、音频CD、VCD和各种流媒体协议
    [ ] vorbis-tools                           //用于Ogg格式文件的独立播放器、编码器和解码器,也可作为流媒体播放器
    [ ] wavpack                                //提供无损，高质量的有损和独特的混合压缩模式
    [ ] yavta                                  //一个V4L2测试应用程序
    [ ] ympd                                   //MPD网页客户端
```

#### 2.6.2 Compressors and decompressors

```bash
Compressors and decompressors  --->   
    [ ] brotli                                 //通用无损压缩库
    -*- bzip2                                  //免费的压缩工具
    [ ] gzip                                   //标准GNU压缩器
    [ ] lz4                                    //快速的无损压缩
    [ ] lzip                                   //类似gzip或bzip2
    [ ] lzop                                   //与gzip相似
    [ ] p7zip                                  //Unix的7-zip命令行版本
    [ ] pigz                                   //是gzip的全功能替代品
    [ ] pixz                                   //是xz的并行索引版本
    [ ] unrar                                  //rar文件解压
    [ ] unzip                                  //zip文件解压
    [*] xz-utils                               //用于处理XZ压缩的命令行工具,包括xz、unxz、xzcat、xzgrep等
    [ ] zip                                    //压缩和解压zip文件
    [ ] zstd                                   //Zstandard或zstd的简短版本
```

#### 2.6.3 Debugging, profiling and benchmark

```bash
 ] blktrace                                   //对通用块层(block layer)的I/O跟踪机制，它能抓取详细的I/O请求，发送到用户空间  
[ ] bonnie++                                   //执行一系列简单的硬盘驱动器和文件系统性能测试   
[ ] cache-calibrator                           //用于分析计算机(缓存)内存系统并提取有用信息，以及作为负载生成器进行实时测试   
    *** clinfo needs an OpenCL provider ***    //查询OpenCL参数和是否安装正常   
[ ] dhrystone                                  //测量处理器运算能力的最常见基准程序之一，常用于处理器的整型运算性能的测量   
[ ] dieharder                                  //随机数/均匀偏差发生器测试仪，适用于测试软件RNG和硬件RNG   
[ ] dmalloc                                    //一个调试分配内存库，替代系统的malloc、realloc、calloc、free等   
[ ] dropwatch                                  //交互式监视和记录内核丢弃的数据包   
[ ] dstat                                      //取代vmstat、iostat、netstat、ifstat等，监控系统运行状况、基准测试、排除故障   
[ ] dt                                         //用于验证外围设备、文件系统、驱动程序或操作系统支持的任何数据流的正常运行   
[ ] duma                                       //检测意外的内存访问(Detect Unintended Memory Access) 
[ ] fio                                        //一种I/O工具，用于基准测试和压力/硬件验证   
[ ] gdb                                        //强大的UNIX下的程序调试工具   
[ ] google-breakpad                            //用于崩溃日志收集   
[ ] iozone                                     //一个文件系统基准测试工具，测试不同的操作系统中文件系统的读写性能   
[ ] kexec                                      //用一个运行的内核去运行一个新内核，就像运行一个应用程序一样   
[ ] ktap                                       //基于脚本的Linux动态跟踪工具，允许用户跟踪Linux内核动态   
[ ] kvm-unit-tests                             //为KVM提供单元测试   
[ ] latencytop                                 //专注于解决音频跳跃、桌面卡顿、服务器过载等延迟   
[ ] lmbench                                    //一种性能检测工具，提供内存、网络、内核等多方面的测试   
[ ] lsof                                       //列出当前系统打开文件的工具(LiSt Open Files)   
[ ] ltp-testsuite                              //测试Linux内核和相关特性的工具集合   
[ ] ltrace                                     //能够跟踪进程的库函数调用,显示哪个库函数被调用   
[ ] lttng-babeltrace                           //LTTng(Linux Trace Toolkit Next Generation)跟踪读写库、转换
[ ] lttng-modules                              //用于LTTng 2.x内核跟踪基础结构的Linux内核模块   
[ ] lttng-tools                                //用于LTTng 2.x跟踪基础结构的用户空间实用程序   
[ ] memstat                                    //列出正在消耗虚拟内存的所有进程、可执行程序、共享库   
[ ] netperf                                    //网络性能基准工具   
[ ] netsniff-ng                                //高性能的Linux网络分析器和网络工具包   
[ ] nmon                                       //监控系统的 CPU、内存、网络、硬盘、文件系统、NFS、高耗进程、资源等信息   
[ ] oprofile                                   //性能监测工具，从代码层面分析程序的性能消耗情况，找出程序性能的问题点   
[ ] pax-utils                                  //用于ELF 32/64二进制文件的ELF相关工具，可以检查文件的安全相关属性   
[ ] pv                                         //基于终端的工具，用于监控通过管道的数据进度   
[ ] ramspeed/smp                               //用于测量多处理器计算机的缓存和内存性能   
[ ] ramspeed                                   //用于测量缓存和内存性能   
[ ] rt-tests                                   //用于测试Linux系统实时行为的程序集   
[ ] spidev_test                                //基于spidev驱动程序的SPI测试程序 
[ ] strace                                     //用于诊断、调试和教学的Linux用户空间跟踪器   
[ ] stress                                     //用于POSIX系统的工作负载生成器   
[ ] stress-ng                                  //以各种可选择的方式对计算机系统进行压力测试   
[ ] sysdig                                     //从正在运行的Linux实例捕获系统状态和活动、然后保存、过滤和分析   
[ ] tcf-agent                                  //一个守护进程，它提供可供本地和远程客户端使用的TCF服务   
[ ] tinymembench                               //内存基准测试程序   
[ ] trace-cmd                                  //帮助开发人员了解Linux内核的运行时行为，以便进行故障调试或性能分析   
[ ] trinity                                    //Linux系统调用模糊测试 
[ ] uclibc-ng-test                             //编译并安装uClibc-ng测试套件   
[ ] valgrind                                   //用于调试和分析Linux程序的工具   
[ ] vmtouch                                    //用于学习和控制unix和unix类系统的文件系统缓存的工具   
[ ] whetstone                                  //测试双精度浮点数操作的速度和效率
```

#### 2.6.4 Development tools

```bash
[ ] binutils                                   //安装binutils(BinaryUtilities)二进制工具的集合，比如ld、as                                                                                                     
[ ] bsdiff                                     //创建补丁path或文件比较diff                                                                                                     
[ ] check                                      //单元测试框架                                                                                                     
[ ] ctest                                      //CTest是CMake集成的一个测试工具，可以自动配置、构建、测试、展现测试结果                                                                                                     
[ ] cppunit                                    //著名的JUnit框架的C++端口，用于单元测试                                                                                                     
[ ] cunit                                      //自动化测试框架                                                                                                     
[ ] cvs                                        //代码版本控制软件                                                                                                     
[ ] cxxtest                                    //C++的单元测试框架                                                                                                     
[ ] diffutils                                  //GNU diff，文件每行比较                                                                                                     
[ ] dos2unix                                   //将DOS格式(CRLF)的文本文件转换成UNIX格式(LF)                                                                                                     
[ ] findutils                                  //基本目录搜索工具，比如find、xargs、locate、updatedb等                                                                                                     
[ ] flex                                       //快速词法分析器生成器，用于生成在文本上执行模式匹配的程序的工具                                                                                                      
[ ] gawk                                       //一种特殊用途的编程语言，它是数据驱动的而不是程序的                                                                                                     
[ ] gettext                                    //提供一个框架来帮助其他GNU包生成多语言消息                                                                                                     
[ ] git                                        //代码版本控制软件                                                                                                     
[ ] git-crypt                                  //用于在git存储库中对文件进行透明加密和解密                                                                                                     
[ ] gperf                                      //一个“完美的哈希函数”生成器                                                                                                     
[ ] grep                                       //GNU正则表达式匹配器                                                                                                     
[ ] jo                                         //从shell输出JSON的命令行处理器                                                                                                     
[ ] jq                                         //类似用于JSON数据的sed，用来切片和过滤，映射和转换结构化数据                                                                                                     
[ ] libtool                                    //一个通用的库支持脚本，藏了在一致的可移植界面背后使用共享库的复杂性                                                                                                     
[ ] make                                       //用于控制程序源文件中程序的可执行文件和其他非源文件的生成                                                                                                     
[ ] patch                                      //获取补丁文件(包含差异列表)并将其应用于原始文件，生成修补版本                                                                                                     
[ ] pkgconf                                    //有助于为开发框架配置编译器和链接器标志的程序                                                                                                     
[ ] sed                                        //非交互式命令行文本编辑器                                                                                                     
[ ] subversion                                 //代码版本控制软件                                                                                                     
[ ] tree                                       //递归显示目录列表的命令
```

#### 2.6.5 Filesystem and flash utilities

```bash
[ ] abootimg                                   //直接通过文件映像或/dev块设备操作Android Boot Images的工具
[ ] aufs-util                                  //aufs文件系统工具
[ ] autofs                                     //自动挂载/卸载文件系统的守护进程
[ ] btrfs-progs                                //Btrfs文件系统工具
[ ] cifs-utils                                 //CIFS文件系统工具
[ ] cpio                                       //用于创建和提取的cpio存档的工具
[ ] cramfs                                     //用于生成和检查cramfs文件系统的工具
[ ] curlftpfs (FUSE)                           //基于FUSE和libcurl访问FTP主机的文件系统
[ ] davfs2                                     //一个Linux文件系统驱动程序，允许挂载WebDAV资源，远程协作创作Web资源
[ ] dosfstools                                 //用于创建和检查DOS FAT文件系统的工具
[*] e2fsprogs  --->                            //Ext2(及Ext3/4)文件系统工具集，包含了创建、修复、配置、调试等工具
    [ ]   debugfs                              //ext2/ext3/ext4文件系统调试工具(不仅限于下列几个工具)
    [ ]   e2image                              //保存关键的ext2/ext3ext4文件系统的元数据到文件中
    [ ]   e4defrag                             //用于ext4文件系统的在线碎片整理程序
    [*]   fsck                                 //检查并修复特定Linux文件系统 
    [ ]   fuse2fs                              //用于ext2/ext3/ext4文件系统的FUSE文件系统客户端
    [ ]   resize2fs                            //用于ext2/ext3/ext4文件系统容量调整
[ ] e2tools                                    //用于读取、写入、操作ext2/ext3中的文件，使用ext2fs库访问文件系统
[ ] ecryptfs-utils                             //适用于Linux的POSIX兼容企业加密文件系统
[ ] exFAT (FUSE)                               //作为FUSE模块，GNU/Linux和其他类Unix系统的全功能exFAT文件系统
[ ] exfat-utils                                //exFAT文件系统工具
[ ] f2fs-tools                                 //用于Flash-Friendly File System(F2FS)的工具
[ ] flashbench                                 //用于识别SD卡和存储媒介属性的工具
[ ] fscryptctl                                 //处理原始密钥并管理Linux文件系统加密策略的工具
[ ] fwup                                       //可编写脚本的嵌入式Linux固件更新创建、运行工具
[ ] genext2fs                                  //作为普通(非root)用户生成ext2文件系统
[ ] genpart                                    //生成由命令行参数定义的16字节分区表条目，并将其转储到stdout
[ ] genromfs                                   //生成ROMFS文件系统的工具
[ ] imx-usb-loader                             //通过USB在Freescale i.MX5/i.MX6和Vybrid SoC上下载和运行程序的工具
[ ] mmc-utils                                  //MMC工具
[ ] mtd, jffs2 and ubi/ubifs tools             //构建mtd、jffs2和ubi/ubifs工具
[ ] mtools                                     //用于从Unix访问MS-DOS磁盘而不安装它们
[ ] nfs-utils                                  //NFS服务工具
[ ] nilfs-utils                                //用于创建和管理NILFS2文件系统的工具
[ ] ntfs-3g                                    //开源免费的读/写NTFS驱动程序，可以处理Windows的NTFS文件系统
[ ] sp-oops-extract                            //一个从MTD中提取OOPS/panic异常日志的工具
[ ] squashfs                                   //生成SquashFS文件系统的工具
[ ] sshfs (FUSE)                               //基于SSH文件传输协议的FUSE文件系统客户端
[ ] sunxi-tools                                //用于全志Allwinner A10(又名sun4i)和A13(又名sun5i)设备的工具
[ ] udftools                                   //用于创建UDF文件系统的工具
[ ] unionfs (FUSE)                             //一个用户空间unionfs的实现
[ ] xfsprogs                                   //XFS文件系统工具和库
```

### 2.6.6 Fonts, cursors, icons, sounds and themes

```csharp
  *** Cursors ***                              //光标                                  

[ ] comix-cursors                              //X11鼠标主题里一个略卡通的鼠标光标                                    

[ ] obsidian-cursors                           //一个明亮、干净的鼠标光标集合                                    

    *** Fonts ***                              //字体                                    

[ ] Bitstream Vera                             //Bitstream Vera字体系列                                    

[ ] cantarell                                  //一款当代人文主义无衬线字体，专为屏幕阅读而设计                                    

[ ] DejaVu fonts                               //基于Vera字体的字体系列，提供更广泛的用途                                    

[ ] font-awesome                               //是一套605个象形图标，可在网站上轻松扩展矢量图形                                    

[ ] ghostscript-fonts                          //随Ghostscript(PDF软件)一起分发的字体，目前包括35种的gostScript字体                                    

[ ] inconsolata                                //一种等宽字体，专为代码、清单等而设计                                    

[ ] Liberation (Free fonts)                    //旨在替代Microsoft三种最常用字体：Times New Roman、Arial和Courier New                                    

    *** Icons ***                              //图标                                    

[ ] google-material-design-icons               //Google根据材料设计语言(Material Design)设计的官方图标集                                    

[ ] hicolor icon theme                         //备用图标主题，用于显示图标主题中不可用的图标                                    

    *** Sounds ***                             //声音                                    

[ ] sound-theme-borealis                       //北极星(borealis)的声音主题                                    

[ ] sound-theme-freedesktop                    //默认桌面(freedesktop)的声音主题                                  

    *** Themes ***                             //主题
```

# buildroot训练

https://bootlin.com/doc/training/buildroot/buildroot-slides.pdf

## 什么是buildroot

Buildroot是Linux平台上一个开源的嵌入式Linux系统**自动构建框架**。

## 0.下载buildroot

Buildroot版本每2个月，2月，5月，8月和11月发布一次。版本号的格式为YYYY.MM，例如2013.02、2014.08。 可以从http://buildroot.org/downloads/获得发行包。 也可通过github仓库获取： `git clone git://git.busybox.net/buildroot ` buildroot目录结构 解压buildroot源码目录如下：  arch: CPU架构相关的配置脚本  board: 在构建系统时，board默认的boot和Linux kernel配置文件，以及一些板级相关脚本  boot: uboot配置脚本目录  configs: 板级配置文件，该目录下的配置文件记录着该机器平台或者方案使用的工具链，boot， kernel，各种应用软件包的配置  dl: download的简写，下载一些开源包。第一次下载后，下次就不会再去从官网下载了，而是从dl/目录下拿开源包，以节约时间  docs:  fs: 各种文件系统的自动构建脚本  linux: 存放Linux kernel的自动构建脚本  package: 第三方开源包的自动编译构建脚本，用来配置编译dl目录下载的开源包  support:  system: 存放文件系统目录的和设备节点的模板，这些模板会被拷贝到output/目录下，用于制作根文件系统rootfs  toolchain/ 目录中存放着各种制作工具链的脚本  编译出的output输出目录介绍：  images/存储所有映像（内核映像，引导加载程序和根文件系统映像）的位置。这些是您需要放在目标系统上的文件。  build/构建所有组件的位置（包括主机上Buildroot所需的工具和针对目标编译的软件包）。该目录为每个组件包含一个子目录。  host/包含为主机构建的工具和目标工具链。  staging/是到内部目标工具链host/的符号链接  target/它几乎包含了目标的完整根文件系统。除了设备文件/dev/（Buildroot无法创建它们，因为Buildroot不能以root身份运行并且不想以root身份运行）之外，所需的一切都存在。  工具链 Buildroot为交叉编译工具链提供了两种解决方案： 内部工具链，称为Buildroot toolchain在配置接口。 外部工具链External toolchain。
 我们这里选择的是外部工具链,工具链的来源可以选择下载也可以选择指定工具链前缀如：/opt/ali/ali-linux/Codescape.GNU.Tools.Package.2017.10-05.for.MIPS.MTI.Linux.CentOS-5.x86_64/usr/bin/mips-mti-linux-gnu-，我这里选择的下载工具链源码。下载完后，同时会对用来编译工具链的原始工具链解压到(BUILD_ROOT)/host/opt/ext-toolchain/目录下，然后会对工具链源码编译安装到$(BUILD_ROOT)/host/usr/bin下，这样我们的工具链就可以使用了。
 ![img](https://img2018.cnblogs.com/blog/1876680/201912/1876680-20191218164156040-902147356.png)
 ![img](https://img2018.cnblogs.com/blog/1876680/201912/1876680-20191218164201190-701859949.png)
 ![img](https://img2018.cnblogs.com/blog/1876680/201912/1876680-20191218164221202-237674422.png)
 ![img](https://img2018.cnblogs.com/blog/1876680/201912/1876680-20191218164231749-690612066.png) 1.buildroot常用make命令 *make help: 可以看到buildroot下make的使用细节，包括对package、uclibc、busybox、linux以及文档生成等配置。  make menuconfig(make linux-menuconfig...):进行图形化配置  make：编译  make命令通常将执行以下步骤： 下载源文件（根据需要）； 配置、构建和安装交叉编译工具链，或仅导入外部工具链； 配置、构建和安装选定的目标软件包； 构建内核映像（如果选择）； 构建引导加载程序映像（如果选择）； 以选定的格式创建一个根文件系统  make clean：delete all build products (including build directories, host, staging and target trees, the images and the toolchain)  make distclean: 等于make clean+删除配置  make show-targets:显示出本次配置所要编译所有的目标  make  pkg-target:单独编译某个pkg模块  make pkg-rebuild:重新编译pkg  make  pkg-extract:只下载解压pkg,不编译，pkg解压后放在 output/build/对应的pkg-dir目录下  make pkg-source:只下载某pkg，然后不做任何事情  2.buildroot框架 Buildroot提供了函数框架和变量命令框架，采用它的框架编写的**app_pkg.mk**这种Makefile格式的**自动构建脚本**，将被**package/pkg-generic.mk 这个核心脚本**展开填充到buildroot主目录下的Makefile中去。 最后make all执行Buildroot主目录下的Makefile，生成你想要的image。  package/pkg-generic.mk中通过调用同目录下的pkg-download.mk、pkg-utils.mk文件，已经帮你自动实现了下载、解压、依赖包下载编译等一系列机械化的流程。 你只要需要按照格式写app_pkg.mk，填充下载地址，链接依赖库的名字等一些特有的构建细节即可。 总而言之，Buildroot本身提供构建流程的框架，开发者按照格式写脚本，提供必要的构建细节，配置整个系统，最后自动构建出你的系统。 ![img](https://img2018.cnblogs.com/blog/1083701/201809/1083701-20180925083046481-2052342645.png) 3.添加自己的软件包 3.1添加package/Config.in入口 添加如下语句： `menu "myown(fuzidage) package" source "package/helloworld/Config.in" ` 为自己的软件包添加入口，这样在make menuconfig的时候就可以找到自己的软件包的Config.in，如果在make  menuconfig的时候选中helloworld，那么‘BR2_PACKAGE_HELLOWORLD=y’也会同步到.config中去。 3.2配置APP对应的Config.in和mk文件 在package中新增目录helloworld，并在里面添加Config.in和helloworld.mk 1)Config.in `config BR2_PACKAGE_HELLOWORLD bool "helloworld" help  This is a demo to add myown(fuzidage) package. ` helloworld/Config.in文件，可以通过make menuconfig可以对helloworld进行选择。只有在BR2_PACKAGE_HELLOWORLD=y条件下，才会调用helloworld.mk进行编译 2)helloworld.mk `################################################################################ # # helloworld # ################################################################################ HELLOWORLD_VERSION:= 1.0.0 HELLOWORLD_SITE:= $(CURDIR)/work/helloworld HELLOWORLD_SITE_METHOD:=local HELLOWORLD_INSTALL_TARGET:=YES define HELLOWORLD_BUILD_CMDS    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) all endef define HELLOWORLD_INSTALL_TARGET_CMDS    $(INSTALL) -D -m 0755 $(@D)/helloworld $(TARGET_DIR)/bin endef define HELLOWORLD_PERMISSIONS    /bin/helloworld f 4755 0 0 - - - - - endef $(eval $(generic-package)) ` helloworld.mk包括源码位置、安装目录、权限设置等。 3.3编写APP源码和Makefile 3.4通过make menuconfig选中APP 通过上面对package/Config.in入口的配置, 我们可以通过make menuconfig，进入Target packages可以看见多了一个"myown(fuzidage) package"入口，选中helloworld。
 然后make savedefconfig，对helloworld的配置就会保存到xxx_defconfig中. 3.5编译使用APP 可以和整个平台一起编译APP；或者make helloworld单独编译。 这两个文件在选中此APP之后，都会被拷贝到output/build/helloworld-1.0.0文件夹中。 然后生成的bin文件拷贝到output/target/bin/helloworld，这个文件会打包到文件系统中。 如果需要清空相应的源文件，通过make helloworld-dirclean。 4如何重新编译软件包 经过第一次完整编译后，如果我们需要对源码包重新配置，我们不能直接在buildroot上的根目录下直接make，buildroot是不知道你已经对源码进行重新配置，它只会将第一次编译出来的文件，再次打包成根文件系统镜像文件。 那么可以通过以下2种方式重新编译：  直接删除源码包,然后make all ` 例如我们要重新编译helloworld，那么可以直接删除output/build/helloworld目录，那么当你make的时候，就会自动从dl文件夹下，解压缩源码包，并重新安装。这种效率偏低 `  进行xxx-rebuild,然后make all ` 也是以helloworld为例子，我们直接输入make helloworld-rebuild，即可对build/helloworld/目录进行重新编译，然后还要进行make all(或者make world 或者 make target-post-image) `  如果要重新配置编译安装： `     make <package>-reconfigure; make all `  5.单独生成目标（build out of tree） `	make O=/home/lyb/output ` 效果如下：可以看见编译产生的output目录在/home/lyb下了： ![img](https://img2018.cnblogs.com/blog/1876680/201912/1876680-20191218152005932-2113885965.png)

具体的build训练教程点击如下链接.
 [buildroot官方训练教程](https://bootlin.com/doc/training/buildroot/)

​          原文地址：https://www.cnblogs.com/fuzidage/p/12049442.html    
