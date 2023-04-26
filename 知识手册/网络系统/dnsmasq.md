目录

    一、简介
    二、dnsmasq安装
        三、dnsmasq的解析流程
    四、dnsmasq的作用以及相关配置
    五、DHCP 服务器设置

一、简介

    Dnsmasq 提供 DNS 缓存和 DHCP 服务功能。作为域名解析服务器(DNS)，dnsmasq可以通过缓存 DNS 请求来提高对访问过的网址的连接速度。作为DHCP 服务器，dnsmasq 可以用于为局域网电脑分配内网ip地址和提供路由。DNS和DHCP两个功能可以同时或分别单独实现。dnsmasq轻量且易配置，适用于个人用户或少于50台主机的网络。此外它还自带了一个 PXE 服务器。

什么是PXE？

    PXE ((Preboot eXecute Environment) )是由 Intel 公司开发的最新技术，工作于 Client/Server 的网络模式，支持工作站通过网络从远端服务器下载映像，并由此支持通过网络启动操作系统，在启动过程中，终端要求服务器分配IP地址，再用TFTP（trivial file transfer protocol）或MTFTP(multicast trivial file transfer protocol)协议下载一个启动软件包到本机内存中执行，由这个启动软件包完成终端（客户端）基本软件设置，从而引导预先安装在服务器中的终端操作系统。

PXE作用：

通过PXE技术，可以让局域网内部的机器通过网络启动系统安装程序，进而统一部署。网吧的无盘(硬盘)系统也是基于PXE开发出来的。
一个PXE服务系统由局域网中的 DHCP 服务器，TFTP 服务器组成。

二、dnsmasq安装

$ yum install -y dnsmasq
$ systemctl start dnsmasq.service

Dnsmasq的主要配置文件路径

/etc/dnsmasq.conf

dnsmasq经常修改的比较重要参数说明（/etc/dnsmasq.conf）
具体参数 	参数说明
resolv-file 	定义dnsmasq从哪里获取上游DNS服务器的地址， 默认从/etc/resolv.conf获取。
strict-order 	表示严格按照resolv-file文件中的顺序从上到下进行DNS解析，直到第一个解析成功为止。
listen-address 	定义dnsmasq监听的地址，默认是监控本机的所有网卡上。
address 	启用泛域名解析，即自定义解析a记录，例如：address=/long.com/192.168.115.10 访问long.com时的所有域名都会被解析成192.168.115.10
bogus-nxdomain 	对于任何被解析到此 IP 的域名，将响应 NXDOMAIN 使其解析失效，可以多次指定 通常用于对于访问不存在的域名，禁止其跳转到运营商的广告站点
server 	指定使用哪个DNS服务器进行解析，对于不同的网站可以使用不同的域名对应解析。 例如：server=/google.com/8.8.8.8 #表示对于google的服务，使用谷歌的DNS解析。

    注意：检查一下no-hosts前面是不是已经有了#号，默认的情况下是有的，dnsmasq 会首先寻找本地的 /etc/hosts 文件再去寻找缓存下来的域名, 最后去上游dns 服务器寻找。

    设置：listen-address=127.0.0.1，表示这个 dnsmasq 本机自己使用有效。注意：如果你想让本机所在的局域网的其它电脑也能够使用上Dnsmasq，应该把本机的局域网IP加上去：listen-address=192.168.1.123,127.0.0.1。注意：如果想允许所有的用户使用你的DNS解析服务器，把listen-address去掉即可。

三、dnsmasq的解析流程

 1. dnsmasq先去解析/etc/hosts文件。
 2. 再去解析/etc/dnsmasq.d/下的*.conf文件，并且这些文件的优先级要高于/etc/dnsmasq.conf。
 3. 我们自定义的resolv.dnsmasq.conf中的DNS也被称为上游DNS，这是最后去查询解析的。

四、dnsmasq的作用以及相关配置

（1）智能DNS加快解析速度，打开/etc/dnsmasq.conf文件，server=后面可以添加指定的DNS，例如国内外不同的网站使用不同的DNS。

国内指定DNS

server=/cn/114.114.114.114

server=/taobao.com/114.114.114.114

server=/taobaocdn.com/114.114.114.114

国外指定DNS

