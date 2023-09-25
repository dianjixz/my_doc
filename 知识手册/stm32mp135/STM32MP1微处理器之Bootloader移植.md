

https://www.douban.com/group/topic/246695003/?type=collect&_i=4834319NTDA2a6

linux 别人的教程/
https://wiki.t-firefly.com/Core-3588J/started.html

1.实验原理
1.1 概念

简单地说，Bootloader就是在操作系统内核运行之前运行的一段程序，它类似于PC机中的BIOS程序。通过这段程序，可以完成硬件设备的初始化，并建立内存空间的映射图的功能，从而将系统的软硬件环境带到一个合适的状态，为最终调用系统内核做好准备。

通常，Bootloader是严重地依赖于硬件实现的，特别是在嵌入式中。因此，在嵌入式世界里建立一个通用的Bootloader几乎是不可能的。尽管如此，仍然可以对Bootloader归纳出一些通用的概念来指导用户特定的Bootloader设计与实现。

（1）Bootloader所支持的CPU和嵌入式开发板

每种不同的CPU体系结构都有不同的Bootloader。有些Bootloader也支持多种体系结构的CPU，如后面要介绍的U-Boot就同时支持ARM体系结构和MIPS体系结构。除了依赖于CPU的体系结构外，Bootloader实际上也依赖于具体的嵌入式板级设备的配置。

（2）Bootloader的安装媒介

系统加电或复位后，所有的CPU通常都从某个由CPU制造商预先安排的地址上取指令。而基于CPU构建的嵌入式系统通常都有某种类型的固态存储设备（比如ROM、EEPROM或FLASH等）被映射到这个预先安排的地址上。因此在系统加电后，CPU将首先执行Bootloader程序。

（3）Bootloader的启动过程分为单阶段和多阶段两种。通常多阶段的Bootloader能提供更为复杂的功能，以及更好的可移植性。

（4）Bootloader的操作模式。大多数Bootloader都包含两种不同的操作模式：“启动加载”模式和“下载”模式，这种区别仅对于开发人员才有意义。

• 启动加载模式：这种模式也称为“自主”模式。也就是Bootloader从目标机上的某个固态存储设备上将操作系统加载到RAM中运行，整个过程并没有用户的介入。这种模式是嵌入式产品发布时的通用模式。

• 下载模式：在这种模式下，目标机上的Bootloader将通过串口连接或网络连接等通信手段从主机（Host）下载文件，比如：下载内核映像和根文件系统映像等。从主机下载的文件通常首先被Bootloader保存到目标机的RAM中，然后再被Bootloader写到目标机上的FLASH类固态存储设备中。Bootloader的这种模系统是在更新时使用。工作于这种模式下的Bootloader通常都会向它的终端用户提供一个简单的命令行接口。

（5）Bootloader与主机之间进行文件传输所用的通信设备及协议，最常见的情况就是，目标机上的Bootloader通过串口与主机之间进行文件传输，传输协议通常是xmodem/ ymodem/zmodem协议中的一种。但是，串口传输的速度是有限的，因此通过以太网连接并借助TFTP协议来下载文件是个更好的选择。
1.2 Bootloader启动流程

Bootloader的启动流程一般分为两个阶段：stage1和stage2，下面分别对这两个阶段进行讲解：

（1）Bootloader的stage1

在stage1中Bootloader主要完成以下工作。

• 基本的硬件初始化，包括屏蔽所有的中断、设置CPU的速度和时钟频率、RAM初始化、初始化LED、关闭CPU内部指令和数据cache灯。

• 为加载stage2准备RAM空间，通常为了获得更快的执行速度，通常把stage2加载到RAM空间中来执行，因此必须为加载Bootloader的stage2准备好一段可用的RAM空间范围。

• 拷贝stage2到RAM中，在这里要确定两点：①stage2的可执行映像在固态存储设备的存放起始地址和终止地址；②RAM空间的起始地址。

• 设置堆栈指针sp，这是为执行stage2的C语言代码做好准备。

（2）Bootloader的stage2

在stage2中Bootloader主要完成以下工作。

• 用汇编语言跳转到main入口函数

由于stage2的代码通常用C语言来实现，目的是实现更复杂的功能和取得更好的代码可读性和可移植性。但是与普通C语言应用程序不同的是，在编译和链接Bootloader这样的程序时，不能使用glibc库中的任何支持函数。

• 初始化本阶段要使用到的硬件设备，包括初始化串口、初始化计时器等。在初始化这些设备之前、可以输出一些打印信息。

• 检测系统的内存映射，所谓内存映射就是指在整个4GB物理地址空间中有指出哪些地址范围被分配用来寻址系统的RAM单元。

• 加载内核映像和根文件系统映像，这里包括规划内存占用的布局和从Flash上拷贝数据。

• 设置内核的启动参数。
Bootloader的种类

嵌入式系统世界已经有各种各样的Bootloader，种类划分也有多种方式。除了按照处理器体系结构不同划分以外，还有功能复杂程度的不同。

首先区分一下“Bootloader”和“Monitor”的概念。严格来说，“Bootloader”只是引导设备并且执行主程序的固件；而“Monitor”还提供了更多的命令行接口，可以进行调试、读写内存、烧写Flash、配置环境变量等。“Monitor”在嵌入式系统开发过程中可以提供很好的调试功能，开发完成以后，就完全设置成了一个“Bootloader”。所以，习惯上大家把它们统称为Bootloader。

下表列出了Linux的开放源码引导程序及其支持的体系结构。表中给出了X86、ARM、PowerPC体系结构的常用引导程序，并且注明了每一种引导程序是不是“Monitor”。

对于每种体系结构，都有一系列开放源码Bootloader可以选用。

（1）X86

X86的工作站和服务器上一般使用LILO和GRUB。LILO是Linux发行版主流的Bootloader。不过Redhat Linux发行版已经使用了GRUB，GRUB比LILO有更友好的显示接口，使用配置也更加灵活方便。

在某些X86嵌入式单板机或者特殊设备上，会采用其他的Bootloader，如ROLO。这些Bootloader可以取代BIOS的功能，能够从Flash中直接引导Linux启动。现在ROLO支持的开发板已经并入U-Boot，所以U-Boot也可以支持X86平台。

（2）ARM

ARM处理器的芯片商很多，所以每种芯片的开发板都有自己的Bootloader。结果ARM Bootloader也变得多种多样。最早有为ARM720处理器的开发板的固件，又有了armboot，StrongARM平台的BLOB，还有S3C2410处理器开发板上的vivi等。现在armboot已经并入了U-Boot，所以U-Boot也支持ARM/XSCALE平台。U-Boot已经成为ARM平台事实上的标准Bootloader。

（3）PowerPC

