




zynq下usb gadget模拟网口、U盘、串口(转)
https://www.cnblogs.com/arci/p/15360076.html













# 配置成网口

```bash

#!/bin/sh

do_start() {
    insmod libcomposite.ko 
    insmod u_ether.ko 
    insmod usb_f_rndis.ko 
    
    mount -t configfs none /sys/kernel/config
    
    mkdir /sys/kernel/config/usb_gadget/g1
    cd /sys/kernel/config/usb_gadget/g1
    
    echo "0x1d6b" > idVendor
    echo "0x0104" > idProduct
    
    echo "0x200" > bcdUSB
    echo "0xEF" > bDeviceClass
    echo "0x02" > bDeviceSubClass
    echo "0x01" > bDeviceProtocol
    echo "0x0100" > bcdDevice
    
    mkdir -p strings/0x409
    echo "0" > strings/0x409/serialnumber
    echo `uname -r` > strings/0x409/manufacturer
    echo `hostname -s` > strings/0x409/product
    
    # Config
    mkdir configs/c.1
    mkdir -p configs/c.1/strings/0x409
    echo "Config 1: RNDIS" > configs/c.1/strings/0x409/configuration
    echo 250 > configs/c.1/MaxPower
    echo 0xC0 > configs/c.1/bmAttributes
    
    mkdir functions/rndis.0
    # Windows extension to force RNDIS config
    echo "1" > os_desc/use
    echo "0xbc" > os_desc/b_vendor_code
    echo "MSFT100" > os_desc/qw_sign
    
    mkdir -p /functions/rndis.0/os_desc/interface.rndis
    echo "RNDIS" > functions/rndis.0/os_desc/interface.rndis/compatible_id
    echo "5162001" > functions/rndis.0/os_desc/interface.rndis/sub_compatible_id
    
    # Set up the rndis device only first
    ln -s functions/rndis.0 configs/c.1
    ln -s configs/c.1 os_desc
    
    echo "ci_hdrc.0" > UDC
    
    ifconfig usb0 192.168.7.100
    ifconfig usb0 up
}

do_stop() {
    # 卸载usb网口
    cd /sys/kernel/config/usb_gadget/g1
    echo "" > UDC
    rm os_desc/c.1/
    rm configs/c.1/rndis.0/
    rmdir configs/c.1/strings/0x409/
    rmdir configs/c.1/
    rmdir functions/rndis.0/
    rmdir strings/0x409/
    cd ..
    rmdir g1/
     
    rmmod u_ether.ko 
    rmmod usb_f_rndis.ko
    rmmod libcomposite.ko
}

case $1 in
    start)
        echo "Start usb gadget"
        do_start 
        ;;
    stop)
        echo "Stop usb gadget"
        do_stop
        ;;
    *)
        echo "Usage: $0 (stop | start)"
        ;;
esac
```




# 配置成U盘
```bash

#!/bin/sh

do_start() {
    insmod  libcomposite.ko
    insmod usb_f_mass_storage.ko
    dd bs=1M count=16 if=/dev/zero of=/tmp/lun0.img
    mount -t configfs none /sys/kernel/config
    mkdir /sys/kernel/config/usb_gadget/g1
    cd /sys/kernel/config/usb_gadget/g1
    echo "0x200" > bcdUSB
    echo "0x100" > bcdDevice
    echo "0x03FD" > idVendor
    echo "0x0500" > idProduct
    
    mkdir -p strings/0x409
    echo "0" > strings/0x409/serialnumber
    echo `uname -r` > strings/0x409/manufacturer
    echo `hostname -s` > strings/0x409/product
    
    mkdir -p  functions/mass_storage.0
    
    mkdir -p configs/c.1
    echo 120 > configs/c.1/MaxPower
    
     mkdir -p   functions/mass_storage.0/lun.0
    echo /tmp/lun0.img > functions/mass_storage.0/lun.0/file
    echo 1 > functions/mass_storage.0/lun.0/removable
    ln -s functions/mass_storage.0 configs/c.1
    echo "ci_hdrc.0" > UDC
}

do_stop() {
    # 卸载USB  U盘
    cd /sys/kernel/config/usb_gadget/g1
    echo "" > UDC
    rm configs/c.1/mass_storage.0/
    rmdir configs/c.1/
    rmdir functions/mass_storage.0/
    rmdir strings/0x409/
    cd ..
    rmdir g1/
    
    rmmod usb_f_mass_storage.ko
    rmmod libcomposite.ko
}

case $1 in
    start)
        echo "Start usb gadget"
        do_start 
        ;;
    stop)
        echo "Stop usb gadget"
        do_stop
        ;;
    *)
        echo "Usage: $0 (stop | start)"
        ;;
esac
```

