0.Linux下的Wi-Fi配置

https://www.eet-china.com/mp/a143806.html

无线网络飞速发展的今天，许多设备都提供了连接无线网络的功能。
那么Linux下的wifi到底该怎么配置、连接呢？？
开始配置之前，我们要说说iw家族。iw是linux下常用的wifi配置工具，网上有相应的库和源码。全名为wirelessTools。
配置wifi模块，并连接相应的无线网络过程：主要使用iwpriv命令
（1）扫描可用的无线网络：


代码如下:
iwlist wifi-name scanning

其中wifi-name为无线网卡的名字，比如网卡eth0就是系统默认的网卡名字， wifi-name可以用ifconfig查看，一般为ra0。
（2）看扫描到的网络信息，按要连接的网络类型进行配置。以下为扫描到的网络：


代码如下:
iwlist ra0 scanning
===>rt_ioctl_giwscan. 11(11) BSS returned, data->length = 1427
ra0 Scan completed :
Cell 01 - Address: C4:CA:D9:1D:9E:A0
Protocol:802.11b/g/n
ESSID:""
Mode:Managed
Frequency:2.412 GHz (Channel 1)
Quality=7/100 Signal level=-87 dBm Noise level=-82 dBm
Encryption key:off
Bit Rates:54 Mb/s
Cell 02 - Address: FC:75:16:A1:A9:16
Protocol:802.11b/g/n
ESSID:"jxj_rd"
Mode:Managed
Frequency:2.452 GHz (Channel 9)
Quality=94/100 Signal level=-53 dBm Noise level=-92 dBm
Encryption key:on
Bit Rates:54 Mb/s
IE: WPA Version 1
Group Cipher : TKIP
Pairwise Ciphers (2) : TKIP CCMP
Authentication Suites (1) : PSK
IE: IEEE 802.11i/WPA2 Version 1
Group Cipher : TKIP
Pairwise Ciphers (2) : TKIP CCMP
Authentication Suites (1) : PSK
Cell 03 - Address: C4:CA:D9:02:2A:70
Protocol:802.11b/g/n
ESSID:""
Mode:Managed
Frequency:2.462 GHz (Channel 11)
Quality=83/100 Signal level=-57 dBm Noise level=-92 dBm
Encryption key:off
Bit Rates:54 Mb/s
Cell 04 - Address: 14:D6:4D:75:CA:9C
Protocol:802.11b/g/n
ESSID:"liangym"
Mode:Managed
Frequency:2.472 GHz (Channel 13)
Quality=100/100 Signal level=-27 dBm Noise level=-92 dBm
Encryption key:on
Bit Rates:54 Mb/s
IE: WPA Version 1
Group Cipher : CCMP
Pairwise Ciphers (1) : CCMP
Authentication Suites (1) : PSK
IE: IEEE 802.11i/WPA2 Version 1
Group Cipher : CCMP
Pairwise Ciphers (1) : CCMP
Authentication Suites (1) : PSK

如上：ESSID项的值即为无线网络的名字，如上的“jxj_rd”等。如果值为空，说明在无线路由勾选了隐藏ESSID的选项。
ESSID是很关键的，如果被隐藏了，也不是不能连接，而是需要破解，用到其他的工具，非常麻烦。至于如何破解，不是本文叙述的内容，有兴趣的朋友可以网上搜索。
得到了网络的信息，接下来就是配置连接选项了。以连接"jxj_rd"为例，我们看看具体的过程：
（1） 设置要连接的网络类型：


代码如下:
iwpriv ra0 set NetworkType=Infra

（2） 设置要连接的无线网络的安全模式：


代码如下:
iwpriv ra0 set AuthMode=WPA2PSK

（3） 设置网络加密方式：（CCMP即为AES）


代码如下:
iwpriv ra0 set EncrypType=TKIP

（4） 设置连接时的密码： ********为加密网络的密码


代码如下:
iwpriv ra0 set WPAPSK=*******

（5） 连接该网络：有两种方式：
A、用iwpriv命令


