USB问题：rejected 1 configuration due to insufficient





usb 2-1: new high speed USB device using musb_hdrc and address 2
usb 2-1: device v0bda p0119 is not supported
usb 2-1: New USB device found, idVendor=0bda, idProduct=0119
usb 2-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
usb 2-1: Product: USB2.0-CRW
usb 2-1: Manufacturer: Generic
usb 2-1: SerialNumber: 20090815198100000
usb 2-1: rejected 1 configuration due to insufficient available bus power
usb 2-1: no configuration chosen from 1 choice
 
解决方法：

你的USB设备需要的电量大于USB口所能提供的，据说 

这实际上是你的USB设备不规范导致的(没有使用标准规定的最大电量），据说

echo -n 1 > /sys/bus/usb/devices/2-1/bConfigurationValue 

可以临时解决这个问题。 我的例子里用了2-1,你要看你这个USB设备对应的号（就是kernel log里 rejected 1 configura



















echo -n 1 > /sys/devices/platform/soc/5800d000.usbh-ehci/usb1/1-1/1-1.2/bConfigurationValue