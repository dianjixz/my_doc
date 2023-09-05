 

Linux - NetworkManager网络管理工具

[![img](https://user-center.cdn.bcebos.com/head/raw/uc.101.5e08ba33.QBJMztMIL4rciv1peGHurw?x-bce-process=image/resize,m_lfit,w_200,h_200&autime=8438)](https://author.baidu.com/home?from=bjh_article&app_id=1582149344008199)

[linux运维菜](https://author.baidu.com/home?from=bjh_article&app_id=1582149344008199)

发布时间: 18-10-3020:32

**简介**

![img](https://t10.baidu.com/it/u=3124396005,3804265367&fm=173&app=25&f=JPEG?w=640&h=360&s=61F0AA661D338BD478E1B909010080D1)

网络管理器(NetworManager)是检测网络、自动连接网络的程序。无论是无线还是有线连接，它都可以令您轻松管理。对于无线网络,网络管理器优先连接已知的网络并可以自动切换到最可靠的无线网络。利用网络管理器的程序可以自由切换在线和离线模式。网络管理器会相对无线网络优先选择有线网络，支持 VPN。网络管理器最初由 Redhat 公司开发，现在由 GNOME 管理。

NetworkManager由一个管理系统网络连接、并且将其状态通过D-BUS（是一个提供简单的应用程序 互相通讯的途径的自由软件项目，它是作为freedesktoporg项目的一部分来开发的。）进行报告的后台服务，以及一个允许用户管理网络连接的客户端程序。

**优点**

简化网络连接的工作，让桌面本身和其他应用程序能感知网络。

**后台服务**

![img](https://t11.baidu.com/it/u=2665868179,229830125&fm=173&app=25&f=JPEG?w=500&h=400&s=2BA2536ECEA4E7760E5CD01B0000E081)

对于GNOME和Xfce那就是nm-applet工具，而对于KDE用户来说就是KNetworkManager。通过桌面的工具可以直接配置网络，不需要在命令行苦苦的配置，降低了操作难度。

**命令行工具**

![img](https://t10.baidu.com/it/u=2347546113,3512221216&fm=173&app=25&f=JPEG?w=640&h=400&s=29C3336E73B582661CD0B8020100C091)

对于没有图形界面配置的情况，networkmanager提供了命令行工具-nmcli ，包括在networkmanager中。

链接WiFi网络：

nmcli dev wifi connectpassword

通过wlan1接口连接 WiFi 网络:

nmcli dev wifi connectpasswordiface wlan1 [profile name]

断开一个接口:

nmcli dev disconnect iface eth0

重新连接一个标记为已断开的接口:

nmcli con up uuid

获得 UUID 列表:

nmcli con show

查看网络设备及其状态列表:

nmcli dev

关闭 WiFi:

nmcli r wifi off

**命令行GUI**

nmtui 是一个基于curses的图形化前端，包括在networkmanager中。

![img](https://t12.baidu.com/it/u=638172638,1610177767&fm=173&app=25&f=JPEG?w=640&h=323&s=CD21707E0EC3657E5E8A11910200F089)

直接输入nmtui就可以进入简单的命令行GUI进行操作。

**服务管理**

![img](https://t12.baidu.com/it/u=2093368847,2151313861&fm=173&app=25&f=JPEG?w=495&h=309&s=25B07494CAA1BA5318B5CD890200B083)

在systemd里面，可以直接使用systemctl进行管理

启动：systemctl start NetworkManger

关闭：systemctl stop NetworkManager

开机启动：systemctl enable NetworkManger

查看是否开机启动：systemctl is-enabled NetworkManager

禁用开机启动：systemctl disable NetworkManager

在CentOS6里面就有这个服务，可以用service和chkconfig里面进行管理：

启动：service NetworkManager start

关闭：service NetworkManager stop

开机启动：chkconfig NetworkManager on

禁用开机启动：chkconfig NetworkManager off






netplan, NetworkManager, systemd-networkd简介

1、systemd-networkd简介

systemd-networkd是systemd 的一部分 ，负责 systemd 生态中的网络配置部分(systemd-networkd.service, systemd-resolved.service)。使用 systemd-networkd，你可以为网络设备配置基础的 DHCP/静态IP网络等，还可以配置虚拟网络功能，例如网桥、隧道和 VLAN等等。systemd-networkd 与wpa_supplicant 服务配合可以支持配置WIFI无线适配器。具体见Linux网络管理工具systemd-networkd简介
2、NetworkManager简介

NetworkManager是一项后端服务(networkmanager.service)，是一个用于控制Ubuntu操作系统上的网络接口图形化界面的工具。NetworkManager和systemd-networked可以理解为相互替代关系，一个是基于图形化界面的工具，另外一个是基于命令行界面的工具。因此，用户可以根据自己的需求，决定使用networkmanager还是systemd-netword。 因此，如果要禁用NetworkManager，则应启用systemd-networkd，而在systemd-networkd运行时最好禁用networkmanager。
3、Netplan简介

Netplan是Canonical(Ubuntu)开发的做为某些 Linux 发行版（主力为ubuntu Linux发行版）上默认的网络配置命令行工具。Netplan 使用 YAML 描述文件来配置网络，然后，通过这些描述为任何给定的底层呈现工具(主要就是systemd-networkd和networkmanager二种工具)生成必要的配置选项。




wpa_supplicant -Dnl80211 -iwlx02e04c3ce83b -c/etc/wpa_supplicant.conf -B

使用systemd-networkd配置网络
https://code84.com/99756.html







