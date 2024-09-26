我们有一台linux服务器，它有一个无线网卡连接了互联网，还有一个有线网卡，有线网卡网口通过网线和另一台linux服务器B连接, 服务器B没有互联网．所以我们想把服务器A的互联网共享给服务器B使用．

经过看了几篇其他老师分享的文章，整理后的思路就是：先把服务器A的无线网共享给服务器Ａ的有限网卡，然后打开服务器A的内核ＩＰ转发功能；再就是把服务器Ｂ的网卡ｉｐ设置成和服务器的有线网卡ｉｐ在一个网段，网关设置成服务器Ａ的有限网卡ｉｐ．接下来记录一下具体步骤：
步骤一：设置服务器A的无线网共享给服务器Ａ的有限网卡

打开ｗｉｆｉ网络设置

nm-connection-editor

然后点击左下角的＂＋＂，设置连接名称，以太网选项页中设备选择网卡编号

然后在ipv4设置页面中的方法设置为与其它计算机共享，然后点击保存．到这步，完成了无线网共享给有线网卡．

步骤二：打开服务器A的内核IP转发功能

    #  echo 1 > /proc/sys/net/ipv4/ip_forward      // 打开ip转发
     
    #  iptables -F
     
    #  iptables -P INPUT ACCEPT
     
    #  iptables -P FORWARD ACCEPT
     
    # iptables -t nat -A POSTROUTING -o wlp2s0 -j MASQUERADE     （wlp2s0为A接外网的无线网卡）

步骤三：设置服务器B的网卡的ｉp和网关

设置ｉp和网管可以分为永久和暂时：因为ubuntu和centos的用就设置网卡ｉｐ的配置文件路径不一致：，就说一下临死的设置方案：

    ifconfig 网卡名　ip/24   # 设置网卡ｉp和服务器A的有限网卡ｉｐ在一个网段
    route add -net 0.0.0.0/0 gw 服务器A的有限网卡ＩＰ　　＃　设置服务器Ａ的有限网卡ｉｐ为服务器Ｂ的网关

设置完成后，可以ping www.baidu.com , 看一下网络是否可用了
————————————————
版权声明：本文为CSDN博主「dogheader」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/dogheader/article/details/128394561








Linux双网卡NAT共享上网
linux双网卡NAT共享上网

术语字汇

    私有IP地址（路由不可达地址）是一个被用于本地局域网的IP地址（在互联网中不可见）。
    公用IP地址（路由可达地址）是一个在互联网中可见的IP地址。
    IP伪装是一项允许一系列机器通过MASQ网关连接互联网的功能。这些MASQ网关之外的机器在互联网中是不可见的。MASQ之后的机器中任何流入或流出的数据必须经过MASQ网关。
    网络地址转换（NAT）是一项通过IP伪装技术可以使私有IP地址访问互联网的功能。

硬件要求

    一台有两个接口（一个公有IP地址和其他的私有IP地址）的linux主机，这个主机将被用作网关。
    一台或者多台拥有私有IP地址的linux/windows系统的PC或者笔记本。
    交换机/集线器（可选）。

服务器设置：
1、激活IP转发
为了设置网络共享，你需要在linux主机上更改一个内核参数来使能IP转发功能。内核启动参数设定在/etc/sysctl.conf文件中。打开这个文件，定位到含有"# net.ipv4.ip_forward = 0"的这一行，移除#号（即取消注释），然后将其值设置为1。
你还要使激活IP转发功能生效，通过执行下面的命令：

sysctl -w net.ipv4.ip_forward=1
sysctl -p

或使用下面的命令临时生效，重启后失效

echo "1" > /proc/sys.Net/ipv4/ip_forward #打开IP转发，1表示转发；0表示不转发

2、NAT配置
另一个网络共享的重要部分是NAT配置，这可以通过使用iptables的命令，iptables包含四个防火墙的规则表：

    FILTER (默认表格)
    NAT
    MANGLE
    RAW

这个教程中我们将仅使用两个表格：FILTER和NAT表格。
首先，刷新所有活跃的防火墙的规则。

iptables -X
iptables -F
iptables -t nat -X
iptables -t nat -F

在输入表格中，你需要设置转发链（FORWARD）成可接受的（ACCEPT）目的地，因此所有通过主机的数据包将会被正确的处理。

iptables -I INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
iptables -I FORWARD -m state --state RELATED,ESTABLISHED -j ACCEPT

在NAT表中，你必须为你的WAN口启用IP伪装功能，我们假设WAN口协议是ppp0。为了在ppp0接口上使能IP伪造技术，我们使用以下的命令：

iptables -t nat -I POSTROUTING -o ppp0 -j MASQUERADE

或使用下面简单的命令是：

