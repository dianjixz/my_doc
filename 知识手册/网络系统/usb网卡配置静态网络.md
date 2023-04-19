遇到的问题，linux otg模拟出来的网卡的mac和网卡名字总是在变，无法有效设置ip

1、将usb设备的名字固定下来  
编辑 /etc/udev/rules.d/10-network.rules 文件，添加  
``` bash
SUBSYSTEM=="net", ACTION=="add", ATTRS{idVendor}=="12ab", ATTRS{idProduct}=="3cd4", NAME="usb"
```

然后执行  
``` bash
sudo udevadm control --reload
```

重新插拔 usb 设备就能将网卡名字固定到 usb



然后编辑 /etc/netplan/01-network-manager-all.yaml 文件，按需要添加下面。
``` bash
# Let NetworkManager manage all devices on this system
network:
  version: 2
  renderer: NetworkManager
  ethernets:
    ens33:
      dhcp4: false
      addresses:
        - 192.168.1.27/24 #虚拟机局域网IP地址，需跟window的有线网卡处于同一网段
      routes:
        - to: default
          via: 192.168.1.1 #本机局域网网关地址，需跟window的有线网卡设置相同
          metric: 200
      nameservers:
        addresses:
          - 114.114.114.114
          - 8.8.8.8
    ens37:
      dhcp4: false
      addresses:
        - 192.168.62.27/24 #与1-d步骤 NAT设置中的 网关IP处于同一网段
      routes:
        - to: default
          via: 192.168.62.2 #与1-d步骤 NAT设置中的 网关IP一致
          metric: 100
      nameservers:
        addresses:
          - 114.114.114.114
          - 8.8.8.8
```
执行  
``` bash
sudo netplan apply 
```
即可










``` bash
nihao@nihao-z690:~/work/raspi/cm4_project/ax-pipeline/build$ file /sys/class/net/*
/sys/class/net/docker0:         symbolic link to ../../devices/virtual/net/docker0
/sys/class/net/enp3s0:          symbolic link to ../../devices/pci0000:00/0000:00:1c.0/0000:03:00.0/net/enp3s0
/sys/class/net/enx00e04c680220: symbolic link to ../../devices/pci0000:00/0000:00:14.0/usb1/1-5/1-5.2/1-5.2:1.0/net/enx00e04c680220
/sys/class/net/lo:              symbolic link to ../../devices/virtual/net/lo
/sys/class/net/usb0:            symbolic link to ../../devices/pci0000:00/0000:00:14.0/usb1/1-5/1-5.1/1-5.1:1.0/net/usb0
/sys/class/net/vethd6a7a74:     symbolic link to ../../devices/virtual/net/vethd6a7a74
/sys/class/net/vethf4f83b5:     symbolic link to ../../devices/virtual/net/vethf4f83b5
/sys/class/net/wlp4s0:          symbolic link to ../../devices/pci0000:00/0000:00:1c.3/0000:04:00.0/net/wlp4s0
```





NetworkManager and netplan
简介

Ubuntu 18.04开始，Ubuntu和Debian移除了以前的ifup/ifdown命令和/etc/network/interfaces配置文件，转而使用ip link set或者/etc/netplan/01-netcfg.yaml模板和sudo netplan apply命令实现网络管理。
Netplan 是抽象网络配置描述器，用于配置Linux网络。 通过netplan命令，你只需用一个 YAML文件描述每个网络接口所需配置。netplan并不关系底层管理工具是NetworkManager还是networkd。
netplan的使用可能导致新版本Ubuntu重启后网络无法连通。
替代关系

ip link set为iproute2工具集命令，多用于临时配置，取代ifup和ifdown。
如果安装的是服务器版本，则默认使用networkd进行管理，并且不会安装NetworkManager和提供nmcli接口。
如果安装的是桌面版本，则默认使用NetworkManager进行管理。
这里的默认使用是指/etc/netplan/下的YAML配置文件中render初始指定为该网络管理工具。
如果是带有无线网络的移动设备，推荐使用NetworkManager，可以通过NetworkManager切换环境网络以及修改WiFi密码；如果是非移动的设备（或无需无线网络的虚拟机），更推荐networkd（无法管理WiFi和3/4/5G移动网络）作为后端管理工具，使用YAML描述文件+**sudo netplan apply**生成后端配置，保证网络设置的静态性。
netplan工作原理

