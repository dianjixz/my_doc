# ssh登陆信息设置
ssh的登陆信息主要修改四个地方：
## 第一个文件/etc/ssh/sshd_config
相关内容：  
~~~
参数选项                                                        说明
Port 22                                                         SSH 预设使用 22 这个 port，您也可以使用多的 port ！
Protocol 2,1                                                    选择的 SSH 协议版本，可以是 1 也可以是 2 ，如果要同时支持两者，就必须要使用 2,1 这个分隔了！
ListenAddress 0.0.0.0                                           监听的主机适配卡！举个例子来说，如果您有两个 IP，分别是 192.168.0.100 及 192.168.2.20 ，那么只想要开放 192.168.0.100 时，就可以写如同下面的样式：
ListenAddress 192.168.0.100                                     只监听来自 192.168.0.100 这个 IP 的SSH联机。如果不使用设定的话，则预设所有接口均接受 SSH
PidFile /var/run/sshd.pid                                       可以放置 SSHD 这个 PID 的档案！左列为默认值
LoginGraceTime 600                                              当使用者连上 SSH server 之后，会出现输入密码的画面，在该画面中，在多久时间内没有成功连上 SSH server ，就断线！时间为秒！
Compression yes                                                 是否可以使用压缩指令？
HostKey /etc/ssh/ssh_host_key                                   SSH version 1 使用的私钥
HostKey /etc/ssh/ssh_host_rsa_key                               SSH version 2 使用的 RSA 私钥
HostKey /etc/ssh/ssh_host_dsa_key                               SSH version 2 使用的 DSA 私钥
KeyRegenerationInterval 3600                                    由前面联机的说明可以知道， version 1 会使用 server 的 Public Key ，每隔一段时间来重新建立一次！时间为秒！
ServerKeyBits 768                                               Server key 的长度！
SyslogFacility AUTH                                             当有人使用 SSH 登入系统的时候，SSH会记录信息
LogLevel INFO                                                   登录记录的等级---》全部
PermitRootLogin no                                              是否允许 root 登入！预设是允许的，但是建议设定成 no！
UserLogin no                                                    在 SSH 底下本来就不接受 login 这个程序的登入！
StrictModes yes                                                 当使用者的 host key 改变之后，Server 就不接受联机
RSAAuthentication yes                                           是否使用纯的 RSA 认证！？仅针对 version 1 ！
PubkeyAuthentication yes                                        是否允许 Public Key ？只有 version 2
AuthorizedKeysFile   .ssh/authorized_keys                       设定若要使用不需要密码登入的账号时，那么那个账号的存放档案所在档名！
RhostsAuthentication no                                         本机系统不使用 .rhosts ， .rhosts 不安全！
IgnoreRhosts yes                                                是否取消使用 ~/.ssh/.rhosts 来做为认证！
RhostsRSAAuthentication no                                      针对 version 1 ，使用 rhosts 档案在/etc/hosts.equiv配合 RSA 演算方式来进行认证！
HostbasedAuthentication no                                      这个项目与上面的项目类似，不过是给 version 2 使用的！
IgnoreUserKnownHosts no                                         是否忽略家目录内的 ~/.ssh/known_hosts 这个档案所记录的主机内容
PasswordAuthentication yes                                      密码验证当然是需要的！
PermitEmptyPasswords no                                         上面那一项如果设定为 yes 的话，这一项就最好设定为 no ，这个项目在是否允许以空的密码登入！
ChallengeResponseAuthentication yes                             挑战任何的密码认证！所以，任何 login.conf 规定的认证方式，均可适用！
PAMAuthenticationViaKbdInt yes                                  是否启用其它的 PAM 模块！启用这个模块将会导致 PasswordAuthentication 设定失效！

与Kerberos 有关的参数设定！底下不用设定
KerberosAuthentication no
KerberosOrLocalPasswd yes
KerberosTicketCleanup yes
KerberosTgtPassing no

有关在 X-Window 底下使用的相关设定
X11Forwarding yes
X11DisplayOffset 10
X11UseLocalhost yes