PowerPC平台的处理器有标准的Bootloader，就是PPCBOOT。PPCBOOT在合并armboot等之后，创建了U-Boot，成为各种体系结构开发板的通用引导程序。U-Boot仍然是PowerPC平台的主要Bootloader。

（4）MIPS

MIPS公司开发的YAMON是标准的Bootloader，也有许多MIPS芯片商为自己的开发板写了Bootloader。现在，U-Boot也已经支持MIPS平台。

（5）SH

SH平台的标准Bootloader是sh-boot。RedBoot在这种平台上也很好用。

（6）M68K

M68K平台没有标准的Bootloader。RedBoot能够支持M68K系列的系统。

值得说明的是RedBoot，它几乎能够支持所有的体系结构，包括MIPS、SH、M68K等。RedBoot是以eCos为基础，采用GPL许可的开源软件工程。现在由core eCos的开发人员维护，源码下载网站是
http://www.ecoscentric.com/snapshots。RedBoot的文档也相当完善，有详细的使用手册《RedBoot User’s Guide》。
1.3 U-Boot概述

U-Boot（UniversalBootloader），是遵循GPL条款的开放源码项目。它是从FADSROM、8xxROM、PPCBOOT逐步发展演化而来。其源码目录、编译形式与Linux内核很相似，事实上，不少U-Boot源码就是相应的Linux内核源程序的简化，尤其是一些设备的驱动程序，这从U-Boot源码的注释中能体现这一点。但是U-Boot不仅仅支持嵌入式Linux系统的引导，而且还支持NetBSD、VxWorks、QNX、RTEMS、ARTOS、LynxOS嵌入式操作系统。其目前要支持的目标操作系统是OpenBSD、NetBSD、FreeBSD、4.4BSD、Linux、SVR4、Esix、Solaris、Irix、SCO、Dell、NCR、VxWorks,LynxOS、pSOS、QNX、RTEMS、ARTOS。这是U-Boot中Universal的一层含义，另外一层含义则是U-Boot除了支持PowerPC系列的处理器外，还能支持MIPS、x86、ARM、NIOS、XScale等诸多常用系列的处理器。这两个特点正是U-Boot项目的开发目标，即支持尽可能多的嵌入式处理器和嵌入式操作系统。就目前为止，U-Boot对PowerPC系列处理器支持最为丰富，对Linux的支持最完善。

U-Boot的特点如下。

• 开放源码；

• 支持多种嵌入式操作系统内核，如Linux、NetBSD、VxWorks、QNX、RTEMS、ARTOS、LynxOS；

• 支持多个处理器系列，如PowerPC、ARM、x86、MIPS、XScale；

• 较高的可靠性和稳定性；

• 高度灵活的功能设置，适合U-Boot调试，操作系统不同引导要求，产品发布等；

• 丰富的设备驱动源码，如串口、以太网、SDRAM、FLASH、LCD、NVRAM、EEPROM、RTC、键盘等；

• 较为丰富的开发调试文档与强大的网络技术支持。

U-Boot可支持的主要功能列表。

• 系统引导：支持NFS挂载、RAMDISK（压缩或非压缩）形式的根文件系统。支持NFS挂载，并从FLASH中引导压缩或非压缩系统内核。

• 基本辅助功能：强大的操作系统接口功能；可灵活设置、传递多个关键参数给操作系统，适合系统在不同开发阶段的调试要求与产品发布，尤其对Linux支持最为强劲；支持目标板环境参数多种存储方式，如FLASH、NVRAM、EEPROM；CRC32校验，可校验FLASH中内核、RAMDISK镜像文件是否完好。

• 设备驱动：串口、SDRAM、FLASH、以太网、LCD、NVRAM、EEPROM、键盘、USB、PCMCIA、PCI、RTC等驱动支持。

• 上电自检功能：SDRAM、FLASH大小自动检测；SDRAM故障检测；CPU型号。

• 特殊功能：XIP内核引导。
1.4 U-Boot的常用命令

U-Boot上电启动后，按任意键可以退出自动启动状态，进入命令行。

U-Boot 2020.01-stm32mp-r1 (Aug 05 2020 - 05:32:37 +0000)

CPU: STM32MP157AAA Rev.B

Model: HQYJ FS-MP1A Discovery Board

Board: stm32mp1 in trusted mode (st,stm32mp157a-fsmp1a)

DRAM: 512 MiB

Clocks:

- MPU : 650 MHz

- MCU : 208.878 MHz

- AXI : 266.500 MHz

- PER : 24 MHz

- DDR : 533 MHz

WDT: Started with servicing (32s timeout)

NAND: 0 MiB

MMC: STM32 SD/MMC: 0, STM32 SD/MMC: 1

Loading Environment from MMC... OK

In: serial

Out: serial

Err: serial

Net: eth0: ethernet@5800a000

Hit any key to stop autoboot: 0

STM32MP>

在命令行提示符下，可以输入U-Boot的命令并执行。U-Boot可以支持几十个常用命令，通过这些命令，可以对开发板进行调试，可以引导Linux内核，还可以擦写Flash完成系统部署等功能。掌握这些命令的使用，才能够顺利地进行嵌入式系统的开发。

输入help命令，可以得到当前U-Boot的所有命令列表。每一条命令后面是简单的命令说明。

STM32MP> help

? - alias for 'help'

adc - ADC sub-system

base - print or set address offset

bdinfo - print Board Info structure

blkcache- block cache diagnostics and control

bmp - manipulate BMP image data

bootefi - Boots an EFI payload from memory

bootm - boot application image from memory

bootp - boot image via network using BOOTP/TFTP protocol

bootz - boot Linux zImage image from memory

chpart - change active partition

clk - CLK sub-system

cls - clear screen

cmp - memory compare

coninfo - print console devices and information

cp - memory copy

crc32 - checksum calculation

date - get/set/reset date & time

dcache - enable or disable data cache

dfu - Device Firmware Upgrade

dhcp - boot image via network using DHCP/TFTP protocol

dm - Driver model low level access

dtimg - manipulate dtb/dtbo Android image

echo - echo args to console

editenv - edit environment variable

env - environment handling commands

erase - erase FLASH memory

exit - exit script

ext2load- load binary file from a Ext2 filesystem

ext2ls - list files in a directory (default /)

ext4load- load binary file from a Ext4 filesystem

ext4ls - list files in a directory (default /)

ext4size- determine a file's size

ext4write- create a file in the root directory

false - do nothing, unsuccessfully

fastboot- run as a fastboot usb or udp device

fatinfo - print information about filesystem

fatload - load binary file from a dos filesystem

fatls - list files in a directory (default /)

fatsize - determine a file's size

fdt - flattened device tree utility commands

flinfo - print FLASH memory information

fstype - Look up a filesystem type

fuse - Fuse sub-system

go - start application at address 'addr'

gpio - query and control gpio pins

gpt - GUID Partition Table

help - print command description/usage

