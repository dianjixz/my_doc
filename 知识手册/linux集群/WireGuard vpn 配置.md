什么是WireGuard？

WireGuard是由Jason Donenfeld等人用C语言编写的开源VPN解决方案，代码只有4000多行，Linux之父Linus Torvalds评价它是Work of art。截止2020年1月，WireGuard已经合并到Linux5.6内核中，大多数Linux版本可以做到开箱即用。
为什么是WireGuard？

就个人使用体验来说，Wireguard具有低延迟、强加密、低系统占用的特点。
需求分析？

家庭电信宽带申请到了公网IP地址，但如果在外地想访问家庭网络，或者多个无公网的计算机需要互相访问，可以考虑使用WireGuard组网。
需要什么？

一台有公网IP的服务器

其他需要组网（互相访问，但没有公网）的机器
如何开始？

关于WireGuard的配置一般的宣传都声称WireGuard具有配置简单，但就个人体验来说，WireGuard的配置过程，对新手来说并不友好，其中涉及到Linux基本操作、iptables防火墙规则、计算机网络相关等知识，这对于一些小白来说并不友好。

所以我推荐在公网服务器端用Github上的一键安装脚本：

https://github.com/angristan/wireguard-install

脚本下载完成后，（root权限）首次执行需要自己配置公网的网络接口（一般为eth0）、公网IP地址（或域名）、VPN网段（默认为：10.66.66.0/24）等配置，其他配置可以保持默认即可，服务端的配置文件在/etc/wireguard/wg0.conf

再次执行脚本可以进行添加用户、移除用户、移除WireGuard等操作。添加用户操作，指定用户名和IP地址（VPN网段地址）后，会在脚本目录生成对应端的配置文件，命令行执行ls，会有名为wg0-client-username.conf配置文件，在下一步我们需要用到这个文件的内容。

在非公网端，根据操作系统的不同，可能需要安装的包不同，例如Debian系可能需要检查是否有这几个包：wireguard-tools  对于内核版本低于5.6的Linux可能还需要wireguard-dkms这个包，包依赖安装好后，使用root权限在/etc/wireguard下创建wg0.conf文件，将公网端脚本目录下的配置文件内容（wg0-client-username.conf）写入此文件。
如何进一步配置？

假设有PeerA，PeerB、PeerC三台服务器，其中PeerA为公网，B、C无公网

PeerA的/etc/wireguard/wg0.conf配置内容一般如下：（上面的脚本会自动生成，一般不需要修改！！！）
```bash
[Interface]

Address = 10.66.66.1/24,fd42:42:42::1/64

ListenPort = 10086

PrivateKey = PeerA私钥，脚本会自动生成

PostUp = iptables -A FORWARD -i eth0 -o wg0 -j ACCEPT; iptables -A FORWARD -i wg0 -j ACCEPT; iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE; ip6tables -A FORWARD -i wg0 -j ACCEPT; ip6tables -t nat -A POSTROUTING -o eth0 -j MASQUERADE   ###启动wg0，防火墙操作，由脚本自动生成

PostDown = iptables -D FORWARD -i eth0 -o wg0 -j ACCEPT; iptables -D FORWARD -i wg0 -j ACCEPT; iptables -t nat -D POSTROUTING -o eth0 -j MASQUERADE; ip6tables -D FORWARD -i wg0 -j ACCEPT; ip6tables -t nat -D POSTROUTING -o eth0 -j MASQUERADE    ###关闭wg0时，防火墙的操作，由脚本自动生成
```
 

### Client PeerB
```bash
[Peer]

PublicKey = PeerB公钥，脚本自动生成

PresharedKey = PeerB预共享密钥，脚本自动生成

AllowedIPs = 10.66.66.2/32,fd42:42:42::2/128

 

### Client PeerC

[Peer]

PublicKey =PeerC公钥，脚本自动生成

PresharedKey = PeerC预共享密钥，脚本自动生成

AllowedIPs = 10.66.66.3/32,fd42:42:42::3/128
```
PeerB的/etc/wireguard/wg0.conf配置内容一般如下：（可以在PeerA脚本的目录下wg0-client-PeerBname.conf复制过来）
```bash
[Interface]

PrivateKey = PeerB私钥，脚本自动在公网端生成

Address = 10.66.66.2/32,fd42:42:42::2/128

DNS = 114.114.114.114,94.140.15.15

 

[Peer]

PublicKey = PeerA公钥，脚本自动生成

PresharedKey = 预共享密钥，脚本自动生成

Endpoint = Peer公网IP或域名:10086

AllowedIPs = 0.0.0.0/0,::/0
```
PeerC的/etc/wireguard/wg0.conf配置内容一般如下：（可以在PeerA脚本的目录下wg0-client-PeerCname.conf复制过来）
``` bash
[Interface]

PrivateKey = PeerC公钥，脚本自动在公网端生成

Address = 10.66.66.2/32,fd42:42:42::2/128

DNS = 114.114.114.114,94.140.15.15

 

[Peer]

PublicKey = PeerA私钥，脚本自动生成

PresharedKey = 预共享密钥，脚本自动生成

Endpoint = Peer公网IP或域名:10086

AllowedIPs = 0.0.0.0/0,::/0
```
 

下面我们从下面这个例子来理解AllowedIPs选项：

假设B现在在请求112.154.12.141这个IP，因为B的Peer配置中AllowIPs为0.0.0.0/0，所以这个流量会被转发给A,而A中Peer配置没有匹配的AllowedIPs,所以这个请求会被A处理，A将处理结果返回给B；如果B访问的IP为10.66.66.3，B将流量发给A,A的Peer配置中有匹配的AllowIPs,即C的配置，所以A会把处理丢给C,这样就实现了B到C的访问，到这里，你应该对AllowedIPs这个配置选项有了一定的认识。我们不难发现如果只是想实现B到C或者C到B的访问，没有必要将AllowedIPs配置为0.0.0.0/0,只需要配置为对方的IP地址就可以实现。

如果我们想在B的局域网网段192.168.22.0/24访问C的局域网网段192.168.33.0/24配置如下：

在B的Peer中AllowedIPs添加192.168.33.0/24，在A的Peer的C配置AllowIPs处添加192.168.33.0/24,这样就能实现B访问C的局域网网段

另外还有一个很重要的配置，虽然这个配置选项违背了WireGuard低资源消耗的初衷，但是对于NAT后面的网络（没有公网IP）是非常重要的，即在PeerB和PeerC现有/etc/wireguard/wg0.conf配置后面添加

PersistentKeepalive = 25

这个配置是为了保活配置，否则你会遇到在刚配置的一段时间互相访问没有问题，而一段时间后，PeerB和PeerC能ping通PeerA，但是PeerA不能ping通PeerB和PeerC,以及PeerB、PeerC之间不能互相访问的尴尬局面。

上述配置完成后可以在PeerB和PeerC执行  sudo wg-quick up wg0   启动WireGuard

以及可以通过  sudo  systemctl enable wg-quick@wg0   添加开机自启动