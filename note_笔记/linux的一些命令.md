resize2fs:调整ext文件系统的空间大小

 搭配逻辑卷lv使用方法：resize2fs /PATH/TO/LV_NAME 3G  调整LV的逻辑边界大小为3G
 resize2fs -p /PATH/TO/LV :-p表示扩展逻辑边界跟物理边界一样大。


 对img进行校验
~~~
e2fsck -p -f linuxroot.img
~~~
对img大小进行重新设定
~~~
resize2fs  -M linuxroot.img

resize2fs  -p linuxroot.img
~~~




运行dd命令刻录完镜像后可以用：sync命令来结束dd占用。

查看dd进度
~~~
sudo watch -n 5 pkill -USR1 ^dd$
~~~

动态调整分区大小的命令：
~~~
tunefs - 调整现有的UFS文件系统
growfs - 扩展现有的UFS文件系统
~~~


查看已经连接的设备
~~~
root@lsgxbsd:~ # camcontrol devlist
~~~
~~~
1. 删除ad0上所有分区

# gpart destroy -F /dev/da1

注: 没有数据情况才可以这样

2. 创建gpt分区信息表
# gpart create -s GPT /dev/da1 创建新的GPT分区表
# gpart create -s MBR /dev/da1 创建新的MBR分区表
# gpart create -s BSD /dev/da1 创建新的BSD分区表

3. 创建分区
# gpart add -b 34 -s 512k -t freebsd-boot /dev/da1
# gpart add -s 10g -t freebsd-ufs -l sys /dev/da1
# gpart add -s 4g -t freebsd-swap -l swap /dev/da1
# gpart add -s 20g -t freebsd /dev/da1

# gpart bootcode -b /boot/pmbr -p /boot/gptzfsboot -i 1 /dev/da1 （建议所有盘有freebsd-boot分区的盘都做）
# newfs -i 4096 /dev/da1p2 (启动分区和swa分区无需格式化)
# mount /dev/da1p2 /mnt/image/
# df -th
# umount /mnt/image/


-b 开始扇区,可省略
-s 分区大小,单位可用扇区,K,M等
-t 分区类型, 还有freebsd-ufs
-l 给分区打标签

4. 显示信息
# gpart show -l /dev/da1
# gpart show -r /dev/da1
# gpart show -p /dev/da1

5. 删除分区
# gpart delete -i 2 /dev/da1


# sysctl kern.geom.debugflags=16
# camcontrol devlist
# dd if=FreeBSD-10.1-RELEASE-i386-memstick.img of=/dev/da0 bs=64k

# mdconfig -a -t vnode -f /tmp/bootable.iso -u 3
# gnop create /dev/md3
# gnop list
# gpart show -p /dev/md3
# mount /dev/md3p3 /mnt/image
# mdconfig -d -u 3

查看磁盘信息
fdisk /dev/ad0
fdisk -s /dev/ad0
修改磁盘信息(问答式的交互)
fdisk -u /dev/ad0

# fsck -y /
# mount -u /
# mount -a
# mount -o remount, rw /

 

设置临时IP地址和默认路由
ifconfig em0 inet 192.168.195.99 netmask 255.255.255.0
ifconfig em0 inet6 accept_rtadv
route delete default
route add default 192.168.195.2
route add 192.168.195.0/24 192.168.195.2
route add 192.168.1.0/24 192.168.1.1
route add 192.168.2.0/24 192.168.2.1
route add 192.168.185.0/24 192.168.185.201
route flush
netstat -nr
netstat -nat

 

重启sshd服务

FreeBSD默认是不让root通过ssh登陆，遂修改相关的配置文件：
FreeBSD#vi /etc/sshd/sshd_config
#PermitRootLogin no
将这句修改成：
PermitRootLogin yes
保存退出，重启sshd服务却报错了：sshd re-exec requires execution with an absolute path
直接进入/etc/rc.d目录，输入sshd start，一样错误提示，
FreeBSD的命令执行方法或许变了，必须这样来：
FreeBSD#cd /etc/rc.d
FreeBSD#./sshd start|stop|restart
或直接输入完整的路径并带上命令操作：
FreeBSD#/etc/rc.d/sshd start|stop|restart





https://blog.csdn.net/qq_39753097/article/details/83349718?utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.nonecase&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.nonecase
Linux网络基本配置/IP/网关
一般是临时的修改，重启后失效，如：ifconfig、route、ip addr等。



eth0：网络接口
link encap : 网络类型
HWaddr : 网卡物理地址
Inet addr ：（internet address） IP地址
Bcast：广播地址
Mask：子网掩码
UP：正在使用的网络接口
RX packets,TX packets : 接收和传输的数据包个数
RX byte，TX byte : 表示接收和传输的具体数目
Interrupt：终端信息
Base address：内存地址






https://www.jb51.net/article/180111.htm







ifconfig eth0 192.168.120.56 netmask 255.255.255.0
#临时配置网卡ip子网码

route
#查看网关
添加路由条目
即时生效 ：route add default gw 192.168.1.1（自己设定）//添加默认网关

重启生效： vim  /etc/sysconfig/network
         --增加或修改下面一行
           NETWORKING=yes  

　　    GATEWAY=192.168.1.1
route命令

设置和查看路由表都可以用 route 命令，设置内核路由表的命令格式是：
Linux下用ifconfig命令设置IP、掩码、网关:https://blog.csdn.net/qq_28090573/article/details/82714028
# route  [add|del] [-net|-host] target [netmask Nm] [gw Gw] [[dev] If]