i2c - I2C sub-system

icache - enable or disable instruction cache

itest - return true/false on integer compare

lcdputs - print string on video framebuffer

led - manage LEDs

load - load binary file from a filesystem

loadb - load binary file over serial line (kermit mode)

loads - load S-Record file over serial line

loadx - load binary file over serial line (xmodem mode)

loady - load binary file over serial line (ymodem mode)

loop - infinite loop on address range

ls - list files in a directory (default /)

md - memory display

mdio - MDIO utility commands

meminfo - display memory information

mii - MII utility commands

mm - memory modify (auto-incrementing address)

mmc - MMC sub system

mmcinfo - display MMC info

mtdparts- define flash/nand partitions

mtest - simple RAM read/write test

mw - memory write (fill)

nand - NAND sub-system

nboot - boot from NAND device

nfs - boot image via network using NFS protocol

nm - memory modify (constant address)

part - disk partition related commands

ping - send ICMP ECHO_REQUEST to network host

pinmux - show pin-controller muxing

pmic - PMIC sub-system

poweroff- Perform POWEROFF of the device

printenv- print environment variables

protect - enable or disable FLASH write protection

pxe - commands to get and boot from pxe files

regulator- uclass operations

reset - Perform RESET of the CPU

rproc - Control operation of remote processors in an SoC

run - run commands in an environment variable

save - save file to a filesystem

saveenv - save environment variables to persistent storage

setcurs - set cursor position within screen

setenv - set environment variables

setexpr - set environment variable as the result of eval expression

sf - SPI flash sub-system

showvar - print local hushshell variables

size - determine a file's size

sleep - delay execution for some time

source - run script from memory

sspi - SPI utility command

stboard - read/write board reference in OTP

stm32key- Fuse ST Hash key

stm32prog- <link> <dev> [<addr>] [<size>]

start communication with tools STM32Cubeprogrammer on <link> with Flashlayout at <addr>

sysboot - command to get and boot from syslinux files

test - minimal test like /bin/sh

tftpboot- boot image via network using TFTP protocol

time - run commands and summarize execution time

timer - access the system timer

true - do nothing, successfully

ubi - ubi commands

ubifsload- load file from an UBIFS filesystem

ubifsls - list files in a directory

ubifsmount- mount UBIFS volume

ubifsumount- unmount UBIFS volume

ums - Use the UMS [USB Mass Storage]

usb - USB sub-system

usbboot - boot from USB device

version - print monitor, compiler and linker version

STM32MP>

U-Boot还提供了更加详细的命令帮助，通过help命令还可以查看每个命令的参数说明。由于开发过程的需要，有必要先把U-Boot命令的用法弄清楚。
2.实验目的

熟悉交叉工具链的使用、u-boot常用命令、u-boot的代码结构和移植方法。
3.实验平台

华清远见开发环境，FS-MP1A平台
4.实验步骤

本实验基于u-boot 2020.01版本，然后添加意法半导体提供的补丁文件。在意法半导体官方的u-boot中移植我们自己的u-boot。
5.导入源码

建立源码目录

linux@ubuntu:$ cd ~

linux@ubuntu:$ mkdir FS-MP1A

将【华清远见-FS-MP1A开发资料\02-程序源码\04-Linux系统移植\01-官方源码】下的
en.SOURCES-stm32mp1-openstlinux-5-4-dunfell-mp1-20-06-24.tar.xz压缩包，导入到ubuntu下的${HOME}/FS-MP1A目录下。

解压缩源码包

linux@ubuntu:$ tar xvf en.SOURCES-stm32mp1-openstlinux-5-4-dunfell-mp1-20-06-24.tar.xz

解压完成后得到“
stm32mp1-openstlinux-5.4-dunfell-mp1-20-06-24”目录

进入uboot目录下

linux@ubuntu:$ cd ~/FS-MP1A/stm32mp1-openstlinux-5.4-dunfell-mp1-20-06-24/sources/arm-ostl-linux-gnueabi/u-boot-stm32mp-2020.01-r0

该目录下以patch结尾的文件为ST官方提供的补丁文件；
u-boot-stm32mp-2020.01-r0.tar.gz为标准u-boot源码包。

解压标准u-boot源码包

linux@ubuntu:$ tar -xvf u-boot-stm32mp-2020.01-r0.tar.gz

解压完成后得到u-boot-stm32mp-2020.01目录

进入u-boot源码目录下：

linux@ubuntu:$ cd u-boot-stm32mp-2020.01

将ST官方补丁文件打到u-boot源码中：

linux@ubuntu:$ for p in `ls -1 ../*.patch`; do patch -p1 < $p; done

6.TF卡分区

要对TF卡进行烧录，需要先将TF接入到ubuntu系统中。

查看TF卡分区

linux@ubuntu:$ ls /dev/sd*

由上图所示只有“/dev/sdb1”一个分区则需要重新进行分区。

首先删除原有分区

linux@ubuntu:$ sudo parted -s /dev/sdb mklabel msdos

如果显示如下内容，则表示设备已经被挂载，需要卸载掉设备再删除分区。

卸载设备

linux@ubuntu:$ umount /dev/sdb1

卸载完成后重新执行删除分区命令

linux@ubuntu:$ sudo parted -s /dev/sdb mklabel msdos

对tf进行重新分区

linux@ubuntu:$ sudo sgdisk --resize-table=128 -a 1 -n 1:34:545 -c 1:fsbl1 -n 2:546:1057 -c 2:fsbl2 -n 3:1058:5153 -c 3:ssbl -n 4:5154:136225 -c 4:bootfs -n 5:136226 -c 5:rootfs -A 4:set:2 -p /dev/sdb -g

注意：最后-p /dev/sdb参数中的/dev/sdb需要按照实际ubuntu中的tf节点为准，否则可能发生不可预料的后果。
7.建立自己的平台

    配置工具链

导入交叉编译工具链（如果还未安装SDK可参考《SDK工具链安装》章节进行安装）

linux@ubuntu:$ source /opt/st/stm32mp1/3.1-openstlinux-5.4-dunfell-mp1-20-06-24/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi

验证开发工具是否安装正确，显示版本信息如下图所示。

linux@ubuntu:$ $CC --version

    增加板级相关文件

进入到u-boot源码目录

linux@ubuntu:$ cd ~/FS-MP1A/stm32mp1-openstlinux-5.4-dunfell-mp1-20-06-24/sources/arm-ostl-linux-gnueabi/u-boot-stm32mp-2020.01-r0/u-boot-stm32mp-2020.01/

添加自己的默认配置文件

linux@ubuntu:$ cp configs/stm32mp15_basic_defconfig configs/stm32mp15_fsmp1a_basic_defconfig

添加设备树文件

linux@ubuntu:$ cp arch/arm/dts/stm32mp15xx-dkx.dtsi arch/arm/dts/stm32mp15xx-fsmp1x.dtsi