代码如下:
iwpriv ra0 set SSID=jxj_rd

B、用iwconfig命令


代码如下:
iwconfig ra0 essid jxj_rd

至此，如果密码正确，就可以连接上网络jxj_rd了。如果你不放心，可以用命令查看状态：


代码如下:
iwpriv ra0 connStatus

当然，你也可以用iwconfig命令查看状态，这里不再赘述。
执行 iwlist 如果 Supported interface modes 的下有 AP 的话表示该网卡支持AP模式，可以创建无线热点。 
比如我的无线网卡是Qualcomm Atheros AR9485 Wireless Network Adapter，Linux内核自带了驱动ath9k，支持创建AP无线热点。

下面我们就着重来讲无线热点的创建步骤：

1. 安装hostapd和dhcpd


代码如下:
sudo apt-get install hostapd isc-dhcp-server

开机不需要启动这两个服务,建议把它们关掉:


代码如下:
sudo update-rc.d -f hostapd remove

注释掉下面两个文件start on那一行:


代码如下:
sudo nano /etc/init/isc-dhcp-server.conf
sudo nano /etc/init/isc-dhcp-server6.conf

2. 配置hostapd和dhcpd


代码如下:
sudo nano /etc/hostapd/hostapd.conf

注意自己设置其中的无线热点名称ssid和认证密码wpa_passphrase.


代码如下:
interface=wlan0
driver=nl80211
ssid=YOUR_AP_NAME
hw_mode=g
channel=10
macaddr_acl=0
auth_algs=3
wpa=2
wpa_passphrase=YOUR_AP_PASSWORD
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP CCMP
rsn_pairwise=TKIP CCMP
sudo nano /etc/dhcp/dhcpd.conf

在末尾加入以下内容:


代码如下:
subnet 192.168.0.0 netmask 255.255.255.0
{
range 192.168.0.2 192.168.0.10;
option routers 192.168.0.1;
option domain-name-servers 192.168.0.1,180.76.76.76,8.8.8.8;
}

其中180.76.76.76是百度公共DNS服务器,8.8.8.8则是Google的DNS服务器.
在自己的家目录下创建一个hostapd目录，里面新建启动和关闭脚本，比如我这样写(注意把里面eechen换成你的用户名)：


代码如下:
nano /home/eechen/hostapd/ap-start.sh

代码如下:
#!/bin/bash
# 开启内核IP转发
bash -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
# 开启防火墙NAT转发(如果本机使用eth0上网,则把ppp0改为eth0)
iptables -t nat -A POSTROUTING -o ppp0 -j MASQUERADE
# 关闭NetworkManager对无线网卡的控制
nmcli nm wifi off
# 设置并启动无线网卡
ifconfig wlan0 192.168.0.1 netmask 255.255.255.0
# 解锁无线设备,可以用rfkill list查看解锁结果.
rfkill unblock wlan
# 睡眠6秒,待rfkill解锁生效
sleep 6s
# 启动dhcpd和hostapd,如果hostapd无法启动请查看日志hostapd.log,查看这两个进程ps -ef|egrep "dhcpd|hostapd"
nohup hostapd /etc/hostapd/hostapd.conf >/home/eechen/hostapd/hostapd.log 2>&1 &
dhcpd wlan0 -pf /var/run/dhcpd.pid
ps -ef|head -n1 && ps -ef|egrep "dhcpd|hostapd"
nano /home/eechen/hostapd/ap-stop.sh

代码如下:
#!/bin/bash
killall hostapd dhcpd
bash -c "echo 0 > /proc/sys/net/ipv4/ip_forward"
ifconfig wlan0 down

3. 启动hostapd和dhcpd
在桌面新建两个启动器（快捷方式），里面分别填入： 


代码如下:
gksu /home/eechen/hostapd/ap-start.sh
gksu /home/eechen/hostapd/ap-stop.sh

因为脚本需要root用户身份执行，所以需要用gksu运行。 
注意：如果你开启了ufw防火墙,ufw默认是禁止转发的,这会导致防火墙NAT转发失效,这时应该设置允许转发: 


