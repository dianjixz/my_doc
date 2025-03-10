# xtcp
## 端口服务端
```toml
serverAddr = "x.x.x.x"
serverPort = 7000
# 如果默认的 STUN 服务器不可用，可以配置一个新的 STUN 服务器
# natHoleStunServer = "xxx"

[[proxies]]
name = "p2p_ssh"
type = "xtcp"
# 只有共享密钥 (secretKey) 与服务器端一致的用户才能访问该服务
secretKey = "abcdefg"
localIP = "127.0.0.1"
localPort = 22
```

## 端口客户端
```toml
serverAddr = "x.x.x.x"
serverPort = 7000
# 如果默认的 STUN 服务器不可用，可以配置一个新的 STUN 服务器
# natHoleStunServer = "xxx"

[[visitors]]
name = "p2p_ssh_visitor"
type = "xtcp"
# 要访问的 P2P 代理的名称
serverName = "p2p_ssh"
secretKey = "abcdefg"
# 绑定本地端口以访问 SSH 服务
bindAddr = "127.0.0.1"
bindPort = 6000
# 如果需要自动保持隧道打开，将其设置为 true
# keepTunnelOpen = false

```


# tcp
## 端口服务端
```toml
serverAddr = "x.x.x.x"
serverPort = 7000

[[proxies]]
name = "ssh"
type = "tcp"
localIP = "127.0.0.1"
localPort = 22
remotePort = 6000

```

# 安全地暴露内网服务
## 端口服务端
```toml
serverAddr = "x.x.x.x"
serverPort = 7000

[[proxies]]
name = "secret_ssh"
type = "stcp"
# 只有与此处设置的 secretKey 一致的用户才能访问此服务
secretKey = "abcdefg"
localIP = "127.0.0.1"
localPort = 22
```
## 端口客户端
```toml
serverAddr = "x.x.x.x"
serverPort = 7000

[[visitors]]
name = "secret_ssh_visitor"
type = "stcp"
# 要访问的 stcp 代理的名字
serverName = "secret_ssh"
secretKey = "abcdefg"
# 绑定本地端口以访问 SSH 服务
bindAddr = "127.0.0.1"
bindPort = 6000
```


# 插件服务
## static_file
```toml
serverAddr = "x.x.x.x"
serverPort = 7000

[[proxies]]
name = "test_static_file"
type = "tcp"
remotePort = 6000
[proxies.plugin]
type = "static_file"
# 本地文件目录，对外提供访问
localPath = "/tmp/file"
# URL 中的前缀，将被去除，保留的内容即为要访问的文件路径
stripPrefix = "static"
httpUser = "abc"
httpPassword = "abc"
```
## https2http
```toml
serverAddr = "x.x.x.x"
serverPort = 7000

[[proxies]]
name = "test_htts2http"
type = "https"
customDomains = ["test.yourdomain.com"]

[proxies.plugin]
type = "https2http"
localAddr = "127.0.0.1:80"

# HTTPS 证书相关的配置
crtPath = "./server.crt"
keyPath = "./server.key"
hostHeaderRewrite = "127.0.0.1"
requestHeaders.set.x-from-where = "frp"

```
## 修改 Host Header
```toml
# frpc.toml Host Header 的修改功能可以动态修改 HTTP 请求中的 Host 字段。
[[proxies]]
name = "web"
type = "http"
localPort = 80
customDomains = ["test.yourdomain.com"]
hostHeaderRewrite = "dev.yourdomain.com"
```

```toml
# frpc.toml
[[proxies]]
name = "web"
type = "http"
localPort = 80
customDomains = ["test.yourdomain.com"]
hostHeaderRewrite = "dev.yourdomain.com"
requestHeaders.set.x-from-where = "frp"
responseHeaders.set.foo = "bar"

```

```toml
# frpc.toml 需要输入配置的用户名和密码才能访问 HTTP 服务
[[proxies]]
name = "web"
type = "http"
localPort = 80
customDomains = ["test.yourdomain.com"]
httpUser = "abc"
httpPassword = "abc"
```

```toml
# frpc.toml frp 支持根据请求的 URL 路径路由转发到不同的后端服务
[[proxies]]
name = "web01"
type = "http"
localPort = 80
customDomains = ["web.yourdomain.com"]
locations = ["/"]

[[proxies]]
name = "web02"
type = "http"
localPort = 81
customDomains = ["web.yourdomain.com"]
locations = ["/news", "/about"]


```

## SSH Tunnel Gateway
frp 支持在 frps 端监听一个 ssh 端口，通过走 ssh -R 协议来完成 TCP 协议代理，该模式下不需要依赖 frpc。
```toml
# frps.toml
sshTunnelGateway.bindPort = 0
sshTunnelGateway.privateKeyFile = ""
sshTunnelGateway.autoGenPrivateKeyPath = ""
sshTunnelGateway.authorizedKeysFile = ""


# frps.toml 复用本机的 id_rsa 文件

sshTunnelGateway.bindPort = 2200
sshTunnelGateway.privateKeyFile = "/home/user/.ssh/id_rsa"


# frps.toml 指定自动生成私钥文件地址

sshTunnelGateway.bindPort = 2200
sshTunnelGateway.autoGenPrivateKeyPath = "/var/frp/ssh-private-key-file"


# frps.toml 使用本机已存在的 authorized_keys 文件，实现 ssh 免密登录鉴权

sshTunnelGateway.bindPort = 2200
sshTunnelGateway.authorizedKeysFile = "/home/user/.ssh/authorized_keys"



# frps.toml 使用自定义的 authorized_keys 文件，实现 ssh 免密登录鉴权

sshTunnelGateway.bindPort = 2200
sshTunnelGateway.authorizedKeysFile = "/var/frps/custom_authorized_keys_file"


```


客户端 ssh
```shell

ssh -R :80:{local_ip:port} v0@{frps address} -p {frps ssh listen port} {tcp|http|https|stcp|tcpmux} --remote_port {real remote port} --proxy_name {proxy_name} --token {frp token}
``` 

--proxy_name 非必填，为空会随机生成一个。
登录 frps 的用户名统一为 v0，目前没有任何含义，即 v0@{frps address}。
server 端的 proxy 监听的端口由 --remote_port 决定。
{tcp|http|https|stcp|tcpmux} 支持的完整命令参数可通过 –help 获取。 如： ssh -R :80::8080 v0@127.0.0.1 -p 2200 http --help
token 非必填，为了安全强烈建议在frps配置上token。


TCP 代理

```shell
ssh -R :80:127.0.0.1:8080 v0@{frp address} -p 2200 tcp --proxy_name "test-tcp" --remote_port 9090
```

HTTP 代理
```shell
ssh -R :80:127.0.0.1:8080 v0@{frp address} -p 2200 http --proxy_name "test-http"  --custom_domain test-http.frps.com

```





















