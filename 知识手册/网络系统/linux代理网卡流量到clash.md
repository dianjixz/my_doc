linux代理网卡流量到clash




想要在嵌入式设备上代理流量到 clash 上，实现让下级设备也能科学上网。
2020年10月3日-树莓派充当透明代理

https://www.geekgao.cn/archives/2515

上面这个配置也不太好用，下面有一个真正的透明代理。

直接获取订阅：
```bash
curl --user-agent "clash" 'https://su.gwzxwk.com/api/v1/client/subscribe?token=xxxxxxxxxxxxxxxxxxxxxxxxxx' > config.yaml
```
使用--user-agent参数。

subconverter转换订阅url为clash配置文件
下载subconverter,解压,
```bash
wget https://github.com/tindy2013/subconverter/releases/download/v0.7.1/subconverter_linux64.tar.gz
tar xzvf subconverter_linux64.tar.gz

```
通过http转换订阅文件
适合搭建转换服务器,供所有的机器使用
```bash
cndaqiang@GIRL subconverter$ ./subconverter
2022/02/21 Mon 21:15:44.125582 [1058 10739384][INFO] SubConverter v0.7.1 starting up..
2022/02/21 Mon 21:15:44.127224 [1058 10739384][INFO] Reading preference settings...
......
2022/02/21 Mon 21:15:44.164532 [1058 10739384][INFO] Startup completed. Serving HTTP @ http://0.0.0.0:25500
```

使用http获取clash配置文件, 网址规则详见README-cn.md,注意要把订阅连接转为URLEncode,示例

```bash

cndaqiang@GIRL tmp$ curl -s "http://127.0.0.1:25500/sub?target=clash&url=http%3A%2F%2Fdy.clashweb.site%2Fapi%2Fv1%2Fclient%2Fsubscribe%3Ftoken%3Dcndaqiangstoken" > config.yaml
cndaqiang@GIRL tmp$ head  config.yaml
port: 7890
socks-port: 7891
allow-lan: true
mode: Rule
log-level: info
external-controller: :9090
proxies:
  - {name: 日本01-CM-2.0, server: xxx.gz.cm.888022.site, port: 32224, type: ss, cipher: chacha20-ietf-poly1305, password: ec684a62-be94-47f6-820c-779e4d921234, udp: true}
  ...
```
根据URLEncode的格式也可以自己写一个
```bash
cndaqiang@GIRL tmp$ curl -s "http://127.0.0.1:25500/sub?target=clash&url="$(echo "http://dy.clashweb.site/api/v1/client/subscribe?token=cndaqiangstoken" | sed  "s|/|%2F|g" | sed  "s|:|%3A|g"  | sed  "s|?|%3F|g")
```

[推荐]命令行转换

修改配置文件,自定义配置类型[cndaqiang],url可以为文件也可以为订阅地址
```bash
cndaqiang@GIRL subconverter$ vi generate.ini
#内容如下
[cndaqiang]
path=config.yml
target=clash
ver=4
url=http://dy.clashweb.site/api/v1/client/subscribe?token=cndaqiangstoken
```
生成配置文件config.yml
```bash
cndaqiang@GIRL subconverter$ ./subconverter -g --artifact "cndaqiang"
#这里生成的是config.yml,配置文件时yaml
cndaqiang@GIRL subconverter$ cp config.yml ~/.config/clash/config.yaml
```

自定义subconverter生成的配置文件

简单粗暴就直接修改vi base/all_base.tpl就可以

```bash
...
port: { { default(global.clash.http_port, "7890") } }
...
```

在vi pref.toml中设置模板也可以

```bash
...
[[template.globals]]
key = "clash.http_port"
value = "7890"
...
```


本文所采用的透明代理方式不依赖于 TUN, 所有是否是增强版本不重要, 如果可以请尽量使用最新版本.