linux@ubuntu:$ cp arch/arm/dts/stm32mp157a-dk1.dts arch/arm/dts/stm32mp157a-fsmp1a.dts

linux@ubuntu:$ cp arch/arm/dts/stm32mp157a-dk1-u-boot.dtsi arch/arm/dts/stm32mp157a-fsmp1a-u-boot.dtsi

修改
arch/arm/dts/stm32mp157a-fsmp1a.dts将

#include "stm32mp15xx-dkx.dtsi"

修改为

#include "stm32mp15xx-fsmp1x.dtsi"

修改arch/arm/dts/Makefile，在stm32mp157a-dk1.dtb下添加stm32mp157a-fsmp1a的内容：

    配置u-boot

使用新添加的默认配置：

linux@ubuntu:$ make stm32mp15_fsmp1a_basic_defconfig

可以使用make menuconfig进行图形化配置

    编译源码

执行如下指令编译u-boot：

linux@ubuntu:$ make DEVICE_TREE=stm32mp157a-fsmp1a all

编译成功后，最后显示内容(部分截图)如下：

编译完成后会得到如下文件：

    固件烧写

由于在移植过程中会多次烧写固件并且会导致正常u-boot无法启动，因此推荐使用TF卡启动的方式来验证。

将TF接入ubuntu系统后，查看TF卡分区

linux@ubuntu:$ ls /dev/sd*

/dev/sdb为TF卡设备。如果该设备下只有/dev/sdb1一个分区则重新分区。

执行如下指令烧写u-boot：

linux@ubuntu:$ sudo dd if=u-boot-spl.stm32 of=/dev/sdb1 conv=fdatasync

linux@ubuntu:$ sudo dd if=u-boot-spl.stm32 of=/dev/sdb2 conv=fdatasync

linux@ubuntu:$ sudo dd if=u-boot.img of=/dev/sdb3 conv=fdatasync

    启动开发板

将拨码开关设置为SD卡启动方式：

Boot mode

BOOT2

BOOT1

BOOT0

Engineering boot

1

0

0

Forced USB bootfor flashing

0

0

0

eMMC

0

1

0

SDCARD

1

0

1

将制作好的TF卡插入开发板，上电后会出现如下错误提示：

错误提示电源初始化错误，需重新调整电源相关配置
调整设备树电源配置

由于官方参考板DK1采用电源管理芯片做电源管理，而FS-MP1A采用分离电路作为电源管理，本例需要将文件中原有电源管理芯片相关内容去掉，增加上固定电源相关内容

    去掉原有电源管理内容

DK1参考板电源管理芯片挂在I2C4上，而FS-MP1A并未使用I2C4总线，所以直接将I2C4相关内容完全删除即可。

修改
arch/arm/dts/stm32mp15xx-fsmp1x.dtsi文件

将文件中i2c4节点相关内容整体删除，删除内容如下：

&i2c4 {

pinctrl-names = "default", "sleep";

pinctrl-0 = <&i2c4_pins_a>;

pinctrl-1 = <&i2c4_pins_sleep_a>;

i2c-scl-rising-time-ns = <185>;

i2c-scl-falling-time-ns = <20>;

clock-frequency = <400000>;

status = "disabled";

/*内容太长此处省略*/

watchdog {

compatible = "st,stpmic1-wdt";

status = "disabled";

};

};

};

修改
arch/arm/dts/stm32mp15xx-fsmp1x.dtsi文件，删除如下内容：

&cpu0{

cpu-supply = <&vddcore>;

};

&cpu1{

cpu-supply = <&vddcore>;

};

修改
arch/arm/dts/stm32mp157a-fsmp1a-u-boot.dtsi

文件

删除如下内容：

&pmic {

u-boot,dm-pre-reloc;

};

由于官方参考板DK1 I2C4总线上有个USB type C的控制器，上文删除I2C4节点的同时将type C控制器的描述删除，所以需要将引用type C控制器的内容删掉。

修改
arch/arm/dts/stm32mp15xx-fsmp1x.dtsi文件，删除红色部分内容：

&usbotg_hs {

phys = <&usbphyc_port1 0>;

phy-names = "usb2-phy";

usb-role-switch;

status = "okay";

port {

usbotg_hs_ep: endpoint {

remote-endpoint = <&con_usbotg_hs_ep>;

};

};

};

    添加固定电源配置

修改
arch/arm/dts/stm32mp15xx-fsmp1x.dtsi文件

固定电源配置通常添加在根节点下，在根节点末尾位置添加如下内容(红色字体为需要添加的内容)：

vin: vin {

compatible = "regulator-fixed";

regulator-name = "vin";

regulator-min-microvolt = <5000000>;

regulator-max-microvolt = <5000000>;

regulator-always-on;

};

v3v3: regulator-3p3v {

compatible = "regulator-fixed";

regulator-name = "v3v3";

regulator-min-microvolt = <3300000>;

regulator-max-microvolt = <3300000>;

regulator-always-on;

regulator-boot-on;

};

v1v8_audio: regulator-v1v8-audio {

compatible = "regulator-fixed";

regulator-name = "v1v8_audio";

regulator-min-microvolt = <1800000>;

regulator-max-microvolt = <1800000>;

regulator-always-on;

regulator-boot-on;

};

v3v3_hdmi: regulator-v3v3-hdmi {

compatible = "regulator-fixed";

regulator-name = "v3v3_hdmi";

regulator-min-microvolt = <3300000>;

regulator-max-microvolt = <3300000>;

regulator-always-on;

regulator-boot-on;

};

v1v2_hdmi: regulator-v1v2-hdmi {

compatible = "regulator-fixed";

regulator-name = "v1v2_hdmi";

regulator-min-microvolt = <1200000>;

regulator-max-microvolt = <1200000>;

regulator-always-on;

regulator-boot-on;

};

vdd: regulator-vdd {

compatible = "regulator-fixed";

regulator-name = "vdd";

regulator-min-microvolt = <3300000>;

regulator-max-microvolt = <3300000>;

regulator-always-on;

regulator-boot-on;

};

vdd_usb: regulator-vdd-usb {

compatible = "regulator-fixed";

regulator-name = "vdd_usb";

regulator-min-microvolt = <3300000>;

regulator-max-microvolt = <3300000>;

regulator-always-on;

regulator-boot-on;

};

};

    配置uboot

linux@ubuntu:$ make menuconfig

去掉PMIC的配置选项，按空格将方括号内*号去掉：

Device Drivers --->

Power --->

[ ] Enable support for STMicroelectronics STPMIC1 PMIC

    更新配置文件

linux@ubuntu:$ cp .config configs/stm32mp15_fsmp1a_basic_defconfig

    重新编译源码

linux@ubuntu:$ make DEVICE_TREE=stm32mp157a-fsmp1a all

    烧写后启动