代码如下:
sudo nano /etc/default/ufw 将 DEFAULT_FORWARD_POLICY 设为 ACCEPT

然后重启ufw即可: 


代码如下:
sudo service ufw restart

之后运行启动器即可启动和关闭AP无线热点。

4. 用wireshark监控经过wlan0的通信
安装wireshark（其中 eechen改为你的用户名）： 


代码如下:
sudo apt-get install wireshark
sudo chgrp eechen /usr/bin/dumpcap
sudo chmod 750 /usr/bin/dumpcap
sudo setcap cap_net_raw,cap_net_admin+eip /usr/bin/dumpcap

Alt+F2运行wireshark，选择网络接口wlan0，Filter里输入http.cookie contains mail.qq.com
然后手机端登录网页QQ邮箱，在WireShark即可查看和获取QQ邮箱登录的Cookie，如图所示：

右键点击Cookie项把Value复制出来，然后用Firefox访问站点https://w.mail.qq.com/，Alt+C激活 Original Cookie Injector（ GreaseMonkey），弹出输入框Wireshark Cookie Dump，把Cookie项的Value粘贴到里面，然后点OK确定并刷新页面即可登录QQ邮箱。或者你使用Tamper Data这个Firefox扩展在提交时修改Cookie值也是可以的。

附: 
查看连接wlan0网络的客户端: 


代码如下:
sudo arp

返回IP地址和MAC硬件地址和无线接口名. 


代码如下:
iw dev wlan0 station dump

能显示收发包和字节数等. 
查看网络接口速度: 


代码如下:
sar -n DEV 1 








中继器与AP的实现

这里我们假定中继器完成下面这样的一个功能：

    Wired Network ---> Wireless Network ---> 上级无线路由器

即从外部插入一个网线，通过imx6UL上面的无线网卡将数据转发到上级的路由器上面， 当然也可以完成下面这样的传送：

    Wireless Network（AP mode） --> Wireless Network（Client） --> 上级无线路由器

即设立一个AP热点，相当于一个路由器，然后再将这个AP的数据通过另外一个无线网卡转发到上一级的路由器或者WLAN上面。

用图片来表示如下：


要完成这些功能， 我们需要：

    1. 驱动无线网卡
    2. 建立AP热点
    3. 其中一个wired或者wireless介入到上一级路由器或者WLAN中
    4. 使用IPTables来对数据包进行转发


对于1，我们在前面已经完成，对于Wired Network访问WLAN我们也已经在前面完成。所以这篇我们将完成2与3， 以及4。


AP热点的建立

建立AP热点，我们使用经典的Hostapd工具来完成， 这个在OpenWRT中已经存在， 直接选上，然后build即可。
Hostapd的使用

然后我们可以查看对应的使用帮助

root@(none):/# hostapd --help
hostapd: invalid option -- '-'
hostapd v2.5
User space daemon for IEEE 802.11 AP management,
IEEE 802.1X/WPA/WPA2/EAP/RADIUS Authenticator
Copyright (c) 2002-2015, Jouni Malinen <j@w1.fi> and contributorsusage: hostapd [-hdBKtv] [-P <PID file>] [-e <entropy file>] \[-g <global ctrl_iface>] [-G <group>] \<configuration file(s)>options:-h   show this usage-d   show more debug messages (-dd for even more)-B   run daemon in the background-e   entropy file-g   global control interface path-G   group for control interfaces-P   PID file-K   include key data in debug messages-t   include timestamps in some debug messages-v   show hostapd version


因此可以确定需要一个config文件， 这个configuration file我们如下填写：

root@(none):/# cat /etc/hostapd_ori.conf 
interface=wlan1
ssid=TonyOpenWRT
channel=9
hw_mode=g
ignore_broadcast_ssid=0
auth_algs=1
wpa=3
wpa_passphrase=TonyHo123456
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP

