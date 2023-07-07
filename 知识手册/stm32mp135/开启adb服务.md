本文章暂时只是记录，后面尝试移植。



https://blog.csdn.net/pwl999/article/details/121873236



文章目录
1. 简介
2. ADBD 源码
3. Gadget Device 配置
3.1 functionfs
3.2 legacy 方式配置 functionfs
3.3 configfs 方式配置 functionfs
3.4 adb 使用配置
参考资料
1. 简介
ADB 是 Android 环境下非常好用的调试工具。它是 client-server架构，由三部分组成：

1、client (adb)，运行于开发机器 (Host) ，你可以在shell中调用adb命令，其它的Android工具比如ADT插件和DDMS也包含adb client。
2、server (adb server)，作为后台进程同样运行于开发机器 (Host) ，server负责管理client和运行于目标机器或者emulator的守护进程之间的通信，就像一座桥一样。
3、daemon (adbd)，运行于目标机 (Target) 或者emulator的守护进程。

本文并不想分析 ADB 的详细工作原理，而是聚焦于在一个普通的 Linux 环境中怎么配置起 ADBD 服务，让普通的 Linux 也能使用 ADB 工具来进行调试。

2. ADBD 源码
Android 的原生 ADBD 是需要和 Java Framework通讯的，并不能直接运行在普通 Linux 环境上。你得找到一份能在普通 Linux 环境上运行的 ADBD 的源码，在 github 上找到以下参考：

1.adbd-linux   https://github.com/tonyho/adbd-linux
2.Android Debug Bridge daemon implementation in RT-Thread  https://github.com/heyuanjie87/adbd
3.android-adbd-for-linux  https://github.com/kiddlu/android-adbd-for-linux
4.adbd for linux	  https://github.com/pengphei/adbd_port

首先把源码编译生成 adbd 可执行文件。

3. Gadget Device 配置
准备好用户态的 adbd 程序以后，还需要创建好好对应的 gadget device。

3.1 functionfs
在 Usb device 详解 一文中描述了 gadget device 的注册方法，通过 Legacy 或者 Configfs 方式创建一个 composite device，这个 composite device 可以包含多个 function。

但是通常情况下 gadget driver 中 function 只能在内核态操作 endpoint，能不能把 endpoint 的直接呈现到用户态？这样用户直接读写 endpoint 效率更高，而且可以把复杂的逻辑放在用户态从而简化内核态的逻辑。

functionfs 就是为了实现上述的设想而诞生的，它通过注册一个文件系统的方式把 endpoint 呈现到了用户态。

而 adbd 就是使用 functionfs 来读写 usb 数据的，所以我们要配置起 functionfs。

和之前一样配置起 functionfs 的 gadget device，有 Legacy 和 Configfs 两种方式。

3.2 legacy 方式配置 functionfs
Legacy 方式内核提供了一个 drivers/usb/gadget/legacy/g_ffs.c 驱动，可以先把它编译成独立的 ko 文件 g_ffs.ko来使用。