重新烧写后启动现象如下：

提示显示TF卡未被检测到。
TF卡支持

系统检测TF卡拔插是通过CD脚的状态确认，通过原理图可知，TF卡对应MMC1的CD脚是与STM32MP1的PH3连接

对照设备树中MMC1的描述，发现设备树种原有CD脚的配置与FS-MP1A板子不一致，需调整为PH3。

修改
arch/arm/dts/stm32mp15xx-fsmp1x.dtsi文件，调整MMC1中CD的配置：

将如下内容

&sdmmc1 {

pinctrl-names = "default", "opendrain", "sleep";

pinctrl-0 = <&sdmmc1_b4_pins_a>;

pinctrl-1 = <&sdmmc1_b4_od_pins_a>;

pinctrl-2 = <&sdmmc1_b4_sleep_pins_a>;

cd-gpios = <&gpiob 7 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>;

disable-wp;

st,neg-edge;

bus-width = <4>;

vmmc-supply = <&v3v3>;

status = "okay";

};

修改为

&sdmmc1 {

pinctrl-names = "default", "opendrain", "sleep";

pinctrl-0 = <&sdmmc1_b4_pins_a>;

pinctrl-1 = <&sdmmc1_b4_od_pins_a>;

pinctrl-2 = <&sdmmc1_b4_sleep_pins_a>;

cd-gpios = <&gpioh 3 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>;

disable-wp;

st,neg-edge;

bus-width = <4>;

vmmc-supply = <&v3v3>;

status = "okay";

};

重新编译烧写后现象如下：

去掉ADC功能

上一小节已经成功启动到了u-boot的控制终端，但是可以看到还有一些错误。本小节将解决如下错误。

官方参考板DK1通过ADC检测开机电流，如果供电电流不足3A则启动失败，FS-MP1A没有设计这个功能，所以需要去掉这部分功能，否则就会报上图中显示的错误，由于u-boot期间ADC主要功能是检测开机电流，这里直接去掉ADC功能即可。

配置uboot去掉ADC功能：按空格键去掉[ ]的星号

linux@ubuntu:$ make menuconfig

Command line interface --->

Device access commands --->

[ ] adc - Access Analog to Digital Converters info and data

Device Drivers --->

[ ] Enable ADC drivers using Driver Model

更新配置文件

linux@ubuntu:$ cp .config configs/stm32mp15_fsmp1a_basic_defconfig

重新编译烧写后adc相关错误已经没有了。

关闭ltdc

由于在u-boot阶段我们不使用显示设备，因此需要将ltdc相关配置关闭。

修改
arch/arm/dts/stm32mp15xx-fsmp1x.dtsi文件

<dc {

pinctrl-names = "default", "sleep";

pinctrl-0 = <<dc_pins_a>;

pinctrl-1 = <<dc_pins_sleep_a>;

status = "okay";

port {

#address-cells = <1>;

#size-cells = <0>;

ltdc_ep0_out: endpoint@0 {

reg = <0>;

remote-endpoint = <&sii9022_in>;

};

};

};

修改为

<dc {

pinctrl-names = "default", "sleep";

pinctrl-0 = <<dc_pins_a>;

pinctrl-1 = <<dc_pins_sleep_a>;

status = "disabled";

port {

#address-cells = <1>;

#size-cells = <0>;

ltdc_ep0_out: endpoint@0 {

reg = <0>;

remote-endpoint = <&sii9022_in>;

};

};

};

然后重新编译烧写
有线网卡配置

启动设备进入u-boot控制台之后，可以发现启动信息中有网卡未被发现的错误信息，这个错误是由于u-boot没有设置正确的MAC地址，由启动信息可以看到，启动后网卡的MAC为00:00:00:00:00:00。这是由于在OTP中没有固化默认的MAC地址，解决这个问题可以通过烧写OTP配置来进行默认MAC地址配置，但是由于操作OTP配置的风险较高，如果操作不当可能会导致不可预料的结果。

如果u-boot期间不使用网卡这个错误可以忽略。

如果需要使用网卡解决这个问题比较简单的方法有两个：

    U-boot启动后通过命令设置一个MAC地址

STM32MP> env set -f ethaddr 1A:1F:DB:0E:69:FD

这种方式设置完成后即刻生效，重启后网卡即可正常工作。

    修改include/configs/stm32mp1.h，增加默认环境变量

修改如下内容：

#define CONFIG_EXTRA_ENV_SETTINGS \

"bootdelay=1\0" \

"kernel_addr_r=0xc2000000\0" \

"fdt_addr_r=0xc4000000\0" \

"scriptaddr=0xc4100000\0" \

为：

#define CONFIG_EXTRA_ENV_SETTINGS \

"bootdelay=1\0" \

"ethaddr=00:80:E1:42:60:17\0" \

"kernel_addr_r=0xc2000000\0" \

"fdt_addr_r=0xc4000000\0" \

"scriptaddr=0xc4100000\0" \

这种方式需重新编译烧写后才能生效。

重新编译烧写后网卡即可正常工作：

将网线插入开发板中可以进行网络测试。

使用dhcp命令获取ip地址：

STM32MP> dhcp

可以看到这里获取到的ip地址为192.168.11.81

Ping网关测试：

Board $> ping 192.168.11.1

Ping外网测试：

Board $> ping 8.8.8.8

eMMC移植

参考原理图可知eMMC使用的是SDMMC2总线，当前所使用的设备树文件中没有SDMMC2的支持，所以需要增加相关内容才能正常驱动eMMC。

由于在使STM32MP1芯片很多管脚为多功能复用管脚，且很多管脚具备同样的功能，所以移植eMMC时需要确认硬件设计是使用的是那些管脚，根据原理图确认后管脚对应关系为：

原理图网络编号

对应管脚

管脚功能

管脚功能码

SD2_DATA0

PB14

SDMMC2_D0

AF9

SD2_DATA1

PB15

SDMMC2_D1

AF9

SD2_DATA2

PB3

SDMMC2_D2

AF9

SD2_DATA3

PB4

SDMMC2_D3

AF9

SD2_DATA4

PA8

SDMMC2_D4

AF9

SD2_DATA5

PA9

SDMMC2_D5

AF10

SD2_DATA6

PE5

SDMMC2_D6

AF9

SD2_DATA7

PD3

SDMMC2_D7

AF9

SD2_CLK

PE3

SDMMC2_CK

AF9

SD2_CMD

PG6

SDMMC2_CMD

AF10

    管脚定义

在u-boot中STM32MP1默认管脚定义在文件
arch/arm/dts/stm32mp15-pinctrl.dtsi中，查看文件中是否有需要的管脚定义：

查看后确认有SDMMC2的管脚定义，且与FS-MP1A硬件使用情况一致，定义如下：

