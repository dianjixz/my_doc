服务器端终端命令：

sudo apt-get install linux-tools-generic #安装包含USBIP的软件包

sudo modprobe usbip-core #使用modprobe自动处理可载入模块usbip-core

sudo modprobe vhci-hcd #使用modprobe自动处理可载入模块vhci-hcd

sudo modprobe usbip-vudc #使用modprobe自动处理可载入模块usbip-vudc

sudo modprobe usbip-host #使用modprobe自动处理可载入模块usbip-host

sudo usbip list -l #显示可被USBIP的USB设备busid值

sudo usbip bind -b 1-1 #把busid值为1-1的设备绑定到usbip-host

客户端终端命令：

sudo modprobe usbip-core #使用modprobe自动处理可载入模块usbip-core

sudo modprobe vhci-hcd #使用modprobe自动处理可载入模块vhci-hcd

sudo modprobe usbip-vudc #使用modprobe自动处理可载入模块usbip-vudc

sudo modprobe usbip-host #使用modprobe自动处理可载入模块usbip-host

sudo usbip attach -r 192.168.1.100 -b 1-1 #远程连接IP为192.168.1.100busid值为1-1的usb设备

sudo usbip port #查看客户端连接usb设备的端口号

sudo usbip detach -p 00 #断开port端口号为00的的远程USB连接


windows:
Windows

PS：安装配置方法也可以看官方README

安装方式与上面server端几乎一样，只是多了一个步骤

    安装VHCI驱动：以管理员身份打开CMD或者PowerShell，进入软件所在目录，运行如下命令

    usbip.exe install
        1

    如果后续不需要了，可以把驱动卸载了

    usbip.exe ubinstall
        1

配置

    以管理员身份打开CMD或者PowerShell，进入软件所在目录

    查看电脑A上可用的USB设备

    usbip.exe list --remote [电脑A的IP]

    USB 设备绑定至本机

    usbip.exe attach --remote=[电脑A的IP] --busid=[想要绑定设备的busid]

    查看已经绑定的远程USB设备

    usbip.exe port

    将已经绑定的远程USB设备解绑：端口来自命令usbip port输出中"Port"后面的数字

    usbip.exe detach -p [端口]
————————————————
版权声明：本文为CSDN博主「OTZ_2333」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/OTZ_2333/article/details/124073337