即我们创建一个SSID为TonyOpenWRT的热点， wpa_passphrase为密码， 后面的为加密方式， 然后还选择了channel， 这个channel可以通过前面的iw list来查看， 例如其中一个为：

               Frequencies:* 2412 MHz [1] (20.0 dBm)* 2417 MHz [2] (20.0 dBm)* 2422 MHz [3] (20.0 dBm)* 2427 MHz [4] (20.0 dBm)* 2432 MHz [5] (20.0 dBm)* 2437 MHz [6] (20.0 dBm)* 2442 MHz [7] (20.0 dBm)* 2447 MHz [8] (20.0 dBm)* 2452 MHz [9] (20.0 dBm)* 2457 MHz [10] (20.0 dBm)* 2462 MHz [11] (20.0 dBm)* 2467 MHz [12] (disabled)* 2472 MHz [13] (disabled)* 2484 MHz [14] (disabled)


后面的【】中的就是channel了。

建立AP

然后我们使用后台运行的方式启动hostapd，并且打开debug信息，便于我们查看：

root@(none):/# hostapd -B -d /etc/hostapd_ori.conf 
random: Trying to read entropy from /dev/random
Configuration file: /etc/hostapd_ori.conf
rfkill: Cannot open RFKILL control device
nl80211: RFKILL status not available
nl80211: TDLS supported
nl80211: TDLS external setup
nl80211: Supported cipher 00-0f-ac:1
nl80211: Supported cipher 00-0f-ac:5
nl80211: Supported cipher 00-0f-ac:2
nl80211: Supported cipher 00-0f-ac:4
nl80211: Supported cipher 00-0f-ac:6
nl80211: Using driver-based off-channel TX
nl80211: Use separate P2P group interface (driver advertised support)
nl80211: interface wlan1 in phy phy1
nl80211: Set mode ifindex 9 iftype 3 (AP)
nl80211: Setup AP(wlan1) - device_ap_sme=0 use_monitor=0
nl80211: Subscribe to mgmt frames with AP handle 0xcfd5d8
nl80211: Register frame type=0xb0 (WLAN_FC_STYPE_AUTH) nl_handle=0xcfd5d8 match=
nl80211: Register frame type=0x0 (WLAN_FC_STYPE_ASSOC_REQ) nl_handle=0xcfd5d8 match=
nl80211: Register frame type=0x20 (WLAN_FC_STYPE_REASSOC_REQ) nl_handle=0xcfd5d8 match=
nl80211: Register frame type=0xa0 (WLAN_FC_STYPE_DISASSOC) nl_handle=0xcfd5d8 match=
nl80211: Register frame type=0xc0 (WLAN_FC_STYPE_DEAUTH) nl_handle=0xcfd5d8 match=
nl80211: Register frame type=0xd0 (WLAN_FC_STYPE_ACTION) nl_handle=0xcfd5d8 match=
nl80211: Register frame type=0x40 (WLAN_FC_STYPE_PROBE_REQ) nl_handle=0xcfd5d8 match=
[  495.443813] IPv6: ADDRCONF(NETDEV_UP): wlan1: link is not ready
nl80211: Add own interface ifindex 9
nl80211: if_indices[16]: 9
phy: phy1
BSS count 1, BSSID mask 00:00:00:00:00:00 (0 bits)
nl80211: Regulatory information - country=00
nl80211: 2402-2472 @ 40 MHz 20 mBm
nl80211: 2457-2482 @ 40 MHz 20 mBm (no IR)
nl80211: 2474-2494 @ 20 MHz 20 mBm (no OFDM) (no IR)
nl80211: 5170-5250 @ 160 MHz 20 mBm (no IR)
nl80211: 5250-5330 @ 160 MHz 20 mBm (DFS) (no IR)
nl80211: 5490-5730 @ 160 MHz 20 mBm (DFS) (no IR)
nl80211: Added 802.11b mode based on 802.11g information
Completing interface initialization
Mode: IEEE 802.11g  Channel: 9  Frequency: 2452 MHz
DFS 0 channels required radar detection
nl80211: Set freq 2452 (ht_enabled=0, vht_enabled=0, bandwidth=20 MHz, cf1=2452 MHz, cf2=0 MHz)* freq=2452* vht_enabled=0* ht_enabled=0
RATE[0] rate=10 flags=0x1
RATE[1] rate=20 flags=0x1
RATE[2] rate=55 flags=0x1
RATE[3] rate=110 flags=0x1
RATE[4] rate=60 flags=0x0
RATE[5] rate=90 flags=0x0
RATE[6] rate=120 flags=0x0
RATE[7] rate=180 flags=0x0
RATE[8] rate=240 flags=0x0
RATE[9] rate=360 flags=0x0
RATE[10] rate=480 flags=0x0
RATE[11] rate=540 flags=0x0
hostapd_setup_bss(hapd=0xcfcf50 (wlan1), first=1)
wlan1: Flushing old station entries
nl80211: flush -> DEL_STATION wlan1 (all)
wlan1: Deauthenticate all stations
nl80211: send_mlme - da= ff:ff:ff:ff:ff:ff noack=0 freq=0 no_cck=0 offchanok=0 wait_time=0 fc=0xc0 (WLAN_FC_STYPE_DEAUTH) nlmode=3
nl80211: send_mlme -> send_frame
nl80211: send_frame - Use bss->freq=2452
nl80211: send_frame -> send_frame_cmd
nl80211: Frame command failed: ret=-16 (Device or resource busy) (freq=2452 wait=0)
wpa_driver_nl80211_set_key: ifindex=9 (wlan1) alg=0 addr=(nil) key_idx=0 set_tx=0 seq_len=0 key_len=0
wpa_driver_nl80211_set_key: ifindex=9 (wlan1) alg=0 addr=(nil) key_idx=1 set_tx=0 seq_len=0 key_len=0
wpa_driver_nl80211_set_key: ifindex=9 (wlan1) alg=0 addr=(nil) key_idx=2 set_tx=0 seq_len=0 key_len=0
wpa_driver_nl80211_set_key: ifindex=9 (wlan1) alg=0 addr=(nil) key_idx=3 set_tx=0 seq_len=0 key_len=0
Using interface wlan1 with hwaddr 94:0c:6d:7c:12:f6 and ssid "TonyOpenWRT"
Deriving WPA PSK based on passphrase
SSID - hexdump_ascii(len=11):54 6f 6e 79 4f 70 65 6e 57 52 54                  TonyOpenWRT     
PSK (ASCII passphrase) - hexdump_ascii(len=12): [REMOVED]
PSK (from passphrase) - hexdump(len=32): [REMOVED]
random: Got 15/20 bytes from /dev/random
random: Only 15/20 bytes of strong random data available from /dev/random
random: Not enough entropy pool available for secure operations
WPA: Not enough entropy in random pool for secure operations - update keys later when the first station connects
GMK - hexdump(len=32): [REMOVED]
Key Counter - hexdump(len=32): [REMOVED]
WPA: Delay group state machine start until Beacon frames have been configured
nl80211: Set beacon (beacon_set=0)
nl80211: Beacon head - hexdump(len=62): 80 00 00 00 ff ff ff ff ff ff 94 0c 6d 7c 12 f6 94 0c 6d 7c 12 f6 00 00 00 00 00 00 00 00 00 00 64 00 11 04 00 0b 54 6f 6e 79 4f 70 65 6e 57 52 54 01 08 82 84 8b 96 0c 12 1y01 09
nl80211: Beacon tail - hexdump(len=65): 2a 01 04 32 04 30 48 60 6c 30 14 01 00 00 0f ac 02 01 00 00 0f ac 04 01 00 00 0f ac 02 00 00 dd 16 00 50 f2 01 01 00 00 50 f2 02 01 00 00 50 f2 02 01 00 00 50 f2 02 7f 08 00
nl80211: ifindex=9
nl80211: beacon_int=100
nl80211: dtim_period=2
nl80211: ssid - hexdump_ascii(len=11):54 6f 6e 79 4f 70 65 6e 57 52 54                  TonyOpenWRT     * beacon_int=100
nl80211: hidden SSID not in use
nl80211: privacy=1
nl80211: auth_algs=0x1
nl80211: wpa_version=0x3
nl80211: key_mgmt_suites=0x2
nl80211: pairwise_ciphers=0x18
nl80211: group_cipher=0x8
nl80211: SMPS mode - off
nl80211: beacon_ies - hexdump(len=10): 7f 08 00 00 00 00 00 00 00 40
nl80211: proberesp_ies - hexdump(len=10): 7f 08 00 00 00 00 00 00 00 40
nl80211: assocresp_ies - hexdump(len=10): 7f 08 00 00 00 00 00 00 00 40
WPA: Start group state machine to set initial keys
WPA: group state machine entering state GTK_INIT (VLAN-ID 0)
GTK - hexdump(len=32): [REMOVED]
WPA: group state machine entering state SETKEYSDONE (VLAN-ID 0)
wpa_driver_nl80211_set_key: ifindex=9 (wlan1) alg=2 addr=0x8dc74 key_idx=1 set_tx=1 seq_len=0 key_len=32
nl80211: KEY_DATA - hexdump(len=32): [REMOVED]broadcast key
nl80211: Set wlan1 operstate 0->1 (UP)
netlink: Operstate: ifindex=9 linkmode=-1 (no change), operstate=6 (IF_OPER_UP)
wlan1: interface state UNINITIALIZED->ENABLED
wlan1: AP-ENABLED 
wlan1: Setup of interface done.
ctrl_iface not configured!