sdmmc2_b4_pins_a: sdmmc2-b4-0 {

pins1 {

pinmux = <STM32_PINMUX('B', 14, AF9)>, /* SDMMC2_D0 */

<STM32_PINMUX('B', 15, AF9)>, /* SDMMC2_D1 */

<STM32_PINMUX('B', 3, AF9)>, /* SDMMC2_D2 */

<STM32_PINMUX('B', 4, AF9)>, /* SDMMC2_D3 */

<STM32_PINMUX('G', 6, AF10)>; /* SDMMC2_CMD */

slew-rate = <1>;

drive-push-pull;

bias-pull-up;

};

pins2 {

pinmux = <STM32_PINMUX('E', 3, AF9)>; /* SDMMC2_CK */

slew-rate = <2>;

drive-push-pull;

bias-pull-up;

};

};

sdmmc2_b4_od_pins_a: sdmmc2-b4-od-0 {

pins1 {

pinmux = <STM32_PINMUX('B', 14, AF9)>, /* SDMMC2_D0 */

<STM32_PINMUX('B', 15, AF9)>, /* SDMMC2_D1 */

<STM32_PINMUX('B', 3, AF9)>, /* SDMMC2_D2 */

<STM32_PINMUX('B', 4, AF9)>; /* SDMMC2_D3 */

slew-rate = <1>;

drive-push-pull;

bias-pull-up;

};

pins2 {

pinmux = <STM32_PINMUX('E', 3, AF9)>; /* SDMMC2_CK */

slew-rate = <2>;

drive-push-pull;

bias-pull-up;

};

pins3 {

pinmux = <STM32_PINMUX('G', 6, AF10)>; /* SDMMC2_CMD */

slew-rate = <1>;

drive-open-drain;

bias-pull-up;

};

};

sdmmc2_b4_sleep_pins_a: sdmmc2-b4-sleep-0 {

pins {

pinmux = <STM32_PINMUX('B', 14, ANALOG)>, /* SDMMC2_D0 */

<STM32_PINMUX('B', 15, ANALOG)>, /* SDMMC2_D1 */

<STM32_PINMUX('B', 3, ANALOG)>, /* SDMMC2_D2 */

<STM32_PINMUX('B', 4, ANALOG)>, /* SDMMC2_D3 */

<STM32_PINMUX('E', 3, ANALOG)>, /* SDMMC2_CK */

<STM32_PINMUX('G', 6, ANALOG)>; /* SDMMC2_CMD */

};

};

sdmmc2_b4_pins_b: sdmmc2-b4-1 {

pins1 {

pinmux = <STM32_PINMUX('B', 14, AF9)>, /* SDMMC2_D0 */

<STM32_PINMUX('B', 15, AF9)>, /* SDMMC2_D1 */

<STM32_PINMUX('B', 3, AF9)>, /* SDMMC2_D2 */

<STM32_PINMUX('B', 4, AF9)>, /* SDMMC2_D3 */

<STM32_PINMUX('G', 6, AF10)>; /* SDMMC2_CMD */

slew-rate = <1>;

drive-push-pull;

bias-disable;

};

pins2 {

pinmux = <STM32_PINMUX('E', 3, AF9)>; /* SDMMC2_CK */

slew-rate = <2>;

drive-push-pull;

bias-disable;

};

};

sdmmc2_b4_od_pins_b: sdmmc2-b4-od-1 {

pins1 {

pinmux = <STM32_PINMUX('B', 14, AF9)>, /* SDMMC2_D0 */

<STM32_PINMUX('B', 15, AF9)>, /* SDMMC2_D1 */

<STM32_PINMUX('B', 3, AF9)>, /* SDMMC2_D2 */

<STM32_PINMUX('B', 4, AF9)>; /* SDMMC2_D3 */

slew-rate = <1>;

drive-push-pull;

bias-disable;

};

pins2 {

pinmux = <STM32_PINMUX('E', 3, AF9)>; /* SDMMC2_CK */

slew-rate = <2>;

drive-push-pull;

bias-disable;

};

pins3 {

pinmux = <STM32_PINMUX('G', 6, AF10)>; /* SDMMC2_CMD */

slew-rate = <1>;

drive-open-drain;

bias-disable;

};

};

sdmmc2_d47_pins_a: sdmmc2-d47-0 {

pins {

pinmux = <STM32_PINMUX('A', 8, AF9)>, /* SDMMC2_D4 */

<STM32_PINMUX('A', 9, AF10)>, /* SDMMC2_D5 */

<STM32_PINMUX('E', 5, AF9)>, /* SDMMC2_D6 */

<STM32_PINMUX('D', 3, AF9)>; /* SDMMC2_D7 */

slew-rate = <1>;

drive-push-pull;

bias-pull-up;

};

};

sdmmc2_d47_sleep_pins_a: sdmmc2-d47-sleep-0 {

pins {

pinmux = <STM32_PINMUX('A', 8, ANALOG)>, /* SDMMC2_D4 */

<STM32_PINMUX('A', 9, ANALOG)>, /* SDMMC2_D5 */

<STM32_PINMUX('E', 5, ANALOG)>, /* SDMMC2_D6 */

<STM32_PINMUX('D', 3, ANALOG)>; /* SDMMC2_D7 */

};

};

    增加SDMMC2节点信息

修改
arch/arm/dts/stm32mp15xx-fsmp1x.dtsi增加SDMMC2的信息

在原有sdmmc1节点下增加sdmmc2的内容，添加内容可参考
arch/arm/dts/stm32mp15xx-edx.dtsi中sdmmc2的写法，内容如下：

&sdmmc2 {

pinctrl-names = "default", "opendrain", "sleep";

pinctrl-0 = <&sdmmc2_b4_pins_a &sdmmc2_d47_pins_a>;

pinctrl-1 = <&sdmmc2_b4_od_pins_a &sdmmc2_d47_pins_a>;

pinctrl-2 = <&sdmmc2_b4_sleep_pins_a &sdmmc2_d47_sleep_pins_a>;

non-removable;

no-sd;

no-sdio;

st,neg-edge;

bus-width = <8>;

vmmc-supply = <&v3v3>;

vqmmc-supply = <&vdd>;

mmc-ddr-3_3v;

status = "okay";

};

    增加mmc映射

修改
arch/arm/dts/stm32mp157a-fsmp1a-u-boot.dtsi文件，调整启动通道。

修改如下内容：

aliases {

i2c3 = &i2c4;

mmc0 = &sdmmc1;

usb0 = &usbotg_hs;

};

修改为(红色字体为增加内容)：

aliases {

i2c3 = &i2c4;

mmc0 = &sdmmc1;

mmc1 = &sdmmc2;

usb0 = &usbotg_hs;

};

在sdmmc1节点后sdmmc2的内容(红色字体部分为添加内容)：

&sdmmc1 {

u-boot,dm-spl;

};

