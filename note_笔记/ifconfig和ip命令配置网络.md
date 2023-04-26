ifconfig命令时早期网络管理的指令，目前Linux新的发行版本很多默认不带ifconfig命令，但可以以通过安装软件包net-tools方式进行使用.

ip命令时新型的网络管理命令，目前基本取代了ifconfig，新的Linux发行版本时一定有ip命令的。

ifconfig命令

ifconfig 查看当前所有up状态的网卡信息

ifconfig xxxx 查看xxxx网卡详细信息

ifconfig eth0 192.168.2.10 netmask 255.255.255.0 broadcast 192.168.2.255 配置网卡信息

ifconfig eth0 up   启动eth0网卡

ifconfig eth0 down  #关闭eth0网卡

ifconfig eth0 hw ether AA:AA:BB:CC:dd:EE  用ifconfig修改网卡eth0MAC地址

ifconfig eth0 arp   开启网卡eth0 的arp协议

ifconfig eth0 -arp   关闭网卡eth0 的arp协议

ip命令

ip link show # 显示网络接口信息

ip link set eth0 up # 开启网卡

ip link set eth0 down # 关闭网卡

ip link set eth0 promisc on # 开启网卡的混合模式

ip link set eth0 promisc offi # 关闭网卡的混个模式

ip link set eth0 txqueuelen 1200 # 设置网卡队列长度

ip link set eth0 mtu 1400 # 设置网卡最大传输单元

ip addr show # 显示网卡IP信息

ip addr add 192.168.0.1/24 dev eth0 # 设置eth0网卡IP地址192.168.0.1

ip addr del 192.168.0.1/24 dev eth0 # 删除eth0网卡IP地址

ip route show # 显示系统路由

ip route add default via 192.168.1.254 # 设置系统默认路由

ip route list # 查看路由信息

ip route add 192.168.4.0/24 via 192.168.0.254 dev eth0 # 设置192.168.4.0网段的网关为192.168.0.254,数据走eth0接口

ip route add default via 192.168.0.254 dev eth0 # 设置默认网关为192.168.0.254

ip route del 192.168.4.0/24 # 删除192.168.4.0网段的网关

ip route del default # 删除默认路由

ip route delete 192.168.1.0/24 dev eth0 # 删除路由
————————————————
版权声明：本文为CSDN博主「黄瓜炒肉」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/m0_72838865/article/details/126284744



ifconfig工具源码分析
https://blog.csdn.net/weixin_34357887/article/details/89748665?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-89748665-blog-98210117.235%5Ev30%5Epc_relevant_default_base3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-89748665-blog-98210117.235%5Ev30%5Epc_relevant_default_base3&utm_relevant_index=2



ifconfig是linux中用于显示或配置网络设备（网络接口卡）的命令，英文全称是network interfaces configuring。

同netstat一样，ifconfig源码也位于net-tools中。

源码位于net-tools工具包中，这是linux网络的基本工具包，此外还有arp,hostname,route等命令。

　　项目链接：http://net-tools.sourceforge.net/

       下载地址：https://sourceforge.net/projects/net-tools/files/latest/download

       下面一起来看下ifconfig的源码。