iptables -F
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE #打开NAT

网络拓补结构：(192.168.1.0/24可以连接Internet，网关192.168.1.1)
1、NAT方法：
服务器：两网卡的设置：
eth0:192.168.1.10 掩码：255.255.255.0 网关：192.168.1.1 #可以连接Internet相联
eth1:192.168.2.1 掩码：255.255.255.0 #与局域网相联
客户机：192.168.2.10/24 网关：192.168.2.1 #与eth1同一网段,网关为eht1
2、网桥方法：
eth0:0.0.0.0 #无IP -->br0
eth1:0.0.0.0 #无IP -->br0
br0:eth0:192.168.1.10 掩码：255.255.255.0 网关：192.168.1.1
客户机：192.168.1.10/24

ifconfig eth0 0.0.0.0 up #网桥工作于数据链路层，不需要IP
ifconfig eth1 0.0.0.0 up
brctl addbr br0 #添加一个网桥
brctl addif br0 eth0
brctl addif br0 eth1 #将eth0 eth1加到网桥中去
ifconfig br0 192.168.1.10.Netmask 255.255.255.0 broadcast 192.168.1.255 up #给网桥分配IP地址，如果不做NAT，也不需要远程访问网桥的话，可以不设IP地址，但必须启用网桥（ifconfig br0 up）
route add default gw 192.168.1.1 #如果网桥不需要访问其他机器的话，可以不添加网关。

配置感想
按照NAT的方法，很快配置完成，并且可以顺利上网。对于那些对于网络配置没什么特殊要求的用户，可以选用NAT方法，简单实用。

另外，无线局域网共享wifi，需要hostapd搭建无线局域网，可以参考creat_ap.
https://www.bbsmax.com/A/6pdDLp7Xdw/



通过 socks5 进行全局共享上网
tun2socks 是一个将虚拟网卡的流量转换成 socks 的工具，也就是 socks vpn 好处是可以进行全局代理上网，任何程序都可以值接使用这个工具进行代理上网。
https://github.com/xjasonlyu/tun2socks.git


linux：
Create TUN interface tun0 and assign an IP address for it.
```bash
ip tuntap add mode tun dev tun0
ip addr add 198.18.0.1/15 dev tun0
ip link set dev tun0 up
```
Configure the default route table with different metrics. Let's say the primary interface is eth0 and gateway is 172.17.0.1.
```bash
ip route del default
ip route add default via 198.18.0.1 dev tun0 metric 1
ip route add default via 172.17.0.1 dev eth0 metric 10
```
tart tun2socks and bind it to the primary interface.
```bash
tun2socks -device tun0 -proxy socks5://host:port -interface eth0
```
socks 服务器尽量不要在本地 127.0.0.1, 不然会造成流量回环，从而不能使用。

windows:
To use it in windows, download wintun to the tun2socks folder or the system PATH and start the program.

    In this example, "WIFI" is the default primary network interface.
```bash
tun2socks -device wintun -proxy socks5://host:port -interface "WIFI"
```
Same as macOS version, but we don't need to bring up the interface by hand, the only thing we need is to assign an IP address to it.
```bash
netsh interface ipv4 set address name="wintun" source=static addr=192.168.123.1 mask=255.255.255.0
```
In Windows, we usually need to manually set up the DNS address for our interface.
```bash
netsh interface ipv4 set dnsservers name="wintun" static address=8.8.8.8 register=none validate=no
```
Then route default traffic to TUN interface.
```bash
netsh interface ipv4 add route 0.0.0.0/0 "wintun" 192.168.123.1 metric=1
```



```bash
echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -F
iptables -t nat -A POSTROUTING -o veth2 -j RETURN
iptables -t nat -A POSTROUTING -o tun0 -j MASQUERADE
nohup ./tun2socks-linux-arm64 -device tun0 -proxy socks5://192.168.28.21:7890 -interface veth2 &> /dev/null &
```

网络操作
```bash
# 1. 创建网络命名空间
sudo ip netns add mynamespace

# 2. 创建网络对
sudo ip link add veth0 type veth peer name veth1

# 3. 将网络对的一端放入网络命名空间
sudo ip link set veth1 netns mynamespace

# 4. 在网络命名空间中配置网络接口
sudo ip netns exec mynamespace ip addr add 10.0.0.2/24 dev veth1
sudo ip netns exec mynamespace ip link set veth1 up

# 5. 创建网桥
sudo ip link add name br0 type bridge
sudo ip link set br0 up


# 6. 将网络对的另一端连接到网桥
sudo ip link set veth0 master br0
sudo ip link set veth0 up

# 7. 配置网桥的IP地址
sudo ip addr add 10.0.0.1/24 dev br0

# 8. 删除网桥
sudo ip link delete br0 type bridge
sudo ip link delete eth0

# 9. 删除网络命名空间
sudo ip netns delete mynamespace

# 10. 将网口eth0从网桥上断开
ip link set eth0 nomaster



```