&sdmmc2 {

u-boot,dm-spl;

};

    测试

重新编译烧写后启动信息对比前文多一个MMC：

eMMC的验证需要通过linux更新，相关操作可参考《通过linux更新eMMC中的u-boot》章节。
生成Trusted镜像

前面我们做的相关配置都是基于basic的配置，而实际FS-MP1A使用的uboot是基于Trusted配置的镜像，后边的《Trusted Firmware-A移植》章节会使用到Trusted镜像。

    建立基础的Trusted配置文件

linux@ubuntu:$ cp configs/stm32mp15_trusted_defconfig configs/stm32mp15_fsmp1a_trusted _defconfig

linux@ubuntu:$ make stm32mp15_fsmp1a_trusted_defconfig

    去掉PMIC的配置选项，按空格将方括号内*号去掉：

linux@ubuntu:$ make menuconfig

Device Drivers --->

Power --->

[ ] Enable support for STMicroelectronics STPMIC1 PMIC

    去掉ADC功能：按空格键去掉[ ]的星号

linux@ubuntu:$ make menuconfig

Command line interface --->

Device access commands --->

[ ] adc - Access Analog to Digital Converters info and data

Device Drivers --->

[ ] Enable ADC drivers using Driver Model

    修改配置文件

linux@ubuntu:$ cp .config configs/stm32mp15_fsmp1a_trusted_defconfig

    修改上层目录下的Makefile.sdk编译脚本在UBOOT_CONFIGS配置项中添加“stm32mp15_fsmp1a_trusted_defconfig,trusted,u-boot.stm32”在DEVICE_TREE配置项中添加“stm32mp157a-fsmp1a”

UBOOT_CONFIGS ?= stm32mp15_fsmp1a_trusted_defconfig,trusted,u-boot.stm32 stm32mp15_trusted_defconfig,trusted,u-boot.stm32 stm32mp15_trusted_defconfig,optee,u-boot.stm32 stm32mp15_basic_defconfig,basic,u-boot.img

DEVICE_TREE ?= stm32mp157a-fsmp1a stm32mp157a-dk1 stm32mp157d-dk1 stm32mp157c-dk2 stm32mp157f-dk2 stm32mp157c-ed1 stm32mp157f-ed1 stm32mp157a-ev1 stm32mp157c-ev1 stm32mp157d-ev1 stm32mp157f-ev1

    编译trusted镜像

linux@ubuntu:$ make distclean

linux@ubuntu:$ make -f $PWD/../Makefile.sdk all UBOOT_CONFIGS=stm32mp15_fsmp1a_trusted_defconfig,trusted,u-boot.stm32

    编译完成后生成的镜像文件在上级目录下的build-trusted文件夹中有一个“u-boot-stm32mp157a-fsmp1a-trusted.stm32”

linux@ubuntu:$ cd ../ build-trusted

linux@ubuntu:$ ls

u-boot-stm32mp157a-fsmp1a-trusted.stm32即为我们后续会使用的镜像文件。







otg的主机和设备模式的切换

https://blog.csdn.net/u011505004/article/details/128626156


切换为主机模式
echo "host" >/sys/class/usb_role/49000000.usb-otg-role-switch/role
切换为设备模式
echo "device" >/sys/class/usb_role/49000000.usb-otg-role-switch/role




devpts on /dev/pts type devpts (rw,relatime,gid=5,mode=620,ptmxmode=000)
debugfs on /sys/kernel/debug type debugfs (rw,nosuid,nodev,noexec,relatime)




devtmpfs on /dev type devtmpfs (rw,relatime,size=405572k,nr_inodes=101393,mode=755)
proc on /proc type proc (rw,relatime)
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,relatime)
securityfs on /sys/kernel/security type securityfs (rw,nosuid,nodev,noexec,relatime)
tmpfs on /dev/shm type tmpfs (rw,nosuid,nodev)

tmpfs on /run type tmpfs (rw,nosuid,nodev,size=194468k,nr_inodes=819200,mode=755)
tmpfs on /sys/fs/cgroup type tmpfs (ro,nosuid,nodev,noexec,size=4096k,nr_inodes=1024,mode=755)
cgroup2 on /sys/fs/cgroup/unified type cgroup2 (rw,nosuid,nodev,noexec,relatime,nsdelegate)
cgroup on /sys/fs/cgroup/systemd type cgroup (rw,nosuid,nodev,noexec,relatime,xattr,name=systemd)
pstore on /sys/fs/pstore type pstore (rw,nosuid,nodev,noexec,relatime)
cgroup on /sys/fs/cgroup/memory type cgroup (rw,nosuid,nodev,noexec,relatime,memory)
mqueue on /dev/mqueue type mqueue (rw,nosuid,nodev,noexec,relatime)

tracefs on /sys/kernel/tracing type tracefs (rw,nosuid,nodev,noexec,relatime)
tmpfs on /tmp type tmpfs (rw,nosuid,nodev,size=486164k,nr_inodes=1048576)
configfs on /sys/kernel/config type configfs (rw,nosuid,nodev,noexec,relatime)




rtl8188fu             925696  0
sha256_generic         16384  0
libsha256              16384  1 sha256_generic
sha256_arm             24576  0
cfg80211              630784  1 rtl8188fu
stm32_adc              32768  0
stm32_lptimer_trigger    16384  1 stm32_adc
snd_soc_stm32_sai_sub    28672  0
stm32_timer_trigger    20480  1 stm32_adc
pwm_stm32              20480  0
usb_f_rndis            24576  2
u_ether                20480  1 usb_f_rndis
snd_soc_audio_graph_card    16384  0
snd_soc_simple_card_utils    20480  1 snd_soc_audio_graph_card
libcomposite           49152  10 usb_f_rndis
stm32_crc32            16384  0
stm32_adc_core         20480  0
out_to_pins            16384  0
snd_soc_core          172032  4 snd_soc_audio_graph_card,snd_soc_simple_card_utils,out_to_pins,snd_soc_stm32_sai_sub
snd_soc_stm32_sai      16384  0
snd_pcm_dmaengine      16384  1 snd_soc_core
stm32_timers           16384  1 pwm_stm32
snd_pcm                94208  3 snd_pcm_dmaengine,snd_soc_core,snd_soc_stm32_sai_sub
snd_timer              28672  1 snd_pcm
snd                    53248  4 snd_timer,snd_soc_core,snd_pcm,snd_soc_stm32_sai_sub
soundcore              16384  1 snd




modprobe stm32_adc_core
modprobe stm32_adc
modprobe pwm_stm32
modprobe stm32_timers
modprobe stm32_crc32
modprobe stm32_timer_trigger




5.调试方法

    使用sysfs接口对PWM驱动进行功能调试，主要调试命令示例如下。

（1）查看PWM控制器节点