PrintMotd no                                                    登入后是否显示出一些信息呢？例如上次登入的时间、地点等，预设是 yes ，但是，如果为了安全，可以考虑改为 no ！
PrintLastLog yes                                                显示上次登入的信息！预设也是 yes 
KeepAlive yes                                                   一般而言，如果设定这项目的话，那么 SSH Server 会传送KeepAlive 的讯息给 Client 端，以确保两者的联机正常！在这个情况下，任何一端死掉后， SSH 可以立刻知道！而不会有僵尸程序的发生！
UsePrivilegeSeparation yes                                      使用者的权限设定项目！
MaxStartups 10                                                  同时允许几个尚未登入的联机画面
DenyUsers *                                                     设定受抵挡的使用者名称
AllowUsers *                                                    设定允许的使用者名称
~~~


## 第二个文件/etc/issue这个是登陆信息头


## 第三个文件/etc/update-motd.d是登陆初始化

## 第四个文件/etc/motd 是的登陆logo





### 开启密登陆

使用命令（vim sshd_config），这个就是ssh的配置文件，也是我们需要改的文件，主要改下面这几个地方（前面四个都是有的，只需要将前面的注释去掉即可，最后一个自己添加下就行）

RSAAuthentication yes 开启RSA验证

PubkeyAuthentication yes 是否使用公钥验证

AuthorizedKeysFile .ssh/authorized_keys 公钥的保存位置

HostKey /etc/ssh/ssh_host_rsa_key

PasswordAuthentication no 禁止使用密码验证登录（自己添加）
























### 信息来源
https://blog.csdn.net/angou6476/article/details/101996928?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control&dist_request_id=1619538237256_54371&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control







## ssh代理功能

ssh 命令除了登陆外还有三种代理功能：

- 正向代理（-L）：相当于 iptable 的 port forwarding
- 反向代理（-R）：相当于 frp 或者 ngrok
- socks5 代理（-D）：相当于 ss/ssr

**正向代理：**

所谓“正向代理”就是在本地启动端口，把本地端口数据转发到远端。

用法1：远程端口映射到其他机器

HostB 上启动一个 PortB 端口，映射到 HostC:PortC 上，在 HostB 上运行：

```bash
HostB$ ssh -L 0.0.0.0:PortB:HostC:PortC user@HostC
```

这时访问 HostB:PortB 相当于访问 HostC:PortC（和 iptable 的 port-forwarding 类似）。

用法2：本地端口通过跳板映射到其他机器

HostA 上启动一个 PortA 端口，通过 HostB 转发到 HostC:PortC上，在 HostA 上运行：

```bash
HostA$ ssh -L 0.0.0.0:PortA:HostC:PortC  user@HostB
```

这时访问 HostA:PortA 相当于访问 HostC:PortC。

两种用法的区别是，第一种用法本地到跳板机 HostB 的数据是明文的，而第二种用法一般本地就是 HostA，访问本地的 PortA，数据被 ssh 加密传输给 HostB 又转发给 HostC:PortC。

**反向代理：**

所谓“反向代理”就是让远端启动端口，把远端端口数据转发到本地。

HostA 将自己可以访问的 HostB:PortB 暴露给外网服务器 HostC:PortC，在 HostA 上运行：

```bash
HostA$ ssh -R HostC:PortC:HostB:PortB  user@HostC
```

那么链接 HostC:PortC 就相当于链接 HostB:PortB。使用时需修改 HostC 的 /etc/ssh/sshd_config，添加：

```apacheconf
GatewayPorts yes
```

相当于内网穿透，比如 HostA 和 HostB 是同一个内网下的两台可以互相访问的机器，HostC是外网跳板机，HostC不能访问 HostA，但是 HostA 可以访问 HostC。

那么通过在内网 HostA 上运行 `ssh -R` 告诉 HostC，创建 PortC 端口监听，把该端口所有数据转发给我（HostA），我会再转发给同一个内网下的 HostB:PortB。

同内网下的 HostA/HostB 也可以是同一台机器，换句话说就是**内网 HostA 把自己可以访问的端口暴露给了外网 HostC。**

