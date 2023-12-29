




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









