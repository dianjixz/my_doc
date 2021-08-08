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