其中：

    add : 添加一条路由规则
    del : 删除一条路由规则
    -net : 目的地址是一个网络
    -host : 目的地址是一个主机
    target : 目的网络或主机
    netmask : 目的地址的网络掩码
    gw : 路由数据包通过的网关
    dev : 为路由指定的网络接口
https://www.cnblogs.com/zhaojingyu/p/9110961.html
使用 route 命令添加    

　　使用route 命令添加的路由，机器重启或者网卡重启后路由就失效了，方法：    

　　//添加到主机的路由    

　　# route add –host 192.168.168.110 (自己设置)  dev eth0    

　　# route add –host 192.168.168.119 (自己设置)  gw 192.168.168.1 (已存在的)    

　　//添加到网络的路由 

　　# route add –net IP (主机 ）netmask MASK eth0    

　　# route add –net IP netmask MASK gw IP (自己设置)    

　　# route add –net IP /24 eth1    

　　//添加默认网关    

　　# route add default gw IP (自己设置)   

　　//删除路由    

　　# route del –host 192.168.168.110 dev eth0 
3.配置网卡的硬件地址（MAC）

ifconfig eth0 hw ether xx：xx：xx：xx：xx：xx就将网卡的硬件地址更改了，此时你就可以骗过局域网内的IP地址绑定了。
原文链接：https://blog.csdn.net/qq_39753097/article/details/83349718



vim /etc/resolv.conf #设置你所要使用的DNS当自己不知道ip时，优先级最高,最多可写四个
在真机切换到超级用户 --> 去网关 --> systemctl restart network 重启网络 -->连接wifi --> 查看网关
/etc/resolv.conf是DNS客户机配置文件，用于设置DNS服务器的IP地址及DNS域名，还包含了主机的域名搜索顺序。
https://www.cnblogs.com/zhoading/p/9602423.html



linux interfaces配置文件详解
https://blog.csdn.net/mountzf/article/details/52035499
/etc/network/interfaces

　　 auto lo
　　 iface lo inet loopback
　　
　　 # The primary network interface
　　 auto eth0
　　 iface eth0 inet static
　　      address 192.168.0.42
　　      network 192.168.0.0
　　      netmask 255.255.255.0
　　      broadcast 192.168.0.255
　　      gateway 192.168.0.1




在系统的终端处修改打印级别，例如，屏蔽所有的内核printk打印，那么只需要将第一个数值调到最小值1或0即可
# echo 1 4 1 7 > /proc/sys/kernel/printk
or
# echo 0 4 0 7 > /proc/sys/kernel/printk



wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wpa_supplicant.conf -O/etc/wifi/sockets


wpa_supplicant_overlay.conf:
disable_scan_offload=1

wpa_supplicant.conf:
ctrl_interface=/etc/wifi/sockets
disable_scan_offload=1
update_config=1
wowlan_triggers=any


wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wpa_supplicant.conf -O/etc/sockets



[D-BUS Service] 
Name=fi.epitest.hostap.WPASupplicant  
Exec=/sbin/wpa_supplicant -u -f /var/log/wpa_supplicant.log 
User=root


[D-BUS Service] 
Name=fi.epitest.hostap.WPASupplicant  
Exec=/sbin/wpa_supplicant -u -onl80211 -O/var/run/wpa_supplicant 
User=root




../configure CC=aarch64-linux-gnu-gcc CXX=aarch64-linux-gnu-g++ AR=aarch64-linux-gnu-ar RANLIB=aarch64-linux-gnu-ranlib --host=aarch64-linux-gnu --build=arm64 --disable-ipv6 --prefix=/home/nihao/tool/Python-3.9.5/build/nihao ac_cv_file__dev_ptc=no ac_cv_file__dev_ptmx=no --eable-loadable-sqlite-extensions

iScas1001


~~~


~~~
$$ 		Shell本身的PID（ProcessID，即脚本运行的当前 进程ID号）
$! 		Shell最后运行的后台Process的PID(后台运行的最后一个进程的 进程ID号)
$? 		最后运行的命令的结束代码（返回值）即执行上一个指令的返回值 (显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误)
$- 		显示shell使用的当前选项，与set命令功能相同
$* 		所有参数列表。如"$*"用「"」括起来的情况、以"$1 $2 … $n"的形式输出所有参数，此选项参数可超过9个。
$@ 		所有参数列表。如"$@"用「"」括起来的情况、以"$1" "$2" … "$n" 的形式输出所有参数。
$@ 		跟$*类似，但是可以当作数组用
$# 		添加到Shell的参数个数
$0 		Shell本身的文件名
$1～$n 		添加到Shell的各参数值。$1是第1参数、$2是第2参数…。 


~~~



我们在正常情况下切换虚拟终端只要用组合键：Ctrl+Alt+Fn ,这个n就是你要切换的虚拟终端的号，然后可以再按Ctrl+Alt+F7回到图形界面。

但是在vmware中这个办法经常不奏效，原因大家应该知道的，就是在vmware下默认按住 Ctrl+Alt组合键，是用来在虚拟机和物理机之间互换的。

但是有了chvt这个命令，就可以这样办拉。呵呵

比如你现在在图形界面，想切换到tty1，也就是一号终端，只要执行

chvt 1 即可

如果你想回到图形己面，只要执行

chvt 7 即可。。
————————————————
版权声明：本文为CSDN博主「杨瘦锅」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_21063873/article/details/50161893
