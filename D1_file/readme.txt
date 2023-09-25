sudo cp libpng12.so.0  /usr/lib/x86_64-linux-gnu/

然后就完成！Ubuntu19.04亲测可用，解决vlc安装，redisdesktop,mysqlworkbench安装问题。

#lcd
cd /sys/kernel/debug/dispdbg; 
echo disp0 > name; 
echo switch1 > command; 
echo 1 4 0 0 0x4 0x101 0 0 0 8 > param; 
echo 1 > start; 
echo 1 > /sys/class/disp/disp/attr/colorbar 


#hdim
echo 4 4 0 0 0x4 0x101 0 0 0 8 > param;

cd /sys/class/sunxi_dump
echo 0x02000090 > dump
cat dump



echo 0x02000090 0x44444004 > write


cat /sys/kernel/debug/pinctrl/2000000.pinctrl/pinmux-pins



echo 1 4 1 7 > /proc/sys/kernel/printk


deb http://192.168.0.56:8000/mirror/ftp.ports.debian.org/debian-ports sid main


i2cget -f -y 2 0x51 0x08
i2cget -f -y 2 0x51 0x07
i2cget -f -y 2 0x51 0x06
i2cget -f -y 2 0x51 0x05
i2cget -f -y 2 0x51 0x04
i2cget -f -y 2 0x51 0x03
i2cget -f -y 2 0x51 0x02
i2cget -f -y 2 0x51 0x02
i2cget -f -y 2 0x51 0x02
i2cget -f -y 2 0x51 0x02
i2cget -f -y 2 0x51 0x02


echo 0 > /sys/class/rfkill/rfkill0/state;
sleep 1
echo 1 > /sys/class/rfkill/rfkill0/state;
sleep 1

/etc/init.d/bluetooth start


/etc/bluetooth/bluetoothd start
hciattach -n ttyS1 xradio &
hciconfig -a
hciconfig hci0 up
hcitool dev


bluetoothctl

libjson-c-dev libglib2.0-dev libdbus-1-dev libdbus-c++-dev libudev-dev libical-dev libreadline-dev

../configure --program-prefix="" --program-suffix="" --prefix=/usr --exec-prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/etc --datadir=/usr/share --localstatedir=/var --mandir=/usr/man --infodir=/usr/info --disable-nls   --enable-static --enable-shared --enable-client --enable-datafiles --enable-experimental --enable-library --enable-monitor --enable-obex --enable-threads --enable-tools --disable-android --disable-cups --disable-manpages --disable-sixaxis --disable-systemd --disable-test --disable-udev --enable-deprecated --enable-mesh --localstatedir=/etc CFLAGS="-O0"


● chrony.service     loaded failed failed chrony, an NTP client/server
● networking.service loaded failed failed Raise network interfaces

cat > start_bluez << EOF
echo 0 > /sys/class/rfkill/rfkill0/state
sleep 1
echo 1 > /sys/class/rfkill/rfkill0/state
sleep 1 
/usr/lib/bluetooth/bluetoothd >> /dev/null 2>&1 &
sleep 1 
/usr/bin/hciattach -n ttyS1 xradio >> /dev/null 2>&1  &


EOF

bash -C start_bluez


bash -c "echo 0 > /sys/class/rfkill/rfkill0/state ; sleep 1 ; echo 1 > /sys/class/rfkill/rfkill0/state ; sleep 1 ; /usr/lib/bluetooth/bluetoothd >> /dev/null 2>&1 & ; /usr/bin/hciattach -n ttyS1 xradio >> /dev/null 2>&1  & "







SUBSYSTEMS=="net", KERNELS=="*?*", ATTR{ifindex}=="6", NAME="wlan0"
SUBSYSTEMS=="net", KERNELS=="*?*", ATTR{ifindex}=="7", NAME="wlan1"






#!/bin/bash


echo 1 4 1 7 > /proc/sys/kernel/printk

ifconfig wlan0 up >> /dev/null 2>&1  && wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf -B >> /dev/null 2>&1  && dhclient -i wlan0 >> /dev/null 2>&1 &




cat > start_bluez << EOF
echo 0 > /sys/class/rfkill/rfkill0/state
sleep 1
echo 1 > /sys/class/rfkill/rfkill0/state
sleep 1 
/usr/lib/bluetooth/bluetoothd >> /dev/null 2>&1 &
sleep 1 
/usr/bin/hciattach -n ttyS1 xradio >> /dev/null 2>&1  &

hciconfig hci0 down >> /dev/null 2>&1


EOF

bash -C start_bluez &




















