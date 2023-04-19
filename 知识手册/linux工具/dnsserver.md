DNSmasq介绍

DNSmasq介绍

转载自：http://www.cnblogs.com/demonxian3/p/7472300.html#top

云主机的mac地址由物理机的dnsmasq随机分配，如果要修改mac地址需要将网卡卸载再加载

zstack的dhcp服务由谁提供
扁平网络：各物理机的dnsmasq
云路由网络：虚拟路由器的dhcp

DNSmasq是一个小巧且方便地用于配置 DNS 和 DHCP 的工具，适用于小型网络，它提供了DNS功能和可选择的DHCP功能。

它服务那些只在本地适用的域名，这些域名是不会在全球的DNS服务器中出现的。DHCP服务器和DNS服务器结合，

并且允许DHCP分配的地址能在DNS中正常解析，而这些DHCP分配的地址和相关命令可以配置到每台主机中，

也可以配置到一台核心设备中（比如路由器），DNSmasq支持静态和动态两种DHCP配置方式。

因此如果需要快速搭建一个DNS服务或者DHCP服务，那么可以使用此程序来搭建