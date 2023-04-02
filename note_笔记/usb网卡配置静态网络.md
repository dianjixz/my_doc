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