1、插入g_ffs.ko驱动。注册 functionfs 文件系统：
$ insmod g_ffs.ko idVendor=0x18d1 idProduct=0x4e26 functions=mtp,hid,adb
1
2.1、挂载 mtp 的 functionfs 文件系统，并启动 mtp 服务：
$ mkdir /dev/ffs-mtp && mount -t functionfs mtp /dev/ffs-mtp
$ ( cd /dev/ffs-mtp && mtp-daemon ) &
1
2
2.2、挂载 hid 的 functionfs 文件系统，并启动 hid 服务：
$ mkdir /dev/ffs-hid && mount -t functionfs hid /dev/ffs-hid
$ ( cd /dev/ffs-hid && hid-daemon ) &
1
2
2.3、挂载 adbd 的 functionfs 文件系统，并启动 adbd 服务：
$ mkdir -p /dev/usb-ffs/adb
$ mount -o uid=2000,gid=2000 -t functionfs adb /dev/usb-ffs/adb
$ adbd&
1
2
3
3.3 configfs 方式配置 functionfs
Configfs 方式内核提供了一个 drivers/usb/gadget/function/f_fs.c function，将其编译进内核，然后就可以通过 configfs 文件系统来进行配置：
``` bash
# 挂载 configfs
mount -t configfs none /sys/kernel/config
cd /sys/kernel/config/usb_gadget

mkdir g_adb
cd g_adb

echo "0x18d1" > idVendor
echo "0x4e26" > idProduct

mkdir configs/c.1
# 创建 'ffs' function，这样就注册了`functionfs` 文件系统
mkdir functions/ffs.adb

mkdir strings/0x409
mkdir configs/c.1/strings/0x409

echo "0123456789ABCDEF" > strings/0x409/serialnumber
echo "Google Inc." > strings/0x409/manufacturer
echo "FunctionFS gadget (adb)" > strings/0x409/product

echo "Conf 1" > configs/c.1/strings/0x409/configuration
echo 120 > configs/c.1/MaxPower

ln -s functions/ffs.adb configs/c.1

#  用户态挂载 `adbd`  的 `functionfs` 文件系统
mkdir -p /dev/usb-ffs/adb
mount -o uid=2000,gid=2000 -t functionfs adb /dev/usb-ffs/adb

echo s3c-hsotg > UDC
```

实际上 adbd 还需要 devpts 和 network 的配置，完整配置脚本如下：
``` bash
mkdir /dev/pts
mount -t devpts none /dev/pts

mount -t configfs none /sys/kernel/config
cd /sys/kernel/config/usb_gadget

mkdir g_adb
cd g_adb

echo "0x18d1" > idVendor
echo "0x4e26" > idProduct

mkdir configs/c.1
mkdir functions/ffs.adb

mkdir strings/0x409
mkdir configs/c.1/strings/0x409

echo "0123456789ABCDEF" > strings/0x409/serialnumber
echo "Google Inc." > strings/0x409/manufacturer
echo "FunctionFS gadget (adb)" > strings/0x409/product

echo "Conf 1" > configs/c.1/strings/0x409/configuration
echo 120 > configs/c.1/MaxPower

ln -s functions/ffs.adb configs/c.1

mkdir -p /dev/usb-ffs/adb
mount -o uid=2000,gid=2000 -t functionfs adb /dev/usb-ffs/adb

ifconfig lo up
ifconfig

cd /root
adbd&

sleep 1

echo `ls /sys/class/udc/` > /sys/kernel/config/usb_gadget/g_adb/UDC

```
3.4 adb 使用配置
如果 Host 是 windows 环境，修改 adb_usb.ini 文件，增加 0x4e26 字段，就能正常使用 adb 连接上述 Target 启动的 adbd 服务了。

参考资料
1.USB/Linux USB Layers/Configfs Composite Gadget/Usage eq. to g ffs.ko https://wiki.tizen.org/USB/Linux_USB_Layers/Configfs_Composite_Gadget/Usage_eq._to_g_ffs.ko
2.ptmx/pts https://blog.csdn.net/luckywang1103/article/details/71191821
3.functionfs.rst https://elixir.bootlin.com/linux/latest/source/Documentation/usb/functionfs.rst
4.Android 4.4 Kitkat 使能 USB adb 功能 https://blog.csdn.net/jingxia2008/article/details/25902817
5.Android开发工具ADB教程之一：ADB概论 http://www.jizhuomi.com/android/environment/209.html
6.Android开发工具ADB教程之二：HOST端 http://www.jizhuomi.com/android/environment/215.html
7.Android开发工具ADB教程之三：Dalvik虚拟机之jdwp线程 http://www.jizhuomi.com/android/environment/220.html
8.ADB 操作命令详解及用法大全 https://blog.csdn.net/lb245557472/article/details/84068519
9.adb和adbd分析 https://blog.csdn.net/viewsky11/article/details/53889143
————————————————
版权声明：本文为CSDN博主「pwl999」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/pwl999/article/details/121873236