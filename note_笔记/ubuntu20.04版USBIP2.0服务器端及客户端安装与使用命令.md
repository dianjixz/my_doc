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