按照前文《[韦易笑：内网穿透：在公网访问你家的 NAS](https://zhuanlan.zhihu.com/p/57477087)》中，相当于再 HostA 上启动了 frpc，而再 HostC 上启动了 frps。



**本地 socks5 代理**

在 HostA 的本地 1080 端口启动一个 socks5 服务，通过本地 socks5 代理的数据会通过 ssh 链接先发送给 HostB，再从 HostB 转发送给远程主机：

```bash
HostA$ ssh -D localhost:1080  HostB
```

那么在 HostA 上面，浏览器配置 socks5 代理为 127.0.0.1:1080，看网页时就能把数据通过 HostB 代理出去，类似 ss/ssr 版本，只不过用 ssh 来实现。



**使用优化**

为了更好用一点，ssh 后面还可以加上：`-CqTnN` 参数，比如：

```bash
$ ssh -CqTnN -L 0.0.0.0:PortA:HostC:PortC  user@HostB
```

其中 `-C` 为压缩数据，`-q` 安静模式，`-T` 禁止远程分配终端，`-n` 关闭标准输入，`-N` 不执行远程命令。此外视需要还可以增加 `-f` 参数，把 ssh 放到后台运行。

这些 ssh 代理没有短线重连功能，链接断了命令就退出了，所以需要些脚本监控重启，或者使用 autossh 之类的工具保持链接。

**功能对比**

正向代理（-L）的第一种用法可以用 iptable 的 port-forwarding 模拟，iptable 性能更好，但是需要 root 权限，ssh -L 性能不好，但是正向代理花样更多些。反向代理（-R）一般就作为没有安装 frp/ngrok/shootback 时候的一种代替，但是数据传输的性能和稳定性当然 frp 这些专用软件更好。

socks5 代理（-D）其实是可以代替 ss/ssr 的，区别和上面类似。所以要长久使用，推荐安装对应软件，临时用一下 ssh 挺顺手。



\--

补充下 iptable 的 `port-forwarding` 怎么设置，十分管用的功能，两个函数即可：

```bash
#! /bin/sh

# create forward rule by source interface
# http://serverfault.com/questions/532569/how-to-do-port-forwarding-redirecting-on-debian
PortForward1() {
    local IN_IF=$1
    local IN_PORT=$2
    local OUT_IP=$3
    local OUT_PORT=$4
    local IPTBL="/sbin/iptables"
    echo "1" > /proc/sys/net/ipv4/ip_forward
    $IPTBL -A PREROUTING -t nat -i $IN_IF -p tcp --dport $IN_PORT -j DNAT --to-destination ${OUT_IP}:${OUT_PORT}
    $IPTBL -A FORWARD -p tcp -d $OUT_IP --dport $OUT_PORT -j ACCEPT
    $IPTBL -A POSTROUTING -t nat -j MASQUERADE
}

# create forward rule by source ip
# http://blog.csdn.net/zzhongcy/article/details/42738285
ForwardPort2() {
    local IN_IP=$1
    local IN_PORT=$2
    local OUT_IP=$3
    local OUT_PORT=$4
    local IPTBL="/sbin/iptables"
    echo "1" > /proc/sys/net/ipv4/ip_forward
    $IPTBL -t nat -A PREROUTING --dst $IN_IP -p tcp --dport $IN_PORT -j DNAT --to-destination ${OUT_IP}:${OUT_PORT}
    $IPTBL -t nat -A POSTROUTING --dst $OUT_IP -p tcp --dport $OUT_PORT -j SNAT --to-source $IN_IP
}
```

第一个函数是按照网卡名称设置转发：

```bash
PortForward1 eth1 8765 202.115.8.2 8765
```

这时，本地 eth1 网卡的 8765 端口就会被转发给 202.115.8.2 的 8765 端口。

第二个函数是按照本机的 ip 地址，比如本机是 192.168.1.2：

```bash
PortForward2 192.168.1.2 8765 202.115.8.2 8765
```

那么任何访问本机 192.168.1.2 这个地址 8765 端口，都会被转发到 202.115.8.2:8765

这个 iptable 的 `port forwarding` 是内核层运行的，性能极好，只不过每次重启都需要重新设置下。
