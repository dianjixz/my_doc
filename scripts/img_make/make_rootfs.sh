#!/bin/bash

if [ "$1" == "" ] ; then
        exit -1
fi

set -x

sudo expect << EOF
spawn gdisk $1
expect "(? for help):" {send "o\r"}
expect "Proceed*:" {send "y\r"}
expect "(? for help):" {send "n\r"}
expect "Partition number*:" {send "\r"}
expect "First sector*:" {send "\r"}
expect "Last sector*:" {send "+32M\r"}
expect "Hex code or GUID*:" {send "\r"}
expect "(? for help):" {send "n\r"}
expect "Partition number*:" {send "\r"}
expect "First sector*:" {send "\r"}
expect "Last sector*:" {send "\r"}
expect "Hex code or GUID*:" {send "\r"}
expect "(? for help):" {send "x\r"}
expect "(? for help):" {send "s\r"}
expect "Enter new size*:" {send "4\r"}
expect "(? for help):" {send "w\r"}
expect "(Y/N):" {send "y\r"}
expect

EOF

#sudo parted -s $1 mklabel msdos
#sudo parted -s $1 mklabel gpt

#sleep 1
#sudo parted -s $1 mkpart boot fat16 1M 33M
#sudo parted -s $1 mkpart primary fat16 1M 33M
#sleep 1
#sudo parted -s $1 mkpart rootfs ext4 33M 100%
#sudo parted -s $1 mkpart primary ext4 33M 100%

sync
sleep 1
sudo mkfs.fat ${1}1
sleep 1
sudo su -c "yes |  mkfs.ext4 ${1}2"
sleep 1



sudo dd if=./u-boot-sunxi-with-spl.bin of=$1 bs=1024 seek=8
sleep 1

mkdir nihao

sudo mount ${1}1 ./nihao
sudo cp ./boot/* nihao -a
sudo umount nihao
sleep 1
sudo mount ${1}2 ./nihao
sudo cp ./rootfs/* nihao -a
sudo umount nihao
sync 
sudo rm nihao -r

exit 0