创建专用的 clash 用户:
```bash
useradd -M -s /usr/sbin/nologin clash
```
编写 Systemd 配置文件:
```bash
cat > /lib/systemd/system/clash.service <<EOF
[Unit]
Description=Clash TProxy
After=network.target

[Service]
Type=simple
User=clash
Group=clash
CapabilityBoundingSet=CAP_NET_ADMIN CAP_NET_BIND_SERVICE CAP_NET_RAW
AmbientCapabilities=CAP_NET_ADMIN CAP_NET_BIND_SERVICE CAP_NET_RAW
Restart=on-failure

ExecStartPre=+/usr/bin/bash /etc/clash/clean.sh
ExecStart=/usr/bin/clash -d /etc/clash
ExecStartPost=+/usr/bin/bash /etc/clash/iptables.sh

ExecStopPost=+/usr/bin/bash /etc/clash/clean.sh

[Install]
WantedBy=multi-user.target
EOF

```

调整配置
本文中 Clash 配置文件、脚本等统一存放到 /etc/clash 目录中, 针对于 Clash 配置文件, 着重说明重点配置, 完整配置请从官方 Wiki 复制: https://github.com/Dreamacro/clash/wiki/configuration#all-configuration-options

端口配置
端口配置请尽量保持默认, 如果需要调整端口, 请同步修改后面相关脚本中的端口(TProxy):
```bash
# 注释掉 port 端口配置, 使用 mixed-port
#port: 7890

# 注释掉 socks-port 端口配置, 使用 mixed-port
#socks-port: 7891

# 注释掉 redir-port 端口配置, 因为全部采用 TProxy 模式
#redir-port: 7892

# TProxy 的透明代理端口
tproxy-port: 7893

# mixed-port 端口将同时支持 SOCKS5/HTTP
mixed-port: 7890

# 允许来自局域网的连接
allow-lan: true

# 绑定到所有接口
bind-address: '*'

```

DNS 配置
Clash 配置中请开启 DNS, 并使用 fake-ip 模式, 样例配置如下:
```yaml
dns:
  enable: true
  listen: 0.0.0.0:1053
  ipv6: false
  default-nameserver:
    - 114.114.114.114
    - 8.8.8.8
  enhanced-mode: fake-ip
```
上面的dns配置有问题，还是需要从原来的配置中抄一份过来比较好。

防火墙规则
为了保证防火墙规则不被破坏, 本文采用脚本暴力操作, 如果宿主机有其他 iptables 控制程序, 则推荐手动执行并通过 iptables-persistent 等工具进行持久化;