netplan从/etc/netplan/文件夹中读取YAML配置文件，配置可以是管理员或者系统安装人员配置，可以参考该目录中的模板文件。 在系统启动阶段早期，netplan在/run目录生成好配置文件并将设备控制权交给相关后台程序。
netplan原理

Netplan目前支持以下两种网络管理工具 ：

    NetworkManager
    Systemd-networkd

在YAML文件中通过renderer关键字进行描述，networkd和NetworkManager。
一言以蔽之，从前你需要根据不同的管理工具编写网络配置，现在**Netplan**将管理工具差异性给屏蔽了。 你只需按照Netplan规范编写YAML配置，不管底层管理工具是啥，一份配置走天下！
配置

如下简单配置，NetworkManager会默认管理所有以太网设备和WiFi设备，默认以DHCP的方式启动这些以太网设备。而networkd则徐手动指定。

network :
  version : 2
  renderer : NetworkManager
  # 同缩进的关键字无所谓先后，version: 2可以现在后面
  # 可以sudo apt install yamllint进行自动缩进

    1
    2
    3
    4
    5

注：

    YAML在Saltstack中使用两个空格进行缩进，一定不要使用Tab进行缩进。
    YAML的冒号后边如果还要文本，需要一个空格。
    列表项的写法类似Markdown，-加上空格。

可以单独配置YAML，文件位于/etc/netplan目录下且以.yaml结尾即可。示例如下：

network:
  version: 2
  renderer: networkd
  ethernets:
    enp0s3:
      ## 可以ip a查看以太网连接名称
      dhcp4: true
    enp0s8:
      # addresses: []
      ## addresses添加网络掩码，如192.168.56.3/24
      addresses: [192.168.5.3/24]
      # gateway4: 10.10.10.1
      ## gateway4已经废弃，推荐使用routes
      routes:
      - to: "default"
        via: "192.168.5.1"
      nameservers:
        search: [mydomain , otherdomain ]
        addresses: [192.168.5.1 , 114.114.114.114]
      optional: true

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20

这里的addresses: [192.168.56.3/24]就是列表项，可以写作：

addresses: 
  - 192.168.56.3/24

    1
    2

如上[192.168.5.1 , 114.114.114.114]的域名服务器设置，在通过netplan get进行查看时，会显示为：

nameservers:
  addresses:
    - 192.168.5.1
    - 114.114.114.114

    1
    2
    3
    4

整个配置为enp0s3网卡开启DHCP，该网络连接将自动获取地址；为enp0s8网卡配置了一个静态IP192.168.5.3，掩码为24位。
命令

完成了配置文件的描述，之后就可以进行通过netplan通过针对后端管理工具生成配置文件。
netplan操作命令提供常用子命令如下：

    netplan generate ：生成后端管理工具对应的配置；
    netplan apply ：应用配置，必要时重启管理工具；
    netplan try ：在配置得到确认之后才应用，如果配置存在错误，则回滚，类似test；
    netplan get：获取当前netplan配置；
    netplan set：修改当前netplan配置。

netplan --debug apply 返回错误信息。
示例

（选作操作）无移动设备桌面版（或移动设备虚拟机）需执行如下操作：

    sudo systemctl status NetworkManager停止NetworkManger.service；
    sudo systemctl disable NetworkManager关闭NetworkManger.service开启自启。

必须步骤：

    sudo cp /etc/netplan/01-network-manager-all.yaml{,.bak}进行配置文件备份（打头的两位数字类似于udev中的优先级，这里不过多关心）；
    sudo vim /etc/netplan/01-network-manager-all.yaml修改配置文件，内容如下（自行修改）：

network:
  version: 2
  renderer: networkd
  ethernets:
    enp0s25:
      addresses: [192.168.5.243/24]
      routes:
        - to: default
          via: 192.168.5.1
      nameservers: 
        addresses: [192.168.5.1, 114.114.114.114]

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11

    sudo netplan apply应用配置。
    注：网关不确定的情况下，可以通过networkctl status查看（如果使用的是VMware虚拟机，则.2地址为网关和本地DNS服务器）。

辅助命令

networkctl可以查看网络设备的汇总信息，networkctl status可以查看详情状态。
————————————————
版权声明：本文为CSDN博主「cccrick」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/bigbaojian/article/details/125396045