其中里面用到了加密时候需要的随机数，这个是使用内核中的random节点来生成的，所以我们在内核这两个还需要添加random驱动。

验证AP

在验证之前，我们需要给我们的这个网卡自己一个IP地址， 因此直接：

root@(none):/# ifconfig wlan1 192.168.2.1  
root@(none):/# ifconfig wlan1
wlan1     Link encap:Ethernet  HWaddr 94:0c:6d:7c:12:f6  inet addr:192.168.2.1  Bcast:192.168.2.255  Mask:255.255.255.0inet6 addr: fe80::960c:6dff:fe7c:12f6/64 Scope:LinkUP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1RX packets:131 errors:0 dropped:0 overruns:0 frame:0TX packets:106 errors:0 dropped:0 overruns:0 carrier:0collisions:0 txqueuelen:1000 RX bytes:20162 (19.6 KiB)  TX bytes:20954 (20.4 KiB)


然后我们打开手机查看是否可以看到我们的热点：


然后我们使用手动配置IP方式连接热点


连接完成后， 我们可以看到已经连接的状态：


然后在手机上面可以Ping一把确定是否连通：


然后在iMX6UL上面也来ping一把这个手机：

root@(none):/# ping 192.168.2.111
PING 192.168.2.111 (192.168.2.111): 56 data bytes
64 bytes from 192.168.2.111: seq=0 ttl=64 time=90.952 ms
64 bytes from 192.168.2.111: seq=1 ttl=64 time=143.093 ms
64 bytes from 192.168.2.111: seq=2 ttl=64 time=4.508 ms
64 bytes from 192.168.2.111: seq=3 ttl=64 time=46.859 ms
64 bytes from 192.168.2.111: seq=4 ttl=64 time=2.539 ms
64 bytes from 192.168.2.111: seq=5 ttl=64 time=2.697 ms
^C
--- 192.168.2.111 ping statistics ---
6 packets transmitted, 6 packets received, 0% packet loss
round-trip min/avg/max = 2.539/48.441/143.093 ms