# 配置成串口
```bash
#!/bin/sh

do_start() {
    insmod libcomposite.ko
    insmod u_serial.ko
    insmod usb_f_serial.ko
    insmod usb_f_acm.ko 
    mount -t configfs none /sys/kernel/config
    mkdir /sys/kernel/config/usb_gadget/g1
    cd /sys/kernel/config/usb_gadget/g1
    echo "0x200" > bcdUSB
    echo "0x100" > bcdDevice
    echo "0x03FD" > idVendor
    echo "0x0500" > idProduct
    
    mkdir -p strings/0x409
    echo "0" > strings/0x409/serialnumber
    echo `uname -r` > strings/0x409/manufacturer
    echo `hostname -s` > strings/0x409/product
    
    mkdir -p functions/acm.gs0
    
    mkdir -p configs/c.1
    echo 120 > configs/c.1/MaxPower
    ln -s functions/acm.gs0 configs/c.1/
    echo "ci_hdrc.0" > UDC
}

do_stop() {
    # 卸载USB串口
    cd /sys/kernel/config/usb_gadget/g1
    echo "" > UDC
    rm configs/c.1/acm.gs0/
    rmdir configs/c.1/
    rmdir functions/acm.gs0/
    rmdir strings/0x409/
    cd ..
    rmdir g1/
    
    rmmod u_serial.ko
    rmmod usb_f_serial.ko
    rmmod usb_f_acm.ko 
    rmmod libcomposite.ko
}

case $1 in
    start)
        echo "Start usb gadget "
        do_start 
        ;;
    stop)
        echo "Stop usb gadget"
        do_stop
        ;;
    *)
        echo "Usage: $0 (stop | start)"
        ;;
esac
```









https://blog.csdn.net/ccwzhu/article/details/112600846
USB 虚拟串口简介

    1. USB虚拟串口简介
    2.usb gadget 串口(serial)的配置和使用
    3.USB中CDC-ECM的了解和配置

1. USB虚拟串口简介

USB虚拟串口属于USB通信设备类。在物理层通过USB总线，采用虚拟串口的方式为主机提供一个物理串口。在系统内部，USB控制器提供了一个批量传输IN端点和一个批量传输的OUT端点，用于数据的接收和发送，模拟串口的RX和TX线。另外USB控制器还提供中断IN端点，发送当前串口的状态，实现对串口传输的控制。串口设备的数据，由系统的串口采集，在芯片内完成USB包的封装，通过USB总线上传至主机，再由相应的串口应用程序进行处理。对用户来说，看到的是基于串口的数据采集和传输，而实际上实现的是基于USB协议包的数据传输。

1.1 USB CDC协议简介
USB的CDC类是USB通信设备类（Communication Device Class）的简称。CDC类是USB组织定义的一类专门给各种通信设备（电信通信设备和中速网络通信设备）使用的USB子类。根据CDC类所针对通信设备的不同，CDC类又被分成以下不同的模型：USB传统纯电话业务（POTS）模型，USB ISDN模型和USB网络模型，其中USB传统纯电话业务模型又可分为直线控制模型（Direct Line Control Model）、抽象模型、电话模型，如图 1.1所示。本文讨论的虚拟串口就属于USB 传统纯电话业务模型下的抽象控制模型。

