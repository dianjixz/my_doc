前言
Top

BIND是美国伯克利大学在LINUX平台上开发的一个开源DNS域名解析软件，因为DNS这个域名解析服务用的地方还是比较多的比如说在域控和目录服务方面都必须用到域名解析DNS服务。这里我们用的BIND是域名DNS解析非常常用的开源服务，在全球的90%以上的DNS服务都是采用的BIND，下面来给大家介绍下如何在树莓派上配置一个DNS服务的BIND的软件。
树莓派安装配置BIND域名解析DNS服务

安装并配置好BIND服务
Top

#sudo apt-get install bind
在线安装后开始配置
配置文件
==chroot/etc/named.conf========================
options {
directory "/var/named";
forwarders {202.96.134.133;};
};


zone "bytest.com"{
type master;
file "bytest.com.byPOS";
};
zone "1.168.192.in-addr.arpa"{
type master;
file "bytest.com.byREV";
};


开启正向解析把域名解析到IP地址。
chroot/var/named/bytest.com.byPOS正向解析域名到IP
$TTL 900
@ IN SOA bytest.com. root (1997022700 28800 14400 3600000 86400)
IN NS @
IN MX 10 mail
IN A 192.168.1.86
www IN A 192.168.1.86
mail IN A 192.168.1.86


开启BIND的反向解析，把IP解析到对应的域名。
chroot/var/named/bytest.com.byREV反向解析IP到域名
$TTL 900
@ IN SOA bytest.com root.bytest.com. (
2006022301
1H
15M
1W
1D )
IN NS bytest.com.
86 IN PTR www.bytest.com. //此处86表示IP地址最后一位
87 IN PTR mail.bytest.com. //注意87是反向解析IP地址的最后以为，如192.168.1.87


在客户端测试DNS解析服务
Top

安装host命令，设置nameserver 127.0.0.1
(192.168.1.0)：
#sudo apt-get -y install bind-utils
#service named restaert
#host -t A bytest.com
#host -t A www.nrjs.cn
#host -t A mail.bytest.com
#host 192.168.1.86
#host 192.168.1.87