ls /sys/class/pwm/pwmchip0

（2）打开指定PWM通道信号

echo n > /sys/class/pwm/pwmchip0/export 

 //n为通道编号

（3）设置PWM信号周期

echo pvalue > /sys/class/pwm/pwmchip0/pwm0/period   

 //pvalue为周期值

（4）设置PWM信号占空比

echo dvalue > /sys/class/pwm/pwmchip0/pwm0/duty_cycle  

//dvalue为有效电平宽度值

（5）使能某个PWM通道信号

echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable

（6）禁止某个PWM通道信号

echo 0 > /sys/class/pwm/pwmchip0/pwm0/enable




在Linux中，ADC（Analog-to-Digital Converter，模数转换器）设备通常在 `/sys` 目录下的 `class/hwmon` 或 `class/iio` 子目录中提供接口，以便用户空间程序可以读取模拟输入信号的数字化值。以下是 `/sys` 目录下ADC设备接口的一般说明：

1. **`/sys/class/hwmon` 接口：**

    在 `/sys/class/hwmon` 目录下，一些系统将ADC设备作为硬件监视器（hwmon）来表示，提供接口用于读取传感器的值，包括ADC。

    - `/sys/class/hwmon/hwmonX/`：每个硬件监视器设备都在这个目录下有一个子目录，通常以`hwmon`后跟一个数字（例如，`hwmon0`、`hwmon1`等）命名。
    - `/sys/class/hwmon/hwmonX/device/`：ADC设备通常在此子目录中提供接口。
    - `/sys/class/hwmon/hwmonX/device/inX_input`：ADC输入信号的值通常存储在名为`inX_input`的文件中，其中`X` 是输入通道号（例如，`in0_input`、`in1_input`等）。

2. **`/sys/class/iio` 接口：**

    在 `/sys/class/iio` 目录下，系统通常将ADC设备表示为"Industry I/O"设备，并提供接口以访问ADC通道。

    - `/sys/class/iio/iio:deviceX/`：每个IIO设备都在这个目录下有一个子目录，通常以`iio:device`后跟一个数字（例如，`iio:device0`、`iio:device1`等）命名。
    - `/sys/class/iio/iio:deviceX/in_voltageY_raw`：ADC输入通道的原始数据通常存储在名为`in_voltageY_raw`的文件中，其中`X` 是IIO设备号，`Y` 是通道号（例如，`in_voltage0_raw`、`in_voltage1_raw`等）。

请注意，以上只是一般情况，实际的目录和文件名可能因系统和硬件而异。你可以在 `/sys/class/hwmon` 和 `/sys/class/iio` 目录中浏览，查看可用的设备和通道，以找到你的ADC设备。一旦找到设备和通道，你可以读取相应的文件以获取ADC输入信号的值。

要注意的是，ADC设备和接口的具体细节会因硬件和内核版本而有所不同。因此，你可能需要查阅相关文档或使用 `cat` 命令查看文件以获取准确的信息。


https://blog.csdn.net/qq_32348883/article/details/123307693





modprobe rtl8188fu             		925696  0
modprobe sha256_generic         		16384  0
modprobe libsha256              		16384  1 sha256_generic
modprobe sha256_arm             		24576  0
modprobe cfg80211              		630784  1 rtl8188fu
modprobe stm32_adc              		32768  0
modprobe stm32_lptimer_trigger    	16384  1 stm32_adc
modprobe snd_soc_stm32_sai_sub    	28672  6
modprobe stm32_timer_trigger    		20480  1 stm32_adc
modprobe pwm_stm32              		20480  0
modprobe snd_soc_audio_graph_card    	16384  1
modprobe snd_soc_simple_card_utils    	20480  1 snd_soc_audio_graph_card
modprobe stm32_crc32            		16384  0
modprobe out_to_pins            		16384  1
modprobe stm32_adc_core         		20480  0
modprobe snd_soc_core          		172032  4 snd_soc_audio_graph_card,snd_soc_simple_card_utils,out_to_pins,snd_soc_stm32_sai_sub
modprobe snd_soc_stm32_sai      		16384  0
modprobe stm32_timers           		16384  1 pwm_stm32
modprobe snd_pcm_dmaengine      		16384  1 snd_soc_core
modprobe snd_pcm                		94208  3 snd_pcm_dmaengine,snd_soc_core,snd_soc_stm32_sai_sub
modprobe snd_timer              		28672  1 snd_pcm
modprobe snd                    		53248  6 snd_timer,snd_soc_core,snd_pcm,snd_soc_stm32_sai_sub
modprobe soundcore              		16384  1 snd










modprobe rtl8188fu             		
modprobe sha256_generic         	
modprobe libsha256              	
modprobe sha256_arm             	
modprobe cfg80211              		
modprobe stm32_adc              	
modprobe stm32_lptimer_trigger    	
modprobe snd_soc_stm32_sai_sub    	
modprobe stm32_timer_trigger    	
modprobe pwm_stm32              	
modprobe snd_soc_audio_graph_card   
modprobe snd_soc_simple_card_utils  
modprobe stm32_crc32            	
modprobe out_to_pins            	
modprobe stm32_adc_core         	
modprobe snd_soc_core          		
modprobe snd_soc_stm32_sai      	
modprobe stm32_timers           	
modprobe snd_pcm_dmaengine      	
modprobe snd_pcm                	
modprobe snd_timer              	
modprobe snd                    	
modprobe soundcore              	



RTC 使用

Linux 提供了三种用户空间调用接口。在 ITX-3588J 开发板中对应的路径为：

    SYSFS接口：/sys/class/rtc/rtc0/

    PROCFS接口： /proc/driver/rtc

    IOCTL接口： /dev/rtc0

YSFS接口

可以直接使用 cat 和 echo 操作 /sys/class/rtc/rtc0/ 下面的接口。

比如查看当前 RTC 的日期和时间：

# cat /sys/class/rtc/rtc0/date 
2022-06-21
# cat /sys/class/rtc/rtc0/time 
06:52:08

设置开机时间，如设置 120 秒后开机：

#120秒后定时开机
echo +120 >  /sys/class/rtc/rtc0/wakealarm
# 查看开机时间
cat /sys/class/rtc/rtc0/wakealarm
#关机
reboot -p

打印 RTC 相关的信息：

# cat /proc/driver/rtc
rtc_time        : 06:53:50
rtc_date        : 2022-06-21
alrm_time       : 06:55:05
alrm_date       : 2022-06-21
alarm_IRQ       : yes
alrm_pending    : no
update IRQ enabled      : no
periodic IRQ enabled    : no
periodic IRQ frequency  : 1
max user IRQ frequency  : 64
24hr            : yes

可以使用 ioctl 控制 /dev/rtc0。

详细使用说明请参考文档 kernel-5.10/Documentation/admin-guide/rtc.rst 。