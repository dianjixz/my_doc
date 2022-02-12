 

# [multistrap build debian(ubuntu) rootfs](http://www.vjiot.net/typecho/index.php/archives/82/)





~~~ bash

[General]
#directory=target-rootfs
cleanup=true
noauth=true
unpack=true
debootstrap=Debian Net Utils
aptsources=Debian
noauth=true # GPG error

[Debian]
packages=apt kmod lsof
#source=https://mirrors.sjtug.sjtu.edu.cn/debian/
source=https://mirrors.tuna.tsinghua.edu.cn/debian/
keyring=debian-archive-keyring
suite=buster
components=main contrib non-free

[Net]
# Basic packages to enable the networking
packages=netbase net-tools ethtool udev iproute2 iputils-ping ifupdown isc-dhcp-client ssh
#source=https://mirrors.sjtug.sjtu.edu.cn/debian/
source=https://mirrors.tuna.tsinghua.edu.cn/debian/

[Utils]
# General purpose utilities
packages=locales adduser vim less wget dialog usbutils
#source=https://mirrors.sjtug.sjtu.edu.cn/debian/
source=https://mirrors.tuna.tsinghua.edu.cn/debian/

# sudo multistrap -a armel -d armel-rootfs -f multistrap.conf
# sudo multistrap -a armhf -d armhf-rootfs -f multistrap.conf
~~~

