DNS(Domain Name System) 是一个为计算机、服务器或其他任何需要接入互联网或私有网络提供分级域名分发系统(hierarchical and decentralized naming system)。

在内网中又一台服务器提供服务，为了使用使其可以在公共网络中访问到需要为它分配 ip 或者使用端口转发、内网穿透等，但绑定域名后域名解析到公网ip，如果在内网访问的话就写多此一举。因此在内网自建一台 DNS 服务器，使其优先级高于公网域名解析服务器，进而实现内网访问解析为内网 ip 且不影响外网访问的目的。

DNSMasq 主要用来解决内网 DNS 域名缓存、DHCP、网络启动和路由通告功能，本文目的是将 DNSMasq 作为内网 DNS 使用。

1.安装 DNSMasq

# Ubuntu/Debian
$ sudo apt update
$ sudo apt install -y dnsmasq

# Centos
$ yum install dnsmasq -y 
$ service dnsmasq start

2.修改配置

$ sudo cp /etc/dnsmasq.conf /etc/dnsmasq.conf.bak
$ sudo vim /etc/dnsmasq.conf
....
resolv-file=/etc/resolv.conf
strict-order
listen-address=<host-ip>
addn-hosts=/etc/hosts.dnsmasq

修改内容解释：

    resolv-file：从文件读取 DNSMasq 上游的 DNS 服务器配置。
    strict-order：resolv-file 文件中如果指定了多个 DNS 服务器，严格安装 DNS 服务器的先后顺序查询域名。
    listen-address：监听地址，如果你不想所有用户都使用你的DNS服务，可以在listen-address后面加上你指定的IP地址即可。
    addn-hosts：从文件读取本地 DNS 域名和 IP 的对应关系，格式为 <IP> <Domain name>。其实可以把 IP 和域名的对应关系写在 /etc/hosts 文件中，DNSMasq 默认从那里读取，但如果要支持一个域名对应多个 IP，就必须使用 addn-hosts 选项了。

/etc/hosts.dnsmasq 文件内容如下：

$ cat /etc/hosts.dnsmasq
10.0.0.1 blackpiglet.com
10.0.0.2 blackpiglet.com
10.0.0.3 blackpiglet.com

修改完成后重启 DNSMasq

$ sudo systemctl restart dnsmasq.service

3.配置Dnsmasq上游DNS服务器

DNSMasq 是从 /etc/resolv.conf 文件中读取上游 DNS 服务器, 编辑 /etc/resolv.conf ，参考如下：

nameserver 223.5.5.5
nameserver 223.4.4.4

4.启动Dnsmasq并加入启动项（Centos）

chkconfig dnsmasq on
/etc/init.d/dnsmasq restart

5.使用

在需要内网解析的服务器上修改网络设置中的DNS服务器即可。








不晓得为撒，用网上的一些公共DNS服务的时候，总是莫名其妙的有些网站无法解析，有时候114能解析，阿里DNS不行或者腾讯DNS不行，导致总是来回切换DNS，很是烦心。

于是就想着自己搭建一个DNS服务会不会好一点？网上搜了一下，好像很复杂的样子，一直就没动手，但是今天试了下，发现出奇的简单，体验了一番，感觉效果良好。

首先你要有一个VPS，质量不用太好，我在我美国的64M内存的VPS上面完美安装，看了下内存32M应该也是可以的。

如果你对解析速度要求比较高，建议是用亚洲的VPS，甚至用国内VPS中转也是可以的，但是成本就略高了点，还是根据自己需求DIY吧。
安装Dnsmasq

yum install dnsmasq -y 
service dnsmasq start 

配置Dnsmasq

Dnsmasq的配置文件是 /etc/dnsmasq.conf，编辑成如下格式：

需要注意的一点是，检查一下no-hosts前面是不是已经有了#号，默认的情况下是有的，dnsmasq 会首先寻找本地的 hosts 文件再去寻找缓存下来的域名, 最后去上游dns 服务器寻找。

一般搭建DNS服务，可以允许所有用户使用你的DNS服务，即listen-address默认注释掉即可，如果你不想所有用户都使用你的DNS服务，可以在listen-address后面加上你指定的IP地址，如：

listen-address=listen-address=192.168.1.123,127.0.0.1
修改Dnsmasq上游DNS服务器

编辑 /etc/resolv.conf ，参考如下：

nameserver 8.8.8.8
nameserver 4.4.4.4

启动Dnsmasq并加入启动项

chkconfig dnsmasq on
/etc/init.d/dnsmasq restart

执行 netstat -tunlp|grep 53 可以查看Dnsmasq是否有正常启动

剩下的就是修改你本地的DNS就可以了。
————————————————
版权声明：本文为CSDN博主「程序人生_life of code」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_44037713/article/details/84936362



dns查询命令 nslookup www.baidu.com