image.png

图 1.1 CDC分类

1.2 Linux下USB虚拟串口框架
如图 1.2所示，Linux实现了gadget.h定义的统一接口，然后上层的各种gadget driver（如serial.c等）调用这一套统一的接口可以去实现不同类型的功能，如USB串口、U盘、USB以太网等等。

图 1.2 USB虚拟串口框架

在Linux中通过USB虚拟的其他设备都需要经过Gadget层的绑定，最终与虚拟的相关设备进行绑定。
参考：https://blog.51cto.com/12572800/2043912

2.usb gadget 串口(serial)的配置和使用

    简介
    本篇文章主要是配置内核并使用 usb 的 gadget 的串口，即将带有usb口的linux平台，虚拟成一个串口设备。包括内核配置，加载和使用方法。最终在linux平台上会在创建出一个 /dev/ttyGS*节点。

    具体操作
    首先保证平台设备树中的相应的usb口的配置是slave模式，可以是 peripheral 模式，或者 otg。例子如下：

usb相关的节点中属性 dr_mode = “otg”; ，或者 dr_mode = “peripheral”;

    3.otg

OTG 增加了一根可以动态配置为 HOST 或者 DEVICE（slave）的数据线，以 micro USB 接头为例，其引脚分配如下：

因为传统的 USB 线缆为 4 根线，所以，要将 OTG 设备接入，需对其进行配置（硬件短接）：

　　1. 当配置 OTG 设备为 USB Device（slave） 时，将 ID 脚悬空。

　　2. 当配置 OTG 设备为 USB HOST时，将 ID 脚接地。

所以，我们这里，需要将OTG脚悬空，来将其配置为 Slave 设备。硬件上面，买来的 OTG 转接线默认把 OTG USB 设备设置为 HOST，他们 ID 脚都是接地的。而我们是需要把 OTG 设备当作 Device（slave）来用，所以，最终选择了将板子上的 ID 线割断，使其悬空。

然后配置menuconfig中的相关项，我配置成的是模块加载的形势(linux-4.14.98)

make menuconfig

    Device Drivers > USB support > USB Gadget Support


    make 时，生成的文件所在位置 drivers/usb/gadget/libcomposite.ko 和 drivers/usb/gadget/legacy/下的 u_serial.ko 和 drivers/usb/gadget/function/ 下的u_serial.ko usb_f_serial.ko usb_f_acm.ko。

在系统起来后分别按顺序加载

    insmod libcomposite.ko
    insmod u_serial.ko
    insmod usb_f_serial.ko
    insmod usb_f_acm.ko
    insmod g_serial.ko

    加载完后会报  "udc-core: couldn't find an available UDC - added [g_serial] to list of pending drivers"错误,这个是因为没有调用    retval = usb_add_gadget_udc(dev, &ci->gadget);函数把gaget udc_list添加到链表,即是需要开启内核选项,如下图所示:

          <*>   DesignWare USB3 DRD Core Support                                                                      
                             DWC3 Mode Selection (Dual Role mode)  --->

注：如果没有acm，需要配置上
menuconfig中的 > Device Drivers > USB support >
<*> USB Modem (CDC ACM) support
即使勾选上编译进内核了，仍然需要 usb_f_acm.ko 模块的加载。

如果是在ubuntu pc,需要加载驱动modprobe cdc-acm  cdc_ether
让window识别可能还需要这个文件 linux-cdc-acm.inf ，这个文件是在内核源码的 Documentation\usb目录下，电脑识别后的现象是自动识别出了com口

波特率等的配置不影响通信，
当我们用串口助手打开并发送数据时，是自发自收，当我们在板子中向节点发送数据时，串口助手能收到自发自收
在这里插入图片描述
板子向电脑发送

echo "hello" > /dev/ttyGS0

echo "123456" > /dev/ttyGS0

在这里插入图片描述
/dev 下自动创建了设备节点，如果需要手动创建，需要查看下设备节点主设备号

    cat /sys/class/tty/ttyGS0/dev