## 构建一个小旁路由网关

需要：
一个能够运行 clash 和 tun2socks 的 linux 设备，Linux 设备最好要有两个网卡,方便进行网络的转发。

一个网卡为 eth0 192.168.28.21,用作主出口上网，一个网卡为 eth1 192.168.24.1 接收局域网流量。

在 Linux 设备上：
```bash
# 创建网络命名空间
sudo ip netns add natnamespace

# 创建虚拟网络对
sudo ip link add veth0 type veth peer name vethout

# 将 eth1 和 vethout 移动到网络命名空间内
sudo ip link set eth1 netns natnamespace
sudo ip link set vethout netns natnamespace

# 设置主空间内的 veth0 ip
sudo ip addr add 192.168.27.1/24 dev veth0

# 设计命名空间内的网口 ip 并开启 网口
sudo ip netns exec natnamespace ip addr add 192.168.27.2/24 dev vethout
sudo ip netns exec natnamespace ip link set vethout up

sudo ip netns exec natnamespace ip addr add 192.168.24.1/24 dev eth1
sudo ip netns exec natnamespace ip link set eth1 up

# 添加 tun 接口，用作网口流量转 socket5
sudo ip netns exec natnamespace ip tuntap add mode tun dev tun0
sudo ip netns exec natnamespace ip addr add 198.18.0.1/15 dev tun0
sudo ip netns exec natnamespace ip link set dev tun0 up
sudo ip netns exec natnamespace ip route add default via 198.18.0.1 dev tun0 metric 1

# 开启 ipv4转发
sudo ip netns exec natnamespace sysctl -w net.ipv4.ip_forward=1

# 设置网口 eth1 192.168.24.1 接受到的流量 nat 转发到 tun0 内
sudo ip netns exec natnamespace iptables -t nat -A POSTROUTING -s 192.168.24.0/24 -o tun0 -j MASQUERAD

# 开启 tun 转 sockets5 程序
sudo ip netns exec natnamespace nohup ./tun2socks-linux-arm64 -device tun0 -proxy socks5://192.168.27.1:7890 -interface vethout &> /dev/null &





## 关闭程序
sudo ip netns exec natnamespace kill `ps aux | grep tun2socks | awk '{print $1}'`

# 清除 nat 转发
sudo ip netns exec natnamespace iptables -t nat -D POSTROUTING -s 192.168.24.0/24 -o tun0 -j MASQUERAD

# 清除内核 ipv4 转发
sudo ip netns exec natnamespace sysctl -w net.ipv4.ip_forward=0

# 删除网口
sudo ip netns exec natnamespace ip link delete tun0
sudo ip netns exec natnamespace ip link delete vethout

# 将 eth1 移动到默认网络空间
sudo ip netns exec natnamespace ip link set eth1 netns 1

# 删除命名空间
sudo ip netns delete natnamespace

```

局域网设备使用该网关地址:
在设备上：
```bash
sudo ip route flush all
sudo ip link set eth0 down
sudo ip addr add 192.168.24.100/24 dev eth0
sudo ip link set eth0 up
sudo ip route add default via 192.168.24.1
# or
sudo ip route flush all
sudo ifconfig eth0 down
sudo ifconfig eth0 192.168.24.100
sudo ifconfig eth0 up
sudo ip route add default via 192.168.24.1

```





要将所有来自 `192.168.24.1` 通过 `eth1` 网口的 TCP 流量重定向到 `192.168.27.1:7892`，你可以使用 `iptables` 的 `REDIRECT` 目标。以下是具体的命令：

```bash
iptables -t nat -A PREROUTING -p tcp -d 192.168.24.1 --dport 1:65535 -i eth1 -j DNAT --to-destination 192.168.27.1:7892
```

解释：
- `-t nat`：指定使用 NAT 表。
- `-A PREROUTING`：在 PREROUTING 链中添加规则。
- `-p tcp`：指定协议为 TCP。
- `-d 192.168.24.1`：指定目标 IP 地址为 `192.168.24.1`。
- `--dport 1:65535`：指定目标端口范围为 1 到 65535（即所有端口）。
- `-i eth1`：指定流入接口为 `eth1`。
- `-j DNAT`：指定目标为 DNAT（目标网络地址转换）。
- `--to-destination 192.168.27.1:7892`：指定重定向的目标地址和端口。

这样，所有从 `192.168.24.1` 通过 `eth1` 网口的 TCP 流量都会被重定向到 `192.168.27.1:7892`。





















