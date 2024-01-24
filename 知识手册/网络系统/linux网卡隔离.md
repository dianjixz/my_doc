# linux中的网卡隔离

linux 中的网卡隔离是一个非常大的坑，这个坑在与，linux 的多个网卡接口中，很难进行回环 ping。
就算定义了 `ping -I <dev>`,也是没用的。仔细研究路由表之后感觉这是一个悖论，也就是在两个网口相互ping的过程中，很难确保回ping数据重新走来的时候那条路。这在理论上叫路由冲突。
为了解决这个冲突，需要使用网络命名空间技术，将两个网卡隔离开来。

## 网络命名空间
### 1、检查网络命名空间：
首先，确认您的网络命名空间已经创建并且网卡已经正确分配。使用以下命令列出所有网络命名空间：
```bash
root@M5Core135:~# ip netns list
netns2 (id: 1)
netns1 (id: 0)
```
可以看到现在有两个网络命名空间。

### 2、创建新的网络命名空间
```bash
# 创建新的网络命名空间
sudo ip netns add netns1
sudo ip netns add netns2
```

### 3、将网卡分配给不同的网络命名空间
```bash
# 将网卡分配给不同的网络命名空间
sudo ip link set eth0 netns netns1
sudo ip link set eth1 netns netns2
```

### 4、在各自的网络命名空间中配置网卡
```bash
sudo ip netns exec netns1 ip addr add 192.168.1.1/24 dev eth0
sudo ip netns exec netns2 ip addr add 192.168.2.1/24 dev eth1
```

### 5、启用网卡
```bash
sudo ip netns exec netns1 ip link set eth0 up
sudo ip netns exec netns2 ip link set eth1 up
```

### 6、在网络命名空间内使用命令
使用方法为:
```bash
sudo ip netns exec netns1 cmd
```

### 7、删除网络命名空间
```bash
# 列出现有的网络命名空间
ip netns list

# 查找并移出网卡
sudo ip netns exec mynetns ip link

sudo ip netns exec mynetns ip link set eth0 netns 1



sudo ip netns delete [命名空间名称]


# 重新配置网卡
sudo ip addr add 192.168.1.1/24 dev eth0
sudo ip link set eth0 up


```
> 注意：在删除网络命名空间之前，一定要先将网络命名空间内的设备移出到默认空间内，否则网卡设备将会消失。目前这貌似是一个bug。 



关闭自动网络配置服务：
```bash
systemctl stop NetworkManager
```

配置网卡 eth1 到命名空间：
```bash
# 添加一个命名空间
ip netns add netns1

# 将网卡 eth1 添加到命名空间内
ip link set eth1 netns netns1

# 设置网卡的静态ip
ip netns exec netns1 ifconfig eth1 192.168.1.1

# 添加网卡到 eth0 的路由
ip netns exec netns1 ip route add 192.168.0.0/24 dev eth1

# 查看网卡信息
ip netns exec netns1 ifconfig

# 查看网卡的路由信息
ip netns exec netns1 ip route list
```

在主空间内配置网卡 eth0：
```bash
# 配置网卡静态路由
ifconfig eth0 192.168.0.1

# 添加网卡到 eth1 的路由
ip route add 192.168.1.0/24 dev eth0

# 查看网卡的路由信息
ip route
```

从主空间内 ping eth1：
```bash
ping 192.168.1.1
```
此时 ping 数据会从物理层出去，然后达到网卡 eth1 。网卡 eht1 产生了回包也会从 eth1 经过物理层返回 eth0。



恢复网络到之前的状态：
```bash
# 移除网络命名空间内的网卡
ip netns exec netns1 ip link set eth1 netns 1

# 删除网络命名空间
ip netns delete netns1

# 恢复网络自动配置服务
systemctl start NetworkManager
```



# linux 中的网卡隔离互通
不同的网络命名空间在Linux系统中是彼此隔离的，它们默认是不能直接相互通信的。每个网络命名空间都有自己独立的网络堆栈，包括路由表、防火墙规则和网络设备。这种隔离是网络命名空间设计的核心特性之一，用于提供网络环境的隔离和安全。
如果您想要在不同的网络命名空间之间建立通信，有几种方法可以实现：
## 1. 使用虚拟以太网设备对 (veth pair)

这是最常用的方法之一。veth对像管道一样，一端放在一个命名空间，另一端放在另一个命名空间，从而允许两个命名空间中的网络流量互通。创建veth对的步骤如下：
```bash
# 创建veth对
sudo ip link add veth1 type veth peer name veth2

# 将veth的一端移动到第一个网络命名空间
sudo ip link set veth1 netns netns1

# 将veth的另一端移动到第二个网络命名空间
sudo ip link set veth2 netns netns2

# 配置IP地址并启用veth接口
sudo ip netns exec netns1 ip addr add 192.168.1.1/24 dev veth1
sudo ip netns exec netns1 ip link set veth1 up

sudo ip netns exec netns2 ip addr add 192.168.1.2/24 dev veth2
sudo ip netns exec netns2 ip link set veth2 up
```