3.usb gadget storage的配置和使用

一. 内核配置

make menuconfig

    Device Drivers > USB support > USB Gadget Support

    make 时，生成的文件所在位置 drivers/usb/gadget/libcomposite.ko 和 drivers/usb/gadget/legacy/下的g_mass_storage.ko和 drivers/usb/gadget/function/ 下的usb_f_mass_storage.ko

二.加载驱动

1.分别先按顺序加载依赖驱动：

     insmod libcomposite.ko

     insmod usb_f_mass_storage.ko

2. 加载MSC驱动是需要参数的，可以直接让PC机识别SD卡，emmc，nandflash（emmc和nand默认格式为ext，PC机必须为linux才能识别）：

      （1）让ubuntu PC机识别板子上的emmc卡：

          使用命令挂载MSC驱动并传参，emmc dev路径：insmod g_mass_storage.ko file=/dev/mmcblk0p3 removable=1

          插上otg连接到PC机就可以读写板子上的emmc数据了，因为emmc的格式是fat的在windows下也能识别，

          读写emmc，nandflash使用方法一样，指定的路径分别改为emmc和nandflash的路径就可以了，但是不能被windows识别，因为windows仅能识别fat格式,

         如果要window能识别,必须要把/dev/mmcblk0p3 格式化为fat格式,可以通过过mkdosfs  / mkfs.vfat命令格式化为vfat格式

       （2）在内存创建指定格式的小空间，并跟PC机通讯

             创建一个镜像（本例放到/opt目录下，名为：ram.img，容量：128MB）：（仅需第一次创建时执行）

             dd if=/dev/zero of=/opt/ram.img bs=1M count=128

             格式化镜像为fat格式，因为windows仅能识别fat格式：（仅需第一次创建时执行）
             mkfs.vfat /opt/ram.img

             关联镜像和loop设备（利用loop设备来挂载）：（每次开机都要执行）
             losetup /dev/loop0 /opt/test.img

             挂载loop0到某个目录下：（每次开机都要执行）
             mount -t vfat /dev/loop0 /mnt

             挂载MSC驱动：（每次开机都有执行），先挂载两个依赖驱动

             insmod libcomposite.ko

             insmod usb_f_mass_storage.ko

             insmod g_mass_storage.ko file=/dev/loop0 removable=1            

             加载之后插入usb连接到PC，就可以在PC（win7）上访问板卡了。

            一切完成之后，验证是否正确。发现在windows下写入文件或者新建文件夹。在开发板的挂载点的文件夹中能同步到windows的信息。而在开发板中新建的文件或文件夹，需要拔下设备，重连之后才能同步到windows的可移动设备中。

4. usb gadget 二合一同时使用serial和storage的配置和使用

一. 内核配置

make menuconfig

Device Drivers > USB support > USB Gadget Support

make 时，生成的文件所在位置 drivers/usb/gadget/libcomposite.ko 和 drivers/usb/gadget/legacy/下的g_acm_ms.ko和 drivers/usb/gadget/function/ 下的u_serial.ko   usb_f_acm.ko  usb_f_mass_storage.ko

二.加载驱动

1.分别先按顺序加载依赖驱动：

    insmod libcomposite.ko

    insmod u_serial.ko 

    insmod usb_f_acm.ko 

    insmod usb_f_mass_storage.ko 

2. 加载MSC驱动是需要参数的，同上述单独加载usb gadget storage 类似

    insmod  g_acm_ms.ko file=/dev/mmcblk0p3 removable=1

加载完成g_acm_ms.ko后在windows会同时识别出串口和storage存储设备.

补充学习:

5. usb gadget 二合一同时使用serial和ethernet的配置和使用

一. 内核配置

make menuconfig

Device Drivers > USB support > USB Gadget Support

make 时，生成的文件所在位置 drivers/usb/gadget/libcomposite.ko 和 drivers/usb/gadget/legacy/下的g_cdc.ko和 drivers/usb/gadget/function/ 下的u_serial.ko   usb_f_acm.ko  u_ether.ko  usb_f_ecm.ko

