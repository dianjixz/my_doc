#!/bin/bash






# while getopts ":d:f:bh" OPT;do
# 	case $OPT in
# 		  d)
#              	echo "asdasd"
#              	;;
#           f)    echo "The options is b."
#                 echo "OPTARG:$OPTARG"
#                 echo "OPTIND:$OPTIND"
#                 ;;

# 		  b)
# 				echo "asd"
# 				help
# 		  		;;
# 		  h)    help
		  		
# 				;;
# 		  *)	help
# 				;;
# 	esac
# done

echo "Welcome to use sd to make scripts ！"
echo "使用本脚本会有失败的情况发生，如果失败，请多次运行或手动分区！"
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

echo "正在格式化..."
sudo parted -s $devices mklabel msdos




echo "创建boot分区...."
sudo parted -s $devices mkpart primary fat16 1MB 16MB
echo "创建根文件分区...."
sudo parted -s $devices mkpart primary ext4 16MB %100
#sudo parted -s /dev/sdb mkpart primary ext4 16MB 100%
#sudo mkfs.fat $devices"1"

#sudo mkfs.ext4 $devices"2"

sudo parted -s $devices name 1 boot
sudo parted -s $devices name 2 rootfs





sudo parted -s $devices p

echo "make success!"




fi








fi










#!/bin/sh
#
# Load mpp modules....
#

MODULES_DIR="/lib/modules/`uname -r`"

start() {
    printf "Load mpp modules\n"
    insmod $MODULES_DIR/videobuf2-core.ko
    insmod $MODULES_DIR/videobuf2-memops.ko
    insmod $MODULES_DIR/videobuf2-dma-contig.ko
    insmod $MODULES_DIR/videobuf2-v4l2.ko
    insmod $MODULES_DIR/vin_io.ko

    insmod $MODULES_DIR/sensor_power.ko
    insmod $MODULES_DIR/sp2305_mipi.ko
    insmod $MODULES_DIR/ov9732_mipi.ko
    insmod $MODULES_DIR/vin_v4l2.ko
    swapon /swap.swap
    mmcp=`fdisk -l | grep /dev/mmcblk0p | wc -l`
    if [ $mmcp == "3" ] ; then
        echo -e "n\np\n635392\n\nw\n" | fdisk /dev/mmcblk0
        mkfs.vfat /dev/mmcblk0p4
        reboot
    else
        sleep 1 && sed -i "21,28d" /etc/init.d/S00mpp &
    fi
}

stop() {
    printf "Unload mpp modules\n"

    rmmod $MODULES_DIR/sensor_power.ko

    rmmod $MODULES_DIR/vin_v4l2.ko
    rmmod $MODULES_DIR/sp2305_mipi.ko
    rmmod $MODULES_DIR/ov9732_mipi.ko

    rmmod $MODULES_DIR/vin_io.ko
    rmmod $MODULES_DIR/videobuf2-v4l2.ko
    rmmod $MODULES_DIR/videobuf2-dma-contig.ko
    rmmod $MODULES_DIR/videobuf2-memops.ko
    rmmod $MODULES_DIR/videobuf2-core.ko
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart|reload)
        stop
        start
        ;;
  *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
esac

exit $?

