至此，基本的连接就好了， 如果想要自动获取IP地址，那么我们还需要配置DHCPD。


wpa_supplicant的配置

前面我们使用其中一个网卡，配置好了AP， 接下来我们使用另外一个Wireless作为上一级router的接入设备。 这个可以参考我以前写的文章: Yocto i.MX6 (TQIMX6) (03) : wifi网卡的相关工具wpa_supplicant与dhcp
但是wpa_supplicant的版本升级后， 和hostapd的使用基本一致起来， 因此以前的使用方法稍微有些不同了。
配置

创建一个配置文件，写明需要接入的SSID，密码等信息：

root@(none):/# cat /etc/wpa.conf 
ctrl_interface=/var/run/wpa_supplicant
network={ssid="XXX"proto=WPAkey_mgmt=WPA-PSKpairwise=TKIPgroup=TKIPpsk="PASSWORD"
}


其中XXX是路由器的热点名字， psk填写Password， 加密方式TKIP需要根据实际情况填写，不明白可以看我以前的文章，或者直接man。

运行

先创建一个runtime目录：

root@(none):/# mkdir /var/run/wpa_supplicant


然后直接在后台运行：

root@(none):/# wpa_supplicant -B -iwlan0 -c /etc/wpa.conf 
Successfully initialized wpa_supplicant
rfkill: Cannot open RFKILL contro[  172.506937] ieee80211 phy0: rt2x00lib_request_firmware: Info - Loading firmware file 'rt2870.bin'
l device
[  172.527112] ieee80211 phy0: rt2x00lib_request_firmware: Info - Firmware detected - version: 0.29
[  173.083412] IPv6: ADDRCONF(NETDEV_UP): wlan0: link is not ready
root@(none):/# [  174.937567] wlan0: authenticate with bc:d1:77:a0:4f:3c
[  175.073446] wlan0: send auth to bc:d1:77:a0:4f:3c (try 1/3)
[  175.082318] wlan0: authenticated
[  175.087051] rt2800usb 1-1.2.1:1.0 wlan0: disabling HT/VHT due to WEP/TKIP use
[  175.098735] wlan0: associate with bc:d1:77:a0:4f:3c (try 1/3)
[  175.108018] wlan0: RX AssocResp from bc:d1:77:a0:4f:3c (capab=0x411 status=0 aid=5)
[  175.129960] IPv6: ADDRCONF(NETDEV_CHANGE): wlan0: link becomes ready
[  175.136678] wlan0: associated
[  176.069997] IPv6: wlan0: IPv6 duplicate address fe80::7edd:90ff:feb2:73b1 detected!

