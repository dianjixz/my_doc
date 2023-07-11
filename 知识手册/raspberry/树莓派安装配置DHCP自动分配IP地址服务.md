前言
Top

DHCP自动分配IP地址服务，全名是Dynamic Host Configuration Protocol，动态主机配置协议。他是一个企业局域网中有多台客户端或电脑为其分配IP地址的一个 UDP类型的协议。他能作为网络的中心管理的一种方式，在组织域控的时候会要用到。还有就是在局域网中为了方式IP分配上的冲突，通过DHCP自动协商进行IP的分配 。当然DHCP也是基于TCP/UDP协议的端口的，我们常见的DHCP就是基于UDP协议的67和68服务端口，所以作为防火墙一定要允许这两个端口访问。客户端方面采用的是 546端口来获取DHCPv6 Client提供的IP分配地址。
树莓派安装配置DHCP自动分配IP地址服务

树莓派安装DHCP服务
Top

为了顺利安装最新版本的DHCP服务到树莓派，我们要注意修改APT-GET安装源到最新的版本。这里我们在用安装的命令是；
sudo apt-get update 更新源信息到最新版本
sudo apt-get dist-upgrade 这里的应用比较耗时长
待完成上面的更新后就可以开始安装了，安装命令如下；
sudo apt-get install dhcp3-server
这个时候就不会报错了！


配置DHCP服务
Top

一般情况下安装好了DHCP服务后就可以修改配置文件了，DHCP服务的配置文件在/etc/dhcpd.conf中。用VIM编辑器打开后可以看到各种配置选项，具体选项信息解释如下；
ddns-update-style interim; //设置dhcp互动更新模式
ignore client-updates; //忽略客户端更新
subnet 192.168.12.0 netmask 255.255.255.0 {
option routers 192.168.12.1; //设置客户端默认网关
option subnet-mask 255.255.255.0; //设置客户端子网掩码
option nis-domain "ixdba.net "; //设置NIS域
option time-offset -18000; # Eastern Standard Time //设置时间偏差
range dynamic-bootp 192.168.12.128 192.168.12.254; //设置地址池
option domain-name " ixdba.net "; //设置DNS域
option domain-name-servers 192.168.12.1; //设置DNS服务器地址
default-lease-time 21600; //设置默认租期，单位为秒
max-lease-time 43200; //设置客户端最长租期，单位为秒
host ns { //以下设定分配静态IP地址，不多做解释。
next-server marvin.RedHat.com;
hardware ethernet 12:34:56:78:AB:CD;
fixed-address 207.175.42.254;
}}
根据自己的具体情况，只需要修改上面的配置信息即可。


sudo vim /etc/dhcpcd.conf


interface eth0    # 或 wlan0，根据你的网络接口选择
static ip_address=<静态 IP 地址>/24
static routers=<网关 IP 地址>
static domain_name_servers=<DNS 服务器 IP 地址>



interface eth0
static ip_address=192.168.1.100/24
static routers=192.168.1.1
static domain_name_servers=8.8.8.8










总结
Top

牛人技术在实际中测试发现，一个局域网中不能有多个DHCP服务器。否则会出现IP分配冲突的情况，从而导致局域网电脑和客户端不能正常连接到局域网中。这里一定要注意，建议关闭局域网中路由器上的DHCP服务。 