二.加载驱动

1.分别先按顺序加载依赖驱动：

    insmod libcomposite.ko

    insmod u_serial.ko 

    insmod usb_f_acm.ko 

    insmod u_ether.ko

    insmod usb_f_ecm.ko 

    insmod g_cdc.ko

2.加载驱动成功后

1) 如果是在ubuntu pc,需要加载驱动modprobe cdc-acm  cdc_ether

2) 如果是在windows pc, 设备管理器会出现CDC ECM 警告,这个时候请安装windows版本 cdc ecm驱动

6. usb gadget 三合一同时使用serial storage 和ethernet(CDC-ECM)的配置和使用

一. 内核配置

make menuconfig

Device Drivers > USB support > USB Gadget Support

make 时，生成的文件所在位置 drivers/usb/gadget/libcomposite.ko 和 drivers/usb/gadget/legacy/下的g_multi.ko和 drivers/usb/gadget/function/ 下的u_serial.ko   usb_f_acm.ko  u_ether.ko  usb_f_ecm.ko   usb_f_mass_storage.ko

二.加载驱动

1.分别先按顺序加载依赖驱动：

    insmod libcomposite.ko

    insmod u_serial.ko 

    insmod usb_f_acm.ko 

    insmod u_ether.ko

    insmod usb_f_ecm.ko 

    insmod usb_f_mass_storage.ko

2.加载驱动成功后

2. 加载MSC驱动是需要参数的，同上述单独加载usb gadget storage 类似

    insmod  g_multi.ko file=/dev/mmcblk0p3 removable=1

加载完成g_multi.ko后在windows会同时识别出串口和storage存储设备还有cdc ECM设备

1) 如果是在ubuntu pc,需要加载驱动modprobe cdc-acm  cdc_ether

usb作为device端插入pc，在pc中会出现对用的usb虚拟网卡，可以像普通网卡一样传递数据，采用socket编程

在设备端执行ifconfig -a 会多出一个usb0网卡节点

root@canwang:~# ifconfig -a

usb0      Link encap:Ethernet  HWaddr AE:C6:B5:4A:90:85  
          inet6 addr: fe80::acc6:b5ff:fe4a:9085/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:231 errors:0 dropped:0 overruns:0 frame:0
          TX packets:462 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:25629 (25.0 KiB)  TX bytes:25140 (24.5 KiB)

eth0    Link encap:Ethernet  HWaddr F4:B5:49:32:13:11  
          inet6 addr: fe80::f6b5:49ff:fe32:1311/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:17490 errors:0 dropped:0 overruns:0 frame:0
          TX packets:15132 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:8737975 (8.3 MiB)  TX bytes:766641 (748.6 KiB)

给设备设置ip地址

root@canwang:~# ifconfig usb0 192.168.22.88

root@canwang:~# ifconfig
usb0      Link encap:Ethernet  HWaddr AE:C6:B5:4A:90:85  
          inet addr:192.168.22.88  Bcast:192.168.22.255  Mask:255.255.255.0
          inet6 addr: fe80::acc6:b5ff:fe4a:9085/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:231 errors:0 dropped:0 overruns:0 frame:0
          TX packets:462 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:25629 (25.0 KiB)  TX bytes:25140 (24.5 KiB)

yseth0    Link encap:Ethernet  HWaddr F4:B5:49:32:13:11  
          inet6 addr: fe80::f6b5:49ff:fe32:1311/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:17490 errors:0 dropped:0 overruns:0 frame:0
          TX packets:15132 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:8737975 (8.3 MiB)  TX bytes:766641 (748.6 KiB)

在Pc ubuntu执行ifconfig会多一个enp0s20f0u7网卡节点

