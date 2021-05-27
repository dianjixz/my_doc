#!/bin/bash


echo "Welcome to use sd to make scripts ！"
function help()
{
    echo "sudo ./make_rootfs.sh -d [device]"
    echo "-h    help"
    echo "-d    device!"


}

devices="n"

while getopts ":d:h" OPT;do
	case $OPT in
		  d)
                devices=$OPTARG
                echo "$OPTARG"
             	;;
		  h)    help
		  		
				;;
		  *)	help
				;;
	esac
done


echo "$devices"




if [ "$devices" == "n" ] ; then
    echo "please in you device!"
    exit -1
else

parted_yes=`sudo parted -v | grep "GNU parted" `
if [ "$parted_yes" == "" ] ; then

echo "please install parted."
echo "you can run \"sudo apt install parted\" "
exit -1
else
dev_nu=`sudo parted $devices p | grep $devices | awk '{print $3}' `


sudo parted -s $devices mklabel gpt
sudo parted -s $devices unit MB mkpart boot 1 17
sudo parted -s $devices mkpart rootfs 17MB $dev_nu

sudo mkfs.fat $devices"1"

sudo mkfs.ext4 $devices"2"

sudo parted -s $devices p

echo "make success!"




fi








fi