server=/google.com/223.5.5.5

（2）屏蔽网页广告，将指广告的URL指定127这个IP，就可以将网页上讨厌的广告给去掉了。

address=/ad.youku.com/127.0.0.1

address=/ad.iqiyi.com/127.0.0.1

（3）指定域名解析到特定的IP上。这个功能可以让你控制一些网站的访问，非法的DNS就经常把一些正规的网站解析到不正确IP上。

address=/freehao123.com/123.123.123.123

（4）管理控制内网DNS，首先将局域网中的所有的设备的本地DNS设置为已经安装Dnsmasq的服务器IP地址。然后修改已经安装Dnsmasq的服务器Hosts文件：/etc/hosts，指定域名到特定的IP中。

    例如想让局域网中的所有用户访问www.freehao123.com时跳转到192.168.0.2，添加：192.168.0.2 www.freehao123.com在Hosts文件中既可，整个过程也可以说是“DNS劫持”。

（5）三个以上域名服务器

    Linux 处理 DNS 请求时有个限制，在 resolv.conf 中最多只能配置三个域名服务器（nameserver）。作为一种变通方法,可以在 resolv.conf 文件中只保留 localhost 作为域名服务器，然后为外部域名服务器另外创建 resolv-file 文件。首先，为 dnsmasq 新建一个域名解析文件：

$ cp /etc/resolv.conf /etc/resolv.dnsmasq.conf

然后编辑 /etc/dnsmasq.conf 让 dnsmasq 使用新创建的域名解析文件：

$ vim  /etc/dnsmasq.conf
resolv-file=/etc/resolv.dnsmasq.conf

（6）使用NetworkManager

    NetworkManager 可以靠自身配置文件的设置项启动 dnsmasq 。在 NetworkManager.conf 文件的 [main] 节段添加 dns=dnsmasq 配置语句，然后禁用由 systemd 启动的 dnsmasq.service:

$ vi /etc/NetworkManager/NetworkManager.conf
[main]

plugins=keyfile

dns=dnsmasq

NetworkManager拉起dnsmasq是守护进程的，kill掉又会重新拉起服务，除非把NetworkManager.conf配置文件中的dns去掉，重启NetworkManager，再kill dnsmasq进程就不会自动拉起了。

dnsmasq 创建自定义配置

    可以在 /etc/NetworkManager/dnsmasq.d/ 目录下为 dnsmasq 创建自定义配置文件。例如，调整 DNS
    缓存大小（保存在内存中）：

$ vi /etc/NetworkManager/dnsmasq.d/cache
cache-size=1000

dnsmasq 被 NetworkManager 启动后，此目录下配置文件中的配置将取代默认配置。

IPv6

    启用 dnsmasq 在 NetworkManager 可能会中断仅持IPv6的DNS查询 (例如 dig -6 [hostname]) 否则将工作。 为了解决这个问题，创建以下文件将配置 dnsmasq 总是监听IPv6的loopback：

$ vim /etc/NetworkManager/dnsmasq.d/ipv6_listen.conf
listen-address=::1

五、DHCP 服务器设置

    dnsmasq默认关闭DHCP功能，如果该主机需要为局域网中的其他设备提供IP和路由，应该对dnsmasq 配置文件(/etc/dnsmasq.conf)必要的配置如下：

$ vi  /etc/dnsmasq.conf
# Only listen to routers' LAN NIC.  Doing so opens up tcp/udp port 53 to

# localhost and udp port 67 to world:

interface=<LAN-NIC>

# dnsmasq will open tcp/udp port 53 and udp port 67 to world to help with

# dynamic interfaces (assigning dynamic ips). Dnsmasq will discard world

# requests to them, but the paranoid might like to close them and let the

# kernel handle them:

bind-interfaces

# Dynamic range of IPs to make available to LAN pc

dhcp-range=192.168.111.50,192.168.111.100,12h

# If you’d like to have dnsmasq assign static IPs, bind the LAN computer's

# NIC MAC address:

dhcp-host=aa:bb:cc:dd:ee:ff,192.168.111.50

1）查看租约，存放租借信息（如IP）和租约信息（如租约时长），没有这个文件就创建

cat /var/lib/misc/dnsmasq.leases