root@andy:/# ifconfig
enp0s20f0u7 Link encap:以太网  硬件地址 12:e5:8c:e7:3c:fd  
          UP BROADCAST RUNNING MULTICAST  MTU:1500  跃点数:1
          接收数据包:1 错误:0 丢弃:0 过载:0 帧数:0
          发送数据包:0 错误:0 丢弃:0 过载:0 载波:0
          碰撞:0 发送队列长度:1000
          接收字节:76 (76.0 B)  发送字节:0 (0.0 B)

enp1s0    Link encap:以太网  硬件地址 1c:1b:0d:78:d1:dd  
          inet 地址:192.168.23.155  广播:192.168.23.255  掩码:255.255.255.0
          inet6 地址: fd78:3720:23e3:0:1e1b:dff:fe78:d1dd/64 Scope:Global
          inet6 地址: fe80::1e1b:dff:fe78:d1dd/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  跃点数:1
          接收数据包:24161909 错误:0 丢弃:2888 过载:0 帧数:0
          发送数据包:3807469 错误:0 丢弃:0 过载:0 载波:0
          碰撞:0 发送队列长度:1000
          接收字节:5017899977 (5.0 GB)  发送字节:1101530340 (1.1 GB)

给pc电脑设置ip地址

root@andy:/# ifconfig enp0s20f0u7 192.168.22.66

设备端ping pc电脑端ip

root@canwang:~# ping 192.168.22.66
PING 192.168.22.66 (192.168.22.66): 56 data bytes
64 bytes from 192.168.22.66: seq=0 ttl=64 time=0.385 ms
64 bytes from 192.168.22.66: seq=1 ttl=64 time=0.263 ms
64 bytes from 192.168.22.66: seq=2 ttl=64 time=0.221 ms
64 bytes from 192.168.22.66: seq=3 ttl=64 time=0.228 ms
64 bytes from 192.168.22.66: seq=4 ttl=64 time=0.176 ms
64 bytes from 192.168.22.66: seq=5 ttl=64 time=0.277 ms
64 bytes from 192.168.22.66: seq=6 ttl=64 time=0.211 ms
64 bytes from 192.168.22.66: seq=7 ttl=64 time=0.244 ms
64 bytes from 192.168.22.66: seq=8 ttl=64 time=0.164 ms
64 bytes from 192.168.22.66: seq=9 ttl=64 time=0.142 ms

2) 如果是在windows pc, 设备管理器会出现CDC ECM 警告,这个时候请安装windows版本 cdc ecm驱动(需要自己修改,inf文件)

由于这个windows对cdc-ecm支持不好,而对RNDIS支持比较好,如果要在windows使用,建议用RNDIS.下面继续实现RNDIS配置

7. usb gadget 三合一同时使用serial storage 和ethernet(RNDIS)的配置和使用

一. 内核配置

make menuconfig

Device Drivers > USB support > USB Gadget Support

make 时，生成的文件所在位置 drivers/usb/gadget/libcomposite.ko 和 drivers/usb/gadget/legacy/下的g_multi.ko和 drivers/usb/gadget/function/ 下的u_serial.ko   usb_f_acm.ko  u_ether.ko  usb_f_rndis.ko   usb_f_mass_storage.ko

二.加载驱动

1.分别先按顺序加载依赖驱动：

    insmod libcomposite.ko

    insmod u_serial.ko 

    insmod usb_f_acm.ko 

    insmod u_ether.ko

    insmod usb_f_rndis.ko 

    insmod usb_f_mass_storage.ko

2.加载驱动成功后

 加载g_multi驱动是需要参数的，同上述单独加载usb gadget storage 类似

    insmod  g_multi.ko file=/dev/mmcblk0p3 removable=1

加载完成g_multi.ko后在windows会同时识别出串口和storage存储设备还有RNDIS网络设备

让window识别可能还需要这个文件 linux.inf ，这个文件是在内核源码的 Documentation\usb目录下，电脑识别后的现象是自动识别出了RNDIS网络设备

在Pc windows执行ipconfig /all 会多一个网卡节点,如下图所示:

测试方式根上述6步骤一样
————————————————

                            版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
                        
原文链接：https://blog.csdn.net/ccwzhu/article/details/112600846