验证

使用wpa_cli看查看连接的状态

root@(none):/# wpa_cli status
Selected interface 'wlan0'
bssid=bc:d1:77:a0:4f:3c
freq=2412
ssid=XXX
id=0
mode=station
pairwise_cipher=TKIP
group_cipher=TKIP
key_mgmt=WPA-PSK
wpa_state=COMPLETED
address=7c:dd:90:b2:73:b1
uuid=1e82e362-f927-544e-aa4d-1a229533944c

status为COMPLETED那么表示连接上了， 然后我们获取IP地址， 并尝试ping国内的某个网站：

root@(none):/# udhcpc -i wlan0
udhcpc (v1.24.1) started
Sending discover...
Sending select for 192.168.1.112...
Lease of 192.168.1.112 obtained, lease time 86400
/etc/udhcpc.d/50default: Adding DNS 192.168.1.1
/etc/udhcpc.d/50default: Adding DNS 0.0.0.0root@(none):/# ping meitu.com
PING meitu.com (42.62.69.156): 56 data bytes
64 bytes from 42.62.69.156: seq=0 ttl=42 time=44.499 ms
64 bytes from 42.62.69.156: seq=1 ttl=42 time=44.956 ms
64 bytes from 42.62.69.156: seq=2 ttl=42 time=43.122 ms
64 bytes from 42.62.69.156: seq=3 ttl=42 time=43.387 ms
64 bytes from 42.62.69.156: seq=4 ttl=42 time=46.467 ms
64 bytes from 42.62.69.156: seq=5 ttl=42 time=46.267 ms
64 bytes from 42.62.69.156: seq=6 ttl=42 time=43.843 ms
^C
--- meitu.com ping statistics ---
7 packets transmitted, 7 packets received, 0% packet loss
round-trip min/avg/max = 43.122/44.648/46.467 ms

至此， 我们就完成了基本的配置工作。 