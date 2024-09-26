`althttpd` 是一个简单的、高性能的Web服务器，由Mastodon的开发者Gargron（Eugen Rochko）编写。它主要用于提供静态文件服务，但也支持CGI（公共网关接口）脚本。以下是使用和配置`althttpd`以支持CGI的基本步骤：

### 安装althttpd

首先，你需要获取`althttpd`。如果你使用的是Mastodon，`althttpd`可能已经包含在内。否则，你可以从Mastodon的GitHub仓库中获取源代码。

### 配置althttpd

`althttpd`的配置文件通常是一个简单的文本文件，其中包含服务器的各种设置。以下是一些基本的配置选项，特别是关于CGI的配置：

1. **监听地址和端口**：指定服务器监听的IP地址和端口。
```
-port 8080
```

2. **根目录**：指定服务器提供文件的根目录。
```
-root /path/to/your/web/root
```

3. **日志文件**：指定日志文件的位置。
```
-logfile /var/logs/althttpd/httplog-%Y%m%d.csv
```

4. **运行用户**：指定程序运行的目录。
```
-user user
```


6. **其他设置**：根据需要配置其他设置，如超时时间、最大连接数等。

### 编写CGI脚本

CGI脚本通常是用Perl、Python、Ruby等脚本语言编写的。以下是一个简单的Python CGI脚本示例：

```python
#!/usr/bin/env python3

print("Content-Type: text/html")
print()
print("<html><head><title>Hello</title></head><body>")
print("<h1>Hello, World!</h1>")
print("</body></html>")
```

确保脚本有执行权限：
```bash
chmod +x /path/to/your/web/root/hello.py
```

### 启动althttpd

#### 独立服务器运行
使用以下命令启动`althttpd`：
```bash
althttpd -root /path/to/your/web/root -port 8080 -user user -logfile /path/to/your/web/httplog-%Y%m%d.csv
```

#### systemd 运行

```shell
# /etc/systemd/system/http.socket
[Unit]
Description=HTTP socket

[Socket]
Accept=yes
ListenStream=80
NoDelay=true

[Install]
WantedBy=sockets.target
```
```shell
# /etc/systemd/system/http@.service
[Unit]
Description=HTTP socket server
After=network-online.target

[Service]
WorkingDirectory=/home/www
ExecStart=/usr/bin/althttpd -root /home/www -user www-data
StandardInput=socket

[Install]
WantedBy=multi-user.target
```
```html
<!-- /home/www/default.website/index.html -->
<h1>Hello, World</h1>
<p>If you can see this, that means the web server is running.</p>
```
```shell
# mkdir -p /home/www/default.website
# chown www-data /home/www/default.website

systemctl daemon-reload
systemctl enable http.socket
systemctl start http.socket

# systemctl status http.socket
# systemctl stop http.socket
```

#### Stunnel4 运行
Stunnel4 是一个开源的多平台软件包，主要用于在不安全的网络上提供安全的加密通道。它通过 SSL/TLS 加密协议来保护客户端和服务器之间的通信，确保数据传输的安全性和完整性。以下是 Stunnel4 的一些主要特点：

1. **加密通信**：Stunnel4 使用 SSL/TLS 协议来加密数据，防止在传输过程中被窃听或篡改。

2. **兼容性强**：它可以与多种网络服务和协议（如 HTTP、SMTP、POP3、IMAP 等）配合使用，几乎可以为任何基于 TCP 的应用提供加密支持。

3. **跨平台支持**：Stunnel4 支持多种操作系统，包括 Windows、Linux、macOS 等。

4. **高性能**：Stunnel4 经过优化，可以处理大量并发连接，适用于高负载环境。

5. **易于配置**：其配置文件简单明了，用户可以根据需要快速配置和部署。

6. **透明代理**：Stunnel4 可以作为透明代理工作，使得客户端和服务器无需修改即可享受加密通信的好处。

Stunnel4 通常用于以下场景：

- 为不支持 SSL/TLS 加密的旧应用添加安全层。
- ���护电子邮件传输过程中的隐私和安全。
- 通过加密保护远程数据库连接。
- 在客户端和服务器之间建立安全的 VPN 通道。

总的来说，Stunnel4 是一个强大的工具，通过其灵活性和强大的加密能力，为用户提供了简单而有效的网络安全解决方案。

```conf
; /etc/stunnel/stunnel.conf
cert = /etc/letsencrypt/live/sqlite.org/fullchain.pem
key = /etc/letsencrypt/live/sqlite.org/privkey.pem
[https]
accept       = :::443
TIMEOUTclose = 0
exec         = /usr/bin/althttpd
execargs     = /usr/bin/althttpd -logfile /logs/http.log -root /home/www -user www-data -https 1
```

### 测试CGI脚本

在浏览器中访问你的CGI脚本，例如：
```
http://your_server_ip:8080/hello.py
```

如果一切配置正确，你应该能看到CGI脚本的输出。

### 注意事项

- 确保你的CGI脚本路径和文件权限设置正确。
- 如果你的服务器运行在非标准端口，确保在URL中包含端口号。
- 检查日志文件以诊断任何问题。

以上是使用和配置`althttpd`以支持CGI的基本指南。根据你的具体需求，可能需要进一步调整配置。


### 获取传入的表单数据
```python
#!/usr/bin/env python3

import cgi
import cgitb

# 启用调试模式
cgitb.enable()

# 创建FieldStorage实例
form = cgi.FieldStorage()

# 获取表单数据
name = form.getvalue('name')
age = form.getvalue('age')

print("Content-Type: text/html")    # CGI标准头
print()                             # 空行，表示头部结束

print("<html>")
print("<head>")
print("<title>CGI脚本示例</title>")
print("</head>")
print("<body>")
print("<h1>表单数据</h1>")
print("<p>名字: {}</p>".format(name))
print("<p>年龄: {}</p>".format(age))
print("</body>")
print("</html>")

```

```python
#!/usr/bin/env python3

import cgi
import cgitb
import os

# 启用调试模式
cgitb.enable()

print("Content-Type: text/html")    # CGI的标准开头
print()                             # 空行分隔头部和内容

# 获取并解析GET和POST参数
form = cgi.FieldStorage()

# 获取GET参数
get_params = {}
if os.environ['QUERY_STRING']:
    for param in os.environ['QUERY_STRING'].split('&'):
        key, value = param.split('=')
        get_params[key] = value

# 获取POST参数
post_params = {key: form.getvalue(key) for key in form.keys()}

# 输出HTML内容
print("<html>")
print("<head>")
print("<title>CGI脚本示例</title>")
print("</head>")
print("<body>")

print("<h1>GET参数</h1>")
if get_params:
    print("<ul>")
    for key, value in get_params.items():
        print(f"<li>{key}: {value}</li>")
    print("</ul>")
else:
    print("<p>没有GET参数</p>")

print("<h1>POST参数</h1>")
if post_params:
    print("<ul>")
    for key, value in post_params.items():
        print(f"<li>{key}: {value}</li>")
    print("</ul>")
else:
    print("<p>没有POST参数</p>")

print("</body>")
print("</html>")

```


### 关于 althttpd 权限的问题
althttpd 有对可执行文件 cgi 的权限进行了管理，当可执行文件拥有了除拥有着之外其他用户的权限时，althttpd 将会拒绝执行这个 cgi 程序。这样做的目的似乎是为了保护什么。
所以在部署 cgi 程序时，一定要注意好 cgi 程序的权限。