/etc/clash/iptables.sh: 负责启动时添加 iptables 规则
```bash
#!/usr/bin/env bash

set -ex

# ENABLE ipv4 forward
sysctl -w net.ipv4.ip_forward=1

# ROUTE RULES
ip rule add fwmark 666 lookup 666
ip route add local 0.0.0.0/0 dev lo table 666

# clash 链负责处理转发流量 
iptables -t mangle -N clash

# 目标地址为局域网或保留地址的流量跳过处理
# 保留地址参考: https://zh.wikipedia.org/wiki/%E5%B7%B2%E5%88%86%E9%85%8D%E7%9A%84/8_IPv4%E5%9C%B0%E5%9D%80%E5%9D%97%E5%88%97%E8%A1%A8
iptables -t mangle -A clash -d 0.0.0.0/8 -j RETURN
iptables -t mangle -A clash -d 127.0.0.0/8 -j RETURN
iptables -t mangle -A clash -d 10.0.0.0/8 -j RETURN
iptables -t mangle -A clash -d 172.16.0.0/12 -j RETURN
iptables -t mangle -A clash -d 192.168.0.0/16 -j RETURN
iptables -t mangle -A clash -d 169.254.0.0/16 -j RETURN

iptables -t mangle -A clash -d 224.0.0.0/4 -j RETURN
iptables -t mangle -A clash -d 240.0.0.0/4 -j RETURN

# 其他所有流量转向到 7893 端口，并打上 mark
iptables -t mangle -A clash -p tcp -j TPROXY --on-port 7893 --tproxy-mark 666
iptables -t mangle -A clash -p udp -j TPROXY --on-port 7893 --tproxy-mark 666

# 转发所有 DNS 查询到 1053 端口
# 此操作会导致所有 DNS 请求全部返回虚假 IP(fake ip 198.18.0.1/16)
iptables -t nat -I PREROUTING -p udp --dport 53 -j REDIRECT --to 1053

# 如果想要 dig 等命令可用, 可以只处理 DNS SERVER 设置为当前内网的 DNS 请求
#iptables -t nat -I PREROUTING -p udp --dport 53 -d 192.168.0.0/16 -j REDIRECT --to 1053

# 最后让所有流量通过 clash 链进行处理
iptables -t mangle -A PREROUTING -j clash

# clash_local 链负责处理网关本身发出的流量
iptables -t mangle -N clash_local

# nerdctl 容器流量重新路由
#iptables -t mangle -A clash_local -i nerdctl2 -p udp -j MARK --set-mark 666
#iptables -t mangle -A clash_local -i nerdctl2 -p tcp -j MARK --set-mark 666

# 跳过内网流量
iptables -t mangle -A clash_local -d 0.0.0.0/8 -j RETURN
iptables -t mangle -A clash_local -d 127.0.0.0/8 -j RETURN
iptables -t mangle -A clash_local -d 10.0.0.0/8 -j RETURN
iptables -t mangle -A clash_local -d 172.16.0.0/12 -j RETURN
iptables -t mangle -A clash_local -d 192.168.0.0/16 -j RETURN
iptables -t mangle -A clash_local -d 169.254.0.0/16 -j RETURN

iptables -t mangle -A clash_local -d 224.0.0.0/4 -j RETURN
iptables -t mangle -A clash_local -d 240.0.0.0/4 -j RETURN

# 为本机发出的流量打 mark
iptables -t mangle -A clash_local -p tcp -j MARK --set-mark 666
iptables -t mangle -A clash_local -p udp -j MARK --set-mark 666

# 跳过 clash 程序本身发出的流量, 防止死循环(clash 程序需要使用 "clash" 用户启动) 
iptables -t mangle -A OUTPUT -p tcp -m owner --uid-owner clash -j RETURN
iptables -t mangle -A OUTPUT -p udp -m owner --uid-owner clash -j RETURN

# 让本机发出的流量跳转到 clash_local
# clash_local 链会为本机流量打 mark, 打过 mark 的流量会重新回到 PREROUTING 上
iptables -t mangle -A OUTPUT -j clash_local

# 修复 ICMP(ping)
# 这并不能保证 ping 结果有效(clash 等不支持转发 ICMP), 只是让它有返回结果而已
# --to-destination 设置为一个可达的地址即可
sysctl -w net.ipv4.conf.all.route_localnet=1
iptables -t nat -A PREROUTING -p icmp -d 198.18.0.0/16 -j DNAT --to-destination 127.0.0.1

setcap CAP_NET_BIND_SERVICE=+eip /usr/bin/clash
setcap CAP_NET_ADMIN=+eip /usr/bin/clash

```

/etc/clash/clean.sh: 负责启动前/停止后清理 iptables 规则(暴力清理)

```bash
#!/usr/bin/env bash

set -ex

ip rule del fwmark 666 table 666 || true
ip route del local 0.0.0.0/0 dev lo table 666 || true

iptables -t nat -F
iptables -t nat -X
iptables -t mangle -F
iptables -t mangle -X clash || true
iptables -t mangle -X clash_local || true

```
最终目录结构

所有配置编写完成后, 其目录结构如下:
```bash
root@openrpi # ❯❯❯ tree -L 1 /etc/clash
/etc/clash
├── clean.sh
├── config.yaml
└── iptables.sh

```

最后需要修复 /etc/clash 目录权限, 因为 Clash 启动后会写入其他文件:
```bash
chown -R clash:clash /etc/clash

```

如果所有配置和文件安装没问题的话, 可以直接通过 Systemd 启动:
```bash
# 启动
systemctl start clash

# 查看日志
jouranlctl -fu clash

```


