前言
Top

UDHCP服务是树莓派上一个小型的简单的DHCP动态IP分配服务，一些需要在树莓派搭建AP做无线WIFI热点的朋友就可以通过这个UDHCP进行IP地址的分配，虽然功能不算强大但是用于无线AP网络里分配IP还是足够了。这里简单说一下DHCP（Dynamic Host Configuration Protocol）从字面上面来说就是动态服务器配置服务协议，是通过UDP协议层在局域网中给各种电脑主机分配IP地址的，避免了用户手动输入IP的麻烦。接下来我们来看看如何在树莓派上安装UDHCP服务。
树莓派安装配置UDHCP简单IP分配服务

基本思路和主要的配置文件
Top

因为树莓派有多个网卡，一个是ETH0另一个是ETH1，这里我们是给无线AP做动态IP地址分配，所以我们要把UDHCP配置在ETH0网卡上。主要配置文件是；
/etc/network/interfaces
/etc/udhcpd.conf
/etc/default/udhcpd


使用apt-get在线安装UDHCP服务
Top

首先确保树莓派连接上了因特网，可以先PING一下百度看是否有数据包返回。树莓派连接到因特网后把树莓派软件包信息更新到最新版本，这样能安装最新最稳定的UDHCP服务。完成上面的操作后使用如下命令进行安装；
apt-get install udhcpd


配置UDHCP服务
Top

完成上面的操作后UDHCP就安装好了，这里我们打开他的配置文件进行必要的配置。首先我们修改网络接口文件，其命令如下；
# vim /etc/udhcpd.conf
修改网络接口interface eth0改为interface wlan0 使其在wlan0上分配
然后去掉remaining前注释# 变成 remaining yes

完成修改好，接下来检查下配置文件中OPT的一些选项，具体如下；
检查修改opt router 192.168.10.2（这里的IP是树莓派的ETH0网口IP地址）
在opt dns中设置好114.114.114.114服务地址（这里是指定DNS服务地址）
修改配置start开始IP和结束IP地址192.168.10.10~250（这里配置IP分配的范围）

然后继续编辑udhcpd文件，如下；
# vim /etc/default/udhcpd
使用#符号注释DHCPD_ENABLED="no"


树莓派网口的配置
Top

因为UDHCP是动态IP分配服务，但是不能给自己分配IP，所以需要给网卡分配IP地址。这里打开树莓派网卡配置文件，进行如下配置；
# vim /etc/network/interfaces
注释 #allow-hotplug wlan0等信息并增加

auto eth0
iface eth0 inet static
address 192.168.1.107
netmask 255.255.255.0
gateway 192.168.1.1

auto wlan0
iface wlan0 inet static
address 192.168.10.2
netmask 255.255.255.0
up iptables-restore < /etc/iptables.ipv4.nat #（可选）把规则导入到防火墙


启动UDHCP和开机启动
Top

服务启动命令 # sudo service udhcpd restart
设置到开机启动命令 sudo update-rc.d udhcpd enable 