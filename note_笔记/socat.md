## 前言

socat是强大易用的工具，可以实现任意socket的转换。

## 使用

```
socat [options] <address> <address>
```

options可以查看help。

address有一下几种形式：

- `-` `STDIN` `STDOUT` ：表示标准输入输出，可以就用一个横杠代替
- `/var/log/syslog` : 也可以是任意路径，如果是相对路径要使用./，打开一个文件作为数据流。
- `TCP:127.0.0.1:1080` : 建立一个TCP连接作为数据流，TCP也可以替换为UDP
- `TCP-LISTEN:12345` : 建立TCP监听端口，TCP也可以替换为UDP
- `EXEC:/bin/bash` : 执行一个程序作为数据流。

## 场景一

本地有个文件，我想显示在终端中

```
socat - /etc/sysctl.conf
```

## 场景二

有个TCP连接会连上来，我想看看会获得什么数据。
 `socat TCP-LISTEN:12345 -`

## 场景三

我是黑客，我想在目标机上弄一个shell代理
 `socat TCP-LISTEN:12345 EXEC:/bin/bash`

## 场景四

本地有一个UNIX DOMAIN域套接字，我想把他转成TCP SOCKET供局域网内的机器使用，怎么办？

```
socat TCP-LISTEN:12345,reuseaddr,fork UNIX-CONNECT:/data/deCOREIDPS/unix.domain
```

当有多个tcp连上来那就fork一个去连域套接字

## 场景五

将本地的80端口转到远程去

```
socat TCP-LISTEN:80,fork TCP:www.baidu.com:80
```

> 参考 http://brieflyx.me/2015/linux-tools/socat-introduction/



作者：linanwx
链接：https://www.jianshu.com/p/54005e3095f3
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。





1）实现Linux虚拟串口

```
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

```
http://files.cnblogs.com/files/274914765qq/socat%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.pdf
```



```
what: "netcat++" (extended design, new implementation)
OS:   AIX, BSD, HP-UX, Linux, Solaris e.a. (UNIX)
lic:  GPL2
inst: tar x...; ./configure; make; make install
doc:  README; socat.html, socat.1; xio.help
ui:   command line
exa:  socat TCP6-LISTEN:8080,reuseaddr,fork PROXY:proxy:www.domain.com:80
keyw: tcp, udp, ipv6, raw ip, unix-socket, pty, pipe, listen, socks4, socks4a,
      proxy-connect, ssl-client, filedescriptor, readline, stdio,
      exec, system, file, open, tail -f, termios, setsockopt, chroot,
      fork, perm, owner, trace, dump, dgram, ext3, resolver, datagram,
      multicast, broadcast, interface, socket, sctp, generic, ioctl
```





#             SOCAT 简介        

​      	    Jul 22, 2015    	                in             [Linux](http://brieflyx.me/categories/Linux/)     

## 概述

socat，是linux下的一个工具，其功能与有“瑞士军刀”之称的netcat类似，不过据说可以看做netcat的加强版。的确如此，它有一些netcat所不具备却又很有需求的功能，例如ssl连接这种。nc可能是因为比较久没有维护，确实显得有些陈旧了。

## 安装

Ubuntu上可以直接sudo apt-get install socat，其他发行版没试过。

也可以去官网下载源码包[socat](http://www.dest-unreach.org/socat/)



## 基本语法

```
socat [options] <address> <address>
```

其中这2个address就是关键了，如果要解释的话，address就类似于一个文件描述符，socat所做的工作就是在2个address指定的描述符间建立一个pipe用于发送和接收数据。

那么address的描述就是socat的精髓所在了，几个常用的描述方式如下：

- -,STDIN,STDOUT ：表示标准输入输出，可以就用一个横杠代替，这个就不用多说了吧….
- /var/log/syslog : 也可以是任意路径，如果是相对路径要使用./，打开一个文件作为数据流。
- TCP:: : 建立一个TCP连接作为数据流，TCP也可以替换为UDP
- TCP-LISTEN: : 建立TCP监听端口，TCP也可以替换为UDP
- EXEC: : 执行一个程序作为数据流。

以上规则中前面的TCP等都可以小写。

在这些描述后可以附加一些选项，用逗号隔开，如fork，reuseaddr，stdin，stdout，ctty等。

## socat当cat

直接回显

```
socat - -
```

cat文件

```
socat - /home/user/chuck
```

写文件

```
echo "hello" | socat - /home/user/chuck
```

## socat当netcat

连接远程端口

```
nc localhost 80
socat - TCP:localhost:80
```

监听端口

```
nc -lp localhost 700
socat TCP-LISTEN:700 -
```

正向shell

```
nc -lp localhost 700 -e /bin/bash
socat TCP-LISTEN:700 EXEC:/bin/bash
```

反弹shell

```
nc localhost 700 -e /bin/bash
socat tcp-connect:localhost:700 exec:'bash -li',pty,stderr,setsid,sigint,sane
```

## 代理与转发

将本地80端口转发到远程的80端口

```
socat TCP-LISTEN:80,fork TCP:www.domain.org:80
```

## 其他

其实从这里才是重点

### SSL连接

SSL服务器

```
socat OPENSSL-LISTEN:443,cert=/cert.pem -
```

需要首先生成证书文件

SSL客户端

```
socat - OPENSSL:localhost:443
```

### fork服务器

接下来这个例子，就是我认识socat的原因，可以将一个使用标准输入输出的单进程程序变为一个使用fork方法的多进程服务，非常方便。

```
socat TCP-LISTEN:1234,reuseaddr,fork EXEC:./helloworld
```

### 不同设备的通信

将U盘进行网络共享

```
socat -d -d /dev/ttyUSB1,raw,nonblock,ignoreeof,cr,echo=0 TCP4-LISTEN:5555,reuseaddr
```

-d -d 指的是调试信息的级别

将终端转发到COM1

```
socat READLINE,history=$HOME/.cmd_history /dev/ttyS0,raw,echo=0,crnl
```

socat还有个readbyte的option，这样就可以当dd用了。

## 小结

因为在Linux/UNIX中，一切都是文件，无论是socket还是其他设备。所以从理论上来说，一切能够在文件层级访问的内容都可以成为socat的数据流的来源，2个address可以任意发挥，能够做到的事情还有很多。特别是其fork的功能，确实是netcat所不能比的。

## 参考文献

借鉴的几篇博文：

[Some Useful Socat Commands](http://technostuff.blogspot.com/2008/10/some-useful-socat-commands.html)

[Socat: A very powerful networking tool](http://www.blackbytes.info/2012/07/socat-cheatsheet/)

[Socat Examples](https://stuff.mit.edu/afs/sipb/machine/penguin-lust/src/socat-1.7.1.2/EXAMPLES)

其他内容，可以参考[socat man page](http://www.dest-unreach.org/socat/doc/socat.html)

~~~
姓名
socat - 多用途继电器 (SOCet CAT)

概要
socat [options] <address> <address>
socat -V
socat -h[h[h]] | -?[?[?]]
filan
procan

描述
Socat是一个基于命令行的实用程序，它建立两个双向字节流并在它们之间传输数据。因为流可以由大量不同类型的数据接收器和源构建（请参阅地址类型），并且由于可以将许多 地址选项应用于流，所以 socat 可以用于许多不同的目的。

Filan是一个实用程序，可将有关其活动文件描述符的信息打印到标准输出。它是为调试socat而编写的，但也可能对其他目的有用。使用 -h 选项查找更多信息。

Procan是一个实用程序，可将有关过程参数的信息打印到标准输出。编写它是为了更好地理解一些 UNIX 进程属性和调试socat，但也可能对其他目的有用。

socat实例 的生命周期通常由四个阶段组成。

在初始化阶段，解析命令行选项并初始化日志记录。

在打开阶段，socat打开第一个地址，然后打开第二个地址。这些步骤通常是阻塞的；因此，特别是对于像 socks 这样的复杂地址类型，必须在开始下一步之前完成连接请求或身份验证对话。

在传输阶段，socat通过 监视两个流的读写文件描述符 select() ，当数据在一侧可用并且 可以写入另一侧时，socat 读取它，如果需要，执行换行符转换，然后写入数据到另一个流的写入文件描述符，然后继续在两个方向上等待更多数据。

当其中一个流有效地达到 EOF 时，关闭阶段开始。Socat将 EOF 条件转移到另一个流，即尝试仅关闭其写入流，使其有机会优雅地终止。在定义的时间内， socat继续向另一个方向传输数据，但随后关闭所有剩余的通道并终止。


选项
Socat提供了一些命令行选项来修改程序的行为。它们与用作地址规范一部分的所谓地址选项无关 。

-V
    将版本和可用功能信息打印到标准输出，然后退出。
-h | -?
    将帮助文本打印到标准输出，描述命令行选项和可用地址类型，然后退出。
-hh | -??
    像 -h 一样，加上所有可用地址选项的短名称列表。有些选项是平台相关的，所以这个输出有助于检查特定的实现。
-hhh | -???
    像 -hh 一样，加上所有可用地址选项名称的列表。
-d
    如果没有这个选项，只会生成致命和错误消息；应用此选项还会打印警告消息。有关详细信息，请参阅诊断 。
-d -d
    打印致命、错误、警告和通知消息。
-d -d -d
    打印致命、错误、警告、通知和信息消息。
-d -d -d -d
    打印致命、错误、警告、通知、信息和调试消息。
-D
    在开始传输阶段之前记录有关文件描述符的信息。
-ly[<facility>]
    将消息写入 syslog 而不是 stderr；使用 -d 选项定义的严重性。使用可选的<facility>，可以选择 syslog 类型，默认为“daemon”。第三方库可能不遵守此选项。
-lf <logfile>
    将消息写入 <logfile> [ filename ] 而不是 stderr。某些第三方库，尤其是 libwrap，可能不遵守此选项。
-ls
    将消息写入 stderr（这是默认设置）。一些第三方库可能不遵守此选项，特别是 libwrap 似乎只记录到 syslog。
-lp<progname>
    覆盖错误消息中打印的程序名称并用于构造环境变量名称。
-lu
将错误消息的时间戳扩展到微秒级分辨率。登录到 syslog 时不起作用。
-lm[<facility>]
    混合日志模式。在启动期间，消息被打印到 stderr；当socat 启动传输阶段循环或守护程序模式时（即在打开所有流之后和开始数据传输之前，或者，使用带有 fork 选项的侦听套接字，在第一次接受调用之前），它会将日志记录切换到 syslog。使用可选的<facility>，可以选择 syslog 类型，默认为“daemon”。
-lh
    将主机名添加到日志消息。使用来自环境变量 HOSTNAME 的值，或者uname()如果 HOSTNAME 未设置，则使用检索到的值。
-v
    不仅将传输的数据写入其目标流，还写入 stderr。输出格式是为便于阅读而进行了一些转换的文本，并以“>”或“<”为前缀表示流向。
-x
    不仅将传输的数据写入其目标流，还写入 stderr。输出格式为十六进制，前缀“>”或“<”表示流向。可以与 -v .
-r <file>
    将从左到右地址流动的原始（二进制）数据转储到给定文件。
-R <file>
    将从右到左地址流动的原始（二进制）数据转储到给定文件。
-b<size>
    设置数据传输块 <size> [ size_t ]。每步最多传输 <size> 个字节。默认为 8192 字节。
-s
    默认情况下，当某些选项无法应用时， socat会在发生错误时终止，以防止进程运行。使用此选项，socat会出现错误并尝试继续。即使使用此选项，socat 也会在致命错误时退出，并在安全检查失败时中止连接尝试。
-t<timeout>
    当一个通道达到 EOF 时，另一个通道的写入部分被关闭。然后，socat在终止前等待 <timeout> [ timeval ] 秒。默认值为 0.5 秒。此超时仅适用于可以独立关闭写入和读取部分的地址。当在超时间隔期间读取部分给出 EOF 时，socat 终止而不等待超时。
-T<timeout>
    总不活动超时：当 socat 已经在传输循环中并且在 <timeout> [ timeval ] 秒内没有发生任何事情（没有数据到达，没有发生中断......）然后它终止。对于无法传输 EOF 的协议（如 UDP）很有用。
-u
    使用单向模式。第一个地址仅用于读取，第二个地址仅用于写入（示例）。
-U
    反向使用单向模式。第一个地址只用于写，第二个地址只用于读。
-g
    在地址选项解析期间，不要检查该选项在给定地址环境中是否有用。如果您想强制使用它，例如，将套接字选项应用到串行设备。
-L<lockfile>
    如果 lockfile 存在，则退出并出错。如果 lockfile 不存在，则创建它并继续，在退出时取消链接 lockfile。
-W<lockfile>
    如果 lockfile 存在，则等待它消失。当 lockfile 不存在时，创建它并继续，在退出时取消链接 lockfile。
-4
    如果地址没有隐式或显式指定版本，请使用 IP 版本 4；这是默认设置。
-6
    如果地址没有隐式或显式指定版本，请使用 IP 版本 6。

地址规格
    使用地址命令行参数，用户给出socat指令和建立字节流的必要信息。

    一个地址规范通常由一个地址类型关键字、零个或多个用“:”与关键字和彼此分隔的必需地址参数以及零个或多个用“,”分隔的地址选项组成。

    关键字指定地址类型（例如，TCP4、OPEN、EXEC）。对于某些关键字，存在同义词（STDIO 为“-”，TCP4 为 TCP）。关键字不区分大小写。对于一些特殊的地址类型，可以省略关键字： 以数字开头的地址规范假定为 FD（原始文件描述符）地址；如果在第一个 ':' 或 ',' 之前找到一个 '/'，则假定为 GOPEN（通用文件打开）。

    所需的地址参数数量和类型取决于地址类型。例如，TCP4 需要服务器规范（名称或地址）和端口规范（编号或服务名称）。

    每个地址可以给出零个或多个地址选项。它们在某些方面影响地址。选项由一个选项关键字或一个选项关键字和一个值组成，以“=”分隔。选项关键字不区分大小写。为了过滤对地址类型有用的选项，每个选项都是一个选项组的成员。对于每种地址类型，都有一组允许的选项组。只能使用属于这些地址组之一的选项（选项 -g除外）。

    遵循上述模式的地址规范也称为单 地址规范。两个单个地址可以用“！！”组合 形成一个通道的双类型地址。这里，第一个地址被socat用于读取数据，第二个地址用于写入数据。没有办法只指定一个选项以应用于两个单个地址。

    通常，地址以读/写模式打开。当地址是双地址规范的一部分时，或者使用 选项 -u或-U时，地址可能仅用于读取或写入。考虑到这一点对于某些地址类型很重要。

    在 socat 1.5.0 及更高版本中，词法分析会尝试有意义地处理引号和括号，并允许对特殊字符进行转义。如果找到字符之一 ( { [ ' ，则查找相应的结束字符 - ) } ] ' - ；它们也可能是嵌套的。在这些结构中，socats 特殊字符和字符串 : , !! 没有特别处理。所有这些字符和字符串都可以用 \ 或在 "" 内转义


地址类型
    本节介绍可用的地址类型及其关键字、参数和语义。

CREATE:<filename>
    打开<filename>并 creat() 使用文件描述符进行写入。此地址类型需要只写上下文，因为 creat 无法读取打开的文件。
    不能应用像 O_LARGEFILE 这样的标志。如果您需要它们，请使用 OPEN和选项 create，create。
    <filename> 必须是有效的现有或不存在的路径。如果 <filename> 是命名管道， creat() 可能会阻塞；如果 <filename> 引用套接字，则这是一个错误。
选项组：FD、REG、NAMED
有用的选项： 模式、 用户、 group , unlink-early , unlink-late , append
另见：OPEN , GOPEN

EXEC:<command-line>
派生一个子进程，该子进程与其父进程建立通信并调用指定的程序 execvp() 。 <command-line>是一个简单的命令，其参数由单个空格分隔。如果程序名称中包含“/”，则最后一个“/”之后的部分被视为 ARGV[0]。如果程序名是相对路径，则 通过 applyexecvp() 查找程序的语义 。$PATH程序成功启动后，socatsocketpair()将数据写入进程的标准输入，并使用 默认生成的 UNIX 域套接字从其标准输出读取数据 。（示例）
选项组：FD、SOCKET、EXEC、FORK , TERMIOS
有用的选项： path , fdin , fdout , chroot , su , su-d , nofork , pty , stderr , ctty , setsid , pipe , login , sigint , sigquit
另见：SYSTEM

FD:<fdnum>
使用文件描述符<fdnum>。它必须已经作为有效的 UN*X 文件描述符存在。
选项组：FD（TERMIOS，REG，SOCKET）
另见： STDIO， STDIN， STDOUT， STDERR
GOPEN:<filename>
（通用打开）此地址类型尝试有效地处理除目录之外的任何文件系统条目。<filename>可以是相对路径或绝对路径。如果它已经存在，则检查其类型。如果是 UNIX 域套接字，则socat连接；如果连接失败， socat 会假定一个数据报套接字并使用 sendto() 调用。如果条目不是套接字，socatO_APPEND会应用 标志打开它 。如果它不存在，则使用标志打开它 O_CREAT 作为常规文件（示例）。
选项组：FD、REG、SOCKET、NAMED、OPEN
另请参阅： OPEN、 CREATE、 UNIX-CONNECT

IP-SENDTO:<host>:<protocol>
打开原始 IP 套接字。根据主机规范或选项pf，使用 IP 协议版本 4 或 6。它使用<protocol>向 <host> [ IP 地址] 发送数据包并接收来自主机的数据包，忽略来自其他主机的数据包。协议 255 使用原始套接字，其中 IP 标头是数据的一部分。
选项组：FD、SOCKET、IP4、IP6
有用的选项： pf、 ttl
另见： IP4-SENDTO、 IP6-SENDTO、 IP-RECVFROM、 IP-RECV、 UDP-SENDTO, UNIX-SENDTO

INTERFACE:<interface>
使用包含链路级数据的原始数据包与接口上连接的网络进行通信。<interface>是网络接口的名称。目前仅在 Linux 上可用。选项组：FD , SOCKET
有用的选项： pf , type
另见：ip-recv
IP4-SENDTO:<host>:<protocol>
与IP-SENDTO 类似，但始终使用 IPv4。
选项组：FD、SOCKET、IP4
IP6-SENDTO:<host>:<protocol>
与IP-SENDTO 类似，但始终使用 IPv6。
选项组：FD、SOCKET、IP6

IP-DATAGRAM:<address>:<protocol>
将传出数据发送到指定地址，该地址可能特别是广播或多播地址。如果包的源地址与RANGE或TCPWRAP 选项匹配，则检查到达本地套接字的包 。该地址类型可以例如用于实现对称或非对称广播或多播通信。
选项组：FD、SOCKET、 IP4、IP6、RANGE
有用的选项： bind、 range、 tcpwrap、 broadcast、 ip-multicast-loop、 ip-multicast-ttl , ip-multicast-if , ip-add-membership , ip-add-source-membership , ttl , tos , pf
另见： IP4-DATAGRAM , IP6-DATAGRAM , IP-SENDTO , IP-RECVFROM , IP-RECV , UDP-DATAGRAM
IP4-DATAGRAM:<host>:<protocol>
与IP-DATAGRAM 类似，但始终使用 IPv4。（示例）
选项组：FD、SOCKET、IP4、RANGE

IP6-DATAGRAM:<host>:<protocol>
与IP-DATAGRAM 类似，但始终使用 IPv6。请注意，IPv6 不知道广播。
选项组：FD、SOCKET、IP6、RANGE

IP-RECVFROM:<protocol>
打开<protocol>的原始 IP 套接字。根据选项pf，使用 IP 协议版本 4 或 6。它从未指定的对等方接收一个数据包，并可能向该对等方发送一个或多个应答数据包。这种模式对于 fork 选项特别有用，其中每个到达的数据包 - 来自任意对等方 - 都由其自己的子进程处理。这允许类似于典型的基于 UDP 的服务器（如 ntpd 或命名的服务器）的行为。
请注意，当发送方和接收方 IP 地址相同时，回复数据包可能会作为传入流量获取，因为没有端口号来区分套接字。
此地址适用于 IP-SENDTO 地址对等点（见上文）。协议 255 使用原始套接字，其中 IP 标头是数据的一部分。
请参阅有关 RECVFROM 地址的说明。
选项组：FD、SOCKET、IP4、IP6、CHILD、RANGE
有用的选项： pf、 fork、 range、 ttl、 broadcast
另见： IP4-RECVFROM、 IP6-RECVFROM、 IP-SENDTO、 IP-RECV、 UDP-RECVFROM , UNIX-RECVFROM

IP4-RECVFROM:<protocol>
与IP-RECVFROM 类似，但始终使用 IPv4。
选项组：FD、SOCKET、IP4、CHILD、RANGE

IP6-RECVFROM:<protocol>
与IP-RECVFROM 类似，但始终使用 IPv6。
选项组：FD、SOCKET、IP6、CHILD、RANGE

IP-RECV:<protocol>
打开<protocol>的原始 IP 套接字。根据选项pf，使用 IP 协议版本 4 或 6。它从多个未指定的对等方接收数据包并合并数据。无法回复。例如，它可以由 socat IP-SENDTO 地址对等方寻址。协议 255 使用原始套接字，其中 IP 标头是数据的一部分。
选项组：FD、SOCKET、IP4、IP6、RANGE
有用的选项： pf、 范围
另见： IP4-RECV、 IP6-RECV、 IP-SENDTO、 IP-RECVFROM, UDP-RECV , UNIX-RECV

IP4-RECV:<protocol>
与IP-RECV类似，但始终使用 IPv4。
选项组：FD、SOCKET、IP4、RANGE

IP6-RECV:<protocol>
与IP-RECV类似，但始终使用 IPv6。
选项组：FD、SOCKET、IP6、RANGE

OPEN:<filename>
使用 系统调用（示例） 打开<filename>。此操作在 UNIX 域套接字上失败。 注意：这种地址类型在双向模式中很少有用。 选项组：FD、REG、NAMED、OPEN 有用的选项： creat、 excl、 noatime、 nofollow、 append、 rdonly、 wronly、 lock、 readbytes、 ignoreof 另见： CREATEopen()



, GOPEN , UNIX 连接

OPENSSL:<host>:<port>
根据地址规范、名称解析或选项 pf ， 尝试使用 TCP/IP 版本 4 或 6在 <host> [ IP 地址] 上建立到 <port> [ TCP service ] 的 SSL 连接。 注意：直到 1.7.2.4 版本，服务器证书仅针对系统证书存储或cafile或 capath检查有效性，但不与服务器名称或其 IP 地址匹配。由于版本 1.7.3.0 socat 检查对等证书是否与 <host> 参数或 openssl-commonname的值匹配
选项。Socat 尝试将其与证书主题 commonName 和证书扩展 subjectAltName DNS 名称进行匹配。支持证书中的通配符。
选项组：FD，SOCKET，IP4，IP6，TCP，OPENSSL，RETRY
有用的选项： min-proto-version， cipher， verify， commonname， cafile， capath， certificate， key， compress， bind， pf , connect-timeout , sourceport , retry
另见： OPENSSL-LISTEN , TCP

OPENSSL-LISTEN:<port>
侦听tcp <端口> [ TCP 服务]。IP 版本为 4 或使用 pf指定的版本。当一个连接被接受时，这个地址就像 SSL 服务器一样。
注意：您可能希望将此地址与证书选项一起使用。
注意：客户端证书仅针对 cafile或capath检查有效性，但不与客户端名称或其 IP 地址匹配！
选项组：FD、SOCKET、IP4、IP6、TCP、LISTEN、OPENSSL、CHILD、RANGE , RETRY
有用的选项： pf , min-proto-version , cipher , verify , commonname , cafile , capath , certificate , key , compress , fork , bind , range , tcpwrap , su , reuseaddr , retry
另见： OPENSSL , TCP-听

OPENSSL-DTLS-CLIENT:<host>:<port>
根据地址规范、名称解析或选项 pf ， 尝试使用 UDP/IP 版本 4 或 6在 <host> [ IP 地址] 上建立到 <port> [ UDP service ] 的 DTLS 连接。Socat根据地址选项openssl-commonname或主机名检查对等证书 subjectAltName 或 commonName 。支持证书中的通配符。 使用socat选项-b使数据报足够小以适应网络开销。使用选项-T防止在对等方安静地关闭时无限期挂起。 选项组：FD、SOCKET


, IP4 , IP6 , OPENSSL , RETRY
有用的选项： min-proto-version , cipher , verify , commonname , cafile , capath , certificate , key , compress , bind , pf , sourceport , retry
另见： OPENSSL-DTLS-SERVER , OPENSSL -连接， UDP连接

OPENSSL-DTLS-SERVER:<port>
侦听 UDP <端口> [ UDP 服务]。IP 版本为 4 或使用 pf指定的版本。当一个连接被接受时，这个地址就像 DTLS 服务器一样。
注意：您可能希望将此地址与证书选项一起使用。
注意：客户端证书仅针对 cafile或capath检查有效性，但不与客户端名称或其 IP 地址匹配！使用socat选项-b使数据报足够小以适应网络开销。使用选项-T防止在对等方安静地关闭时无限期挂起。
选项组：FD, SOCKET , IP4 , IP6 , LISTEN , OPENSSL , CHILD , RANGE , RETRY
有用的选项： pf , min-proto-version , cipher , verify , commonname , cafile , capath , certificate , key , compress , fork , bind , range , tcpwrap , 苏, 重用地址， 重试
参见： OPENSSL-DTLS-CLIENT， OPENSSL-LISTEN， UDP-LISTEN

PIPE:<filename>
如果<filename>已存在，则将其打开。如果它不存在，则创建并打开一个命名管道。从 socat 版本 1.4.3 开始，当地址关闭时，命名管道被删除（但请参见选项unlink-close
注意：当管道用于读写时，它用作回显服务。
注意：当管道被用于读取和写入，并且 socat 尝试写入的字节数超过管道可以缓冲的字节数（Linux 2.4：2048 字节），socat 可能会阻塞。考虑使用 socat 选项，例如， -b 2048
选项组：FD、NAMED、OPEN
有用的选项： rdonly , 非阻塞, 组, user , mode , unlink-early
另见：未命名管道

PIPE
创建一个未命名的管道并将其用于读取和写入。它作为回声工作，因为写入它的所有内容都会立即显示为读取数据。
注意：当 socat 尝试写入的字节数超过管道可以排队的字节数（Linux 2.4：2048 字节）时，socat 可能会阻塞。考虑，例如，使用选项 -b 2048
选项组：FD
另见：命名管道

PROXY:<proxy>:<hostname>:<port>
使用 TCP/IP 版本 4 或 6 连接到端口 8080 上的 HTTP 代理服务器，具体取决于地址规范、名称解析或选项 pf，并为 hostname:port 发送 CONNECT 请求。如果代理授予访问权限并成功连接到目标，socat 和目标之间的数据传输就可以开始了。请注意，流量不必是 HTTP，而是可以是任意协议。
选项组：FD、SOCKET、IP4、IP6、TCP、HTTP、RETRY
有用的选项： proxyport、 ignorecr、 proxyauth、 resolve, crnl , bind , connect-timeout , mss , sourceport , retry
另见：SOCKS , TCP

PTY
生成一个伪终端 (pty) 并使用它的主端。另一个进程可能会像串行线或终端一样使用它来打开 pty 的从端。（示例）。如果 ptmx 和 openpty 机制都可用，则使用 ptmx (POSIX)。
选项组：FD、NAMED、PTY、TERMIOS
有用的选项： link、 openpty、 wait-slave、 mode、 user、 group
另见： UNIX-LISTEN、 PIPE、 EXEC、SYSTEM

READLINE
在 stdio 上使用 GNU readline 和历史记录以允许编辑和重用输入行（示例）。这需要 GNU readline 和历史库。请注意，stdio 应该是（伪）终端设备，否则 readline 似乎不起作用。
选项组：FD、READLINE、TERMIOS
有用的选项： history、 noecho
另见： STDIO

SCTP-CONNECT:<host>:<port>
根据地址规范、名称解析或选项pf ，使用 IP 版本 4 或 6 建立到指定 <host> [ IP address ] 和 <port> [ TCP service ] 的 SCTP 流连接。 选项组：FD、SOCKET、IP4、IP6、SCTP、CHILD、RETRY 有用的选项： bind、 pf、 connect-timeout、 tos、 mtudiscover、 sctp-maxseg、 sctp-nodelay、

nonblock , sourceport , retry , readbytes
另见： SCTP4-CONNECT , SCTP6-CONNECT , SCTP-LISTEN , TCP-CONNECT

SCTP4-CONNECT:<host>:<port>
与SCTP-CONNECT类似，但仅支持 IPv4 协议。
选项组：FD、SOCKET、IP4、SCTP、CHILD、RETRY

SCTP6-CONNECT:<host>:<port>
与SCTP-CONNECT类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、IP6、SCTP、CHILD、RETRY

SCTP-LISTEN:<port>
侦听<port> [ TCP service ] 并接受 SCTP 连接。IP 版本为 4 或使用地址选项pf、socat 选项（-4、-6）或环境变量SOCAT_DEFAULT_LISTEN_IP 指定的版本。请注意，打开此地址通常会阻塞，直到客户端连接。
选项组：FD，SOCKET，LISTEN，CHILD，RANGE，IP4，IP6，SCTP，RETRY
有用的选项： crnl， fork， bind, range , tcpwrap , pf , max-children , backlog , accept-timeout , sctp-maxseg , sctp-nodelay , su , reuseaddr , retry , cool-write
另见： SCTP4-LISTEN , SCTP6-LISTEN , TCP-LISTEN , SCTP -连接

SCTP4-LISTEN:<port>
与SCTP-LISTEN类似，但仅支持 IPv4 协议。
选项组：FD、SOCKET、LISTEN、CHILD、RANGE、IP4、SCTP、RETRY

SCTP6-LISTEN:<port>
与SCTP-LISTEN类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、LISTEN、CHILD、RANGE、IP6、SCTP、RETRY

SOCKET-CONNECT:<domain>:<protocol>:<remote-address>
使用第一个和第二个给定的套接字参数和SOCK_STREAM（参见 man socket(2)）创建一个流套接字并连接到远程地址。这两个套接字参数必须由int 数字指定。请查阅您的操作系统文档并包含文件以找到适当的值。远程地址必须是没有 sa_family 和 (BSD) sa_len 组件的 sockaddr 结构的数据 表示。
请注意，当您应用 socat 选项 -g时，除了指定组的选项之外，您还可以使用更高级别协议的选项。
选项组：FD、SOCKET、CHILD、RETRY
有用的选项： bind， setsockopt，
另见： TCP， UDP-CONNECT， UNIX-CONNECT， SOCKET-LISTEN， SOCKET-SENDTO

SOCKET-DATAGRAM:<domain>:<type>:<protocol>:<remote-address>
使用前三个给定套接字参数（参见 man socket(2)）创建一个数据报套接字，并将传出数据发送到远程地址。三个套接字参数必须由int 数字指定。请查阅您的操作系统文档并包含文件以找到适当的值。远程地址必须是没有 sa_family 和 (BSD) sa_len 组件的 sockaddr 结构的数据 表示。
请注意，当您应用 socat 选项 -g时，除了指定组的选项之外，您还可以使用更高级别协议的选项。
选项组：FD、SOCKET、RANGE
有用的选项： 绑定, range , setsockopt ,
另见: UDP-DATAGRAM , IP-DATAGRAM , SOCKET-SENDTO , SOCKET-RECV , SOCKET-RECVFROM

SOCKET-LISTEN:<domain>:<protocol>:<local-address>
使用第一个和第二个给定的套接字参数和SOCK_STREAM（参见 man socket(2)）创建一个流套接字，并等待本地地址上的传入连接。这两个套接字参数必须由 int数字指定。请查阅您的操作系统文档并包含文件以找到适当的值。本地地址必须是没有 sa_family 和 (BSD) sa_len 组件的 sockaddr 结构的 数据表示。
请注意，当您应用 socat 选项 -g时，除了指定组的选项之外，您还可以使用更高级别协议的选项。
选项组：FD、SOCKET、LISTEN、RANGE, CHILD , RETRY
有用的选项: setsockopt , setsockopt-listen ,
另见: TCP , UDP-CONNECT , UNIX-CONNECT , SOCKET-LISTEN , SOCKET-SENDTO , SOCKET-SENDTO

SOCKET-RECV:<domain>:<type>:<protocol>:<local-address>
使用三个给定的套接字参数（参见 man socket(2)）创建一个套接字并将其绑定到 <local-address>。接收到达的数据。这三个参数必须由int数字指定。请查阅您的操作系统文档并包含文件以找到适当的值。本地地址必须是没有 sa_family 和 (BSD) sa_len 组件的 sockaddr 结构的数据表示。
选项组：FD、SOCKET、RANGE
有用的选项： range、 setsockopt、 setsockopt-listen
另见： UDP-RECV、 IP-RECV、 UNIX-RECV, SOCKET-DATAGRAM , SOCKET-SENDTO , SOCKET-RECVFROM

SOCKET-RECVFROM:<domain>:<type>:<protocol>:<local-address>
使用三个给定的套接字参数（参见 man socket(2)）创建一个套接字并将其绑定到 <local-address>。接收到达的数据并将回复发送回发送者。前三个参数必须指定为 int数字。请查阅您的操作系统文档并包含文件以找到适当的值。本地地址必须是没有 sa_family 和 (BSD) sa_len 组件的 sockaddr 结构的 数据表示。
请参阅有关 RECVFROM 地址的说明。
选项组：FD、SOCKET、CHILD、RANGE
有用的选项： fork、 range、 setsockopt , setsockopt-listen
参见: UDP-RECVFROM , IP-RECVFROM , UNIX-RECVFROM , SOCKET-DATAGRAM , SOCKET-SENDTO , SOCKET-RECV

SOCKET-SENDTO:<domain>:<type>:<protocol>:<remote-address>
使用三个给定的套接字参数创建一个套接字（参见 man socket(2)）。将传出数据发送到给定地址并接收回复。这三个参数必须指定为int 数字。请查阅您的操作系统文档并包含文件以找到适当的值。远程地址必须是没有 sa_family 和 (BSD) sa_len 组件的 sockaddr 结构的数据 表示。
选项组：FD、SOCKET
有用的选项： bind、 setsockopt、 setsockopt-listen
另见： UDP-SENDTO、 IP-SENDTO、 UNIX-SENDTO, SOCKET-DATAGRAM , SOCKET-RECV SOCKET-RECVFROM
SOCKS4:<socks-server>:<host>:<port>
通过 <socks-server> [ IP address ] 连接到<port> [ TCP service ] 上的 <host> [ IPv4 address ] ，根据地址规范、名称解析或选项 pf在 IP 版本 4 或 6 上使用 socks 版本 4 协议（示例）。 选项组：FD、SOCKET、IP4、IP6、TCP、SOCKS4、RETRY 有用的选项： socksuser、 socksport、 sourceport、 pf、 retry 另见：


SOCKS4A , 代理, TCP
SOCKS4A:<socks-server>:<host>:<port>
与SOCKS4类似，但使用 socks 协议版本 4a，因此将主机名解析留给 socks 服务器。
选项组：FD、SOCKET、IP4、IP6、TCP、SOCKS4、RETRY

STDERR
使用文件描述符 2.
选项组：FD ( TERMIOS , REG , SOCKET )
另见：FD

STDIN
使用文件描述符 0。
选项组：FD ( TERMIOS , REG , SOCKET )
有用的选项： readbytes
另见：FD

STDIO
使用文件描述符 0 进行读取，使用 1 进行写入。
选项组：FD ( TERMIOS , REG , SOCKET )
有用的选项： readbytes
另见：FD

STDOUT
使用文件描述符 1.
选项组：FD ( TERMIOS , REG , SOCKET )
另见：FD

SYSTEM:<shell-command>
派生一个子进程，该子进程与其父进程建立通信并调用指定的程序 system() 。请注意 <shell-command> [ string ] 不能包含 ',' 或 "!!"，并且 shell 元字符可能必须受到保护。程序启动成功后，socat将数据写入进程的 stdin 并从其 stdout 读取。
选项组：FD、SOCKET、EXEC、FORK、TERMIOS
有用的选项： path、 fdin、 fdout、 chroot、 su、 su-d, nofork , pty , stderr , ctty , setsid , 管道, sigint , sigquit
另见：EXEC

TCP:<host>:<port>
根据地址规范、名称解析或选项 pf使用 TCP/IP 版本 4 或 6 连接到<host> [ IP 地址] 上 的 <port> [ TCP 服务] 。 选项组：FD、SOCKET、IP4、IP6、TCP、RETRY 有用的选项： crnl、 bind、 pf、 connect-timeout、 tos、 mtudiscover、 mss、 nodelay、 nonblock、 sourceport

, retry , readbytes
另见： TCP4 , TCP6 , TCP-LISTEN , UDP , SCTP-CONNECT , UNIX-CONNECT
TCP4:<host>:<port>
与TCP类似，但仅支持 IPv4 协议（示例）。
选项组：FD、SOCKET、IP4、TCP、RETRY

TCP6:<host>:<port>
与TCP类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、IP6、TCP、RETRY

TCP-LISTEN:<port>
侦听<port> [ TCP service ] 并接受 TCP/IP 连接。IP 版本为 4 或使用地址选项pf、socat 选项（-4、-6）或环境变量SOCAT_DEFAULT_LISTEN_IP 指定的版本。请注意，打开此地址通常会阻塞，直到客户端连接。
选项组：FD、SOCKET、LISTEN、CHILD、RANGE、IP4、IP6、TCP、RETRY
有用的选项： crnl、 fork、 bind, range , tcpwrap , pf , max-children , backlog , accept-timeout , mss , su , reuseaddr , retry , cool-write
另见： TCP4-LISTEN , TCP6-LISTEN , UDP-LISTEN , SCTP-LISTEN , UNIX-LISTEN , OPENSSL-LISTEN , TCP-连接

TCP4-LISTEN:<port>
与TCP-LISTEN类似，但仅支持 IPv4 协议（示例）。
选项组：FD、SOCKET、LISTEN、CHILD、RANGE、IP4、TCP、RETRY

TCP6-LISTEN:<port>
与TCP-LISTEN类似，但仅支持 IPv6 协议。
其他有用的选项： ipv6only
选项组：FD、SOCKET、LISTEN、CHILD、RANGE、IP6、TCP、RETRY

TUN[:<if-addr>/<bits>]
创建一个 Linux TUN/TAP 设备，并有选择地为其分配参数给定的地址和网络掩码。生成的网络接口几乎可以供其他进程使用；socat 服务于它的“有线侧”。此地址通常需要对隧道克隆设备具有读写权限 /dev/net/tun ，以及设置一些权限ioctl()s。 需要选项iff-up才能立即激活界面！
注意：如果您打算在两个 Socat“有线端”之间传输数据包，您需要一个保持数据包边界的协议，例如 UDP；TCP 可能与选项nodelay 一起使用。
选项组：FD、NAMED、OPEN、TUN
有用的选项： iff-up , tun-device , tun-name , tun-type , iff-no-pi
另见： ip-recv

UDP:<host>:<port>
根据地址规范、名称解析或选项 pf使用 UDP/IP 版本 4 或 6 连接到 < host> [ IP 地址] 上 的 <port> [ UDP 服务] 。 请注意，由于 UDP 协议属性，没有建立真正的连接；必须发送数据以“连接”到服务器，并且不能传输文件结束条件。 选项组：FD、SOCKET、IP4、IP6 有用的选项： ttl、 tos、 bind、 sourceport、 pf 另见： UDP4



, UDP6 , UDP-LISTEN , TCP , IP

UDP4:<host>:<port>
与UDP类似，但仅支持 IPv4 协议。
选项组：FD、SOCKET、IP4

UDP6:<host>:<port>
与UDP类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、IP6

UDP-DATAGRAM:<address>:<port>
将传出数据发送到指定地址，该地址可能特别是广播或多播地址。仅当使用选项sourceport（这是Socat版本 1.7.4.0 的更改）并且它们的源地址与 RANGE或TCPWRAP 选项匹配时，才会检查到达本地套接字的数据包是否具有正确的远程端口。该地址类型可以例如用于实现对称或非对称广播或多播通信。
选项组：FD、SOCKET、IP4、IP6、RANGE
有用的选项： bind、 range, tcpwrap , 广播, ip-multicast-loop , ip-multicast-ttl , ip-multicast-if , ip-add-membership , ip-add-source-membership , ttl , tos , sourceport , pf
另见： UDP4-DATAGRAM , UDP6-DATAGRAM , UDP-SENDTO , UDP-RECVFROM , UDP-RECV , UDP-CONNECT , UDP-LISTEN , IP-DATAGRAM

UDP4-DATAGRAM:<address>:<port>
与UDP-DATAGRAM 类似，但仅支持 IPv4 协议（example1、 example2）。
选项组：FD、SOCKET、IP4、RANGE

UDP6-DATAGRAM:<address>:<port>
与UDP-DATAGRAM 类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、IP6、RANGE

UDP-LISTEN:<port>
等待 UDP/IP 数据包到达 <port> [ UDP service ] 并“连接”回发送者。接受的 IP 版本是 4 或使用选项 pf指定的版本。请注意，由于 UDP 协议属性，没有建立真正的连接；数据必须首先从对等方到达，并且不能传输文件结束条件。请注意，打开此地址通常会阻塞，直到客户端连接。
选项组：FD、SOCKET、LISTEN、CHILD、RANGE、IP4、IP6
有用的选项： fork、 bind、 range, pf
另见： UDP , UDP4-LISTEN , UDP6-LISTEN , TCP-LISTEN

UDP4-LISTEN:<port>
与UDP-LISTEN类似，但仅支持 IPv4 协议。
选项组：FD、SOCKET、LISTEN、CHILD、RANGE、IP4

UDP6-LISTEN:<port>
与UDP-LISTEN类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、LISTEN、CHILD、RANGE、IP6

UDP-SENDTO:<host>:<port>
根据地址规范、名称解析或选项 pf使用 UDP/IP 版本 4 或 6 与指定的对等套接字通信，该套接字由<host> [ IP 地址] 上的 <port> [ UDP service ] 定义。它仅向该对等套接字发送数据包并从该对等套接字接收数据包。这个地址有效地实现了一个数据报客户端。它适用于 socat UDP-RECVFROM 和 UDP-RECV 地址对等点。 选项组：FD、SOCKET、IP4、IP6 有用的选项： ttl、 tos、 bind、 sourceport、 pf


参见： UDP4-SENDTO、 UDP6-SENDTO、 UDP-RECVFROM、 UDP-RECV、 UDP-CONNECT、 UDP-LISTEN、 IP-SENDTO

UDP4-SENDTO:<host>:<port>
与UDP-SENDTO 类似，但仅支持 IPv4 协议。
选项组：FD、SOCKET、IP4

UDP6-SENDTO:<host>:<port>
与UDP-SENDTO 类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、IP6

UDP-RECVFROM:<port>
根据选项pf使用 UDP/IP 版本 4 或 6 在 <port> [ UDP service ] 上创建 UDP 套接字。它从未指定的对等方接收一个数据包，并可能向该对等方发送一个或多个应答数据包。此模式对于 fork选项特别有用，其中每个到达的数据包 - 来自任意对等方 - 由其自己的子进程处理。这允许类似于典型的基于 UDP 的服务器（如 ntpd 或命名的服务器）的行为。此地址适用于 socat UDP-SENDTO 地址对等点。
注意：当第二个地址在进入传输循环之前失败时，数据包将被丢弃。在第二个地址上使用选项重试或 永远，以避免数据丢失。
选项组：FD、SOCKET、IP4、IP6、CHILD、RANGE
有用的选项： fork、 ttl、 tos、 bind、 sourceport、 pf
另见： UDP4-RECVFROM、 UDP6-RECVFROM、 UDP-SENDTO、 UDP-RECV, UDP-CONNECT , UDP-LISTEN , IP-RECVFROM , UNIX-RECVFROM

UDP4-RECVFROM:<port>
与UDP-RECVFROM 类似，但仅支持 IPv4 协议。
选项组：FD、SOCKET、IP4、CHILD、RANGE

UDP6-RECVFROM:<port>
与UDP-RECVFROM 类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、IP6、CHILD、RANGE

UDP-RECV:<port>
根据选项pf使用 UDP/IP 版本 4 或 6 在 <port> [ UDP service ] 上创建 UDP 套接字。它从多个未指定的对等方接收数据包并合并数据。无法回复。它适用于例如 socat UDP-SENDTO地址对等点；它的行为类似于 syslog 服务器。 注意：如果您需要fork选项，请在单向模式下使用UDP-RECVFROM （使用选项 -u）。 选项组：FD、SOCKET、IP4、IP6、RANGE 有用的选项： pf、 bind


, sourceport , ttl , tos
另见： UDP4-RECV , UDP6-RECV , UDP-SENDTO , UDP-RECVFROM , UDP-CONNECT , UDP-LISTEN , IP-RECV , UNIX-RECV

UDP4-RECV:<port>
与UDP-RECV类似，但仅支持 IPv4 协议。
选项组：FD、SOCKET、IP4、RANGE

UDP6-RECV:<port>
与UDP-RECV类似，但仅支持 IPv6 协议。
选项组：FD、SOCKET、IP6、RANGE

UNIX-CONNECT:<filename>
连接到<filename>假设它是一个 UNIX 域套接字。如果 <filename> 不存在，这是一个错误；如果 <filename> 不是 UNIX 域套接字，则这是一个错误；如果 <filename> 是 UNIX 域套接字，但没有进程正在侦听，则这是一个错误。
选项组：FD、SOCKET、NAMED、RETRY、UNIX
）有用的选项： bind
另见： UNIX-LISTEN、 UNIX-SENDTO、 TCP

UNIX-LISTEN:<filename>
使用 UNIX 域流套接字 侦听<filename>并接受连接。如果 <filename> 存在并且不是套接字，则这是一个错误。如果 <filename> 存在并且是 UNIX 域套接字，则绑定到地址失败（使用选项unlink-early！）。请注意，打开此地址通常会阻塞，直到客户端连接。从 socat 版本 1.4.3 开始，当此地址关闭时，文件系统条目将被删除（但请参见选项unlink-close）（示例）。
选项组：FD、SOCKET、NAMED、LISTEN、CHILD、RETRY、UNIX
有用的选项： fork、 umask、 mode、 user、 group、 unlink-early
另见： UNIX-CONNECT、 UNIX-RECVFROM、 UNIX-RECV、 TCP-LISTEN

UNIX-SENDTO:<filename>
与指定的对等套接字通信，由 [ <filename> ] 定义，假设它是一个 UNIX 域数据报套接字。它仅向该对等套接字发送数据包并从该对等套接字接收数据包。请注意，可能需要将本地套接字绑定到一个地址（例如/tmp/sock1，以前不能存在）。此地址类型适用于 socat UNIX-RECVFROM 和 UNIX-RECV 地址对等体。
选项组：FD、SOCKET、NAMED、UNIX
有用的选项： bind
另见： UNIX-RECVFROM、 UNIX-RECV、 UNIX-CONNECT、 UDP-SENDTO, IP-SENDTO

UNIX-RECVFROM:<filename>
创建一个 UNIX 域数据报套接字 [ <filename> ]。接收一个数据包并可能向该对等方发送一个或多个应答数据包。这种模式对于 fork 选项特别有用，其中每个到达的数据包 - 来自任意对等方 - 都由其自己的子进程处理。此地址适用于 socat UNIX-SENDTO 地址对等点。
选项组：FD、SOCKET、NAMED、CHILD、UNIX
请参阅有关 RECVFROM 地址的说明。
有用的选项： fork
另见： UNIX-SENDTO， UNIX-RECV， UNIX-LISTEN， UDP-RECVFROM , IP-RECVFROM

UNIX-RECV:<filename>
创建一个 UNIX 域数据报套接字 [ <filename> ]。从多个未指定的对等方接收数据包并合并数据。无法回复。例如，它可以由 socat UNIX-SENDTO 地址对等点寻址。它的行为类似于 syslog 服务器。
选项组：FD、SOCKET、NAMED、UNIX
另见： UNIX-SENDTO、 UNIX-RECVFROM、 UNIX-LISTEN、 UDP-RECV、 IP-RECV

UNIX-CLIENT:<filename>
与指定的对等套接字通信，由 [ <filename> ] 定义，假设它是一个 UNIX 域套接字。它首先尝试连接，如果连接失败，则假定它是一个数据报套接字，因此支持这两种类型。
选项组：FD、SOCKET、NAMED、UNIX
有用的选项： bind
另见： UNIX-CONNECT、 UNIX-SENDTO、 GOPEN

VSOCK-CONNECT:<cid>:<port>
建立到指定 <cid> [ VSOCK cid ] 和 <port> [ VSOCK port ] 的 VSOCK 流连接。
选项组：FD、SOCKET、CHILD、RETRY
有用的选项： bind、 pf、 connect-timeout、 retry、 readbytes
另见： VSOCK-LISTEN、

VSOCK-LISTEN:<port>
侦听<port> [ VSOCK 端口] 并接受 VSOCK 连接。请注意，打开此地址通常会阻塞，直到客户端连接。
选项组：FD、SOCKET、LISTEN、CHILD、RETRY
有用的选项： fork、 bind、 pf、 max-children、 backlog、 su、 reuseaddr、 retry、 cool-write
另见： VSOCK-CONNECT
ABSTRACT-CONNECT:<string>
ABSTRACT-LISTEN:<string>
ABSTRACT-SENDTO:<string>
ABSTRACT-RECVFROM:<string>
ABSTRACT-RECV:<string>
ABSTRACT-CLIENT:<string>
ABSTRACT 地址几乎与相关的 UNIX 地址相同，只是它们不寻址基于文件系统的套接字，而是替代 UNIX 域地址空间。为此，套接字地址字符串在内部以“\0”为前缀。此功能（仅？）在 Linux 上可用。选项组与相关的 UNIX 地址相同，只是 ABSTRACT 地址不是 NAMED 组的成员。

地址选项
地址选项可应用于地址规范，以影响打开地址的过程和结果数据通道的属性。

由于技术原因，并非每个选项都可以应用于每种地址类型；例如，将套接字选项应用于常规文件将失败。为了早在开放阶段就捕捉到大多数无用的组合，引入了期权组的概念。每个选项属于一个或多个选项组。选项只能与支持至少一个选项组的地址类型一起使用（但请参阅选项 -g）。

地址选项具有其值必须符合的数据类型。每个地址选项仅包含一个关键字或一个关键字后跟“=value”，其中 value 必须符合选项类型。一些地址选项操纵系统调用的参数；例如，选项同步设置调用的 O_SYNC 标志 open() 。其他选项会导致系统或库调用；例如，使用选项 `ttl=value' setsockopt(fd, SOL_IP, IP_TTL, value, sizeof(int)) 会应用调用。其他选项设置在数据传输期间使用的内部socat变量；例如，'crnl' 会导致显式字符转换。一些选项具有更复杂的实现；chroot() 例如，su-d (substuser-delayed) 查询一些用户和组信息，存储它们，并在可能的调用 之后应用它们 。

如果一个地址有多个选项，它们在地址规范中的顺序（几乎）对它们的执行/应用顺序没有影响。相反，socat建立了一个期权阶段模型，试图将期权按有用的顺序排列。一些选项以不同的形式存在（例如，unlink、unlink-early、unlink-late）来控制它们的执行时间。

如果在一个地址规范中多次指定相同的选项，具有相同或不同的值，则效果取决于选项的种类。导致函数调用的选项 setsockopt() 会导致多次调用。对于为所需调用设置参数 open() 或设置内部标志的选项，最后一个选项出现的值是有效的。

许多选项的存在或语义取决于系统。Socat 通常不会尝试模拟缺少的 libc 或内核功能，它只是提供到底层系统的接口。因此，如果操作系统缺少某个功能，则相关选项在此平台上根本不可用。

以下段落仅介绍更常见的地址选项。如需更全面的参考并查找有关规范选项名称、别名名称、选项阶段和平台的信息，请参阅文件xio.help。


FD 选项组

此选项组包含应用于 UN*X 样式文件描述符的选项，无论它是如何生成的。因为当前所有的socat地址类型都是基于文件描述符的，所以这些选项可以应用于任何地址。
注意：其中一些选项也是另一个选项组的成员，该组提供另一种非 fd 机制。对于这些选项，它取决于实际地址类型及其选项组使用哪种机制。第二种，非基于 fd 的机制被优先考虑。

cloexec=<bool>
使用系统调用 将FD_CLOEXEC 标志 设置 为值<bool>。如果设置，文件描述符在 家庭函数调用时关闭。Socat 在 内部为它控制的 fds 处理这个标志，所以在大多数情况下不需要应用这个选项。 fcntl()exec()
setlk
尝试使用 fcntl(fd, F_SETLK, ...) 系统调用为整个文件设置任意写入锁定。如果文件已被锁定，则此调用会导致错误。在 Linux 上，当组的文件权限为“S”（gx,g+s），并且文件系统使用“mand”选项本地挂载时，锁定是强制性的，即阻止其他进程打开文件。
setlkw
尝试使用 fcntl(fd, F_SETLKW, ...) 系统调用为整个文件设置任意等待写锁定。如果文件已被锁定，则此调用将阻塞。有关强制执行此锁定的信息，请参见选项setlk。
setlk-rd
尝试使用 fcntl(fd, F_SETLK, ...) 系统调用为整个文件设置任意读取锁定。如果文件已被写锁定，则此调用会导致错误。有关强制执行此锁定的信息，请参见选项setlk。
setlkw-rd
尝试使用 fcntl(fd, F_SETLKW, ...) 系统调用为整个文件设置任意等待读取锁定。如果文件已被写锁定，则此调用将阻塞。有关强制执行此锁定的信息，请参见选项setlk。
flock-ex
尝试使用 flock(fd, LOCK_EX) 系统调用为文件设置阻塞排他咨询锁。如果文件被另一个进程锁定， Socat 会在此调用中挂起。
flock-ex-nb
尝试使用 flock(fd, LOCK_EX|LOCK_NB) 系统调用为文件设置非阻塞排他咨询锁。如果文件已被锁定，则此选项会导致错误。
flock-sh
尝试使用 flock(fd, LOCK_SH) 系统调用为文件设置阻塞共享咨询锁。如果文件被另一个进程锁定， Socat 会在此调用中挂起。
flock-sh-nb
尝试使用 flock(fd, LOCK_SH|LOCK_NB) 系统调用为文件设置非阻塞共享咨询锁。如果文件已被锁定，则此选项会导致错误。
lock
在文件上设置阻塞锁。根据特定平台上的可用性使用 setlk 或flock 机制。如果两者都可用，则使用 POSIX 变体 (setlkw)。
user=<user>
设置流的<user>（所有者）。如果地址是 NAMED 选项组的成员， socatchown() 在打开文件或绑定到 UNIX 域套接字（竞争条件！）后使用 系统调用。如果没有文件系统条目，socat使用 fchown() 系统调用设置流的用户。这些调用可能需要 root 权限。
user-late=<user>
在打开或连接通道后，通过系统调用将 fd 的所有者设置为<user> 。fchown()这仅对文件系统条目有用。
group=<group>
设置流的<group>。如果地址是 NAMED 选项组的成员， socatchown() 在打开文件或绑定到 UNIX 域套接字（竞争条件！）后使用 系统调用。如果没有文件系统条目，socatfchown()会使用 系统调用设置流的组 。这些调用可能需要组成员身份或 root 权限。
group-late=<group>
在打开或连接通道后，通过系统调用将 fd 的组设置为<group> 。fchown()这仅对文件系统条目有用。
mode=<mode>
设置流的 <mode> [ mode_t ]（权限）。如果地址是 NAMED 选项组的成员并使用 open() or creat() 调用，则模式适用于这些。如果地址是 NAMED 选项组的成员而不使用这些系统调用，socatchmod() 在打开文件系统条目或绑定到 UNIX 域套接字（竞争条件！）后使用 系统调用。否则，socat使用 fchmod() . 这些调用可能需要所有权或 root 权限。
perm-late=<mode>
在打开或连接通道后使用系统调用 将 fd 的权限设置为值 <mode> [ mode_t ] 。fchmod()这仅对文件系统条目有用。
append=<bool>
始终将数据写入文件的实际末尾。如果地址是 OPEN 选项组的成员， socat将 O_APPEND 标志与 open() 系统调用一起使用（示例）。否则，socat应用 fcntl(fd, F_SETFL, O_APPEND) 调用。
nonblock=<bool>
尝试以非阻塞模式打开或使用文件。它的唯一作用是 connect() TCP 地址的调用不会阻塞，打开命名管道进行读取也不会阻塞。如果地址是 OPEN 选项组的成员， socat将 O_NONBLOCK 标志与 open() 系统调用一起使用。否则，socat应用 fcntl(fd, F_SETFL, O_NONBLOCK) 调用。
binary
以二进制模式打开文件以避免隐式行终止符转换 (Cygwin)。
text
以文本模式打开文件以强制进行隐式行终止符转换 (Cygwin)。
noinherit
不在衍生进程 (Cygwin) 中保持此文件打开。
cool-write
当使用 EPIPE 或 ECONNRESET 写入失败并使用通知级别而不是错误记录消息时，这很容易。当 socat 用作客户端经常中止连接的大容量服务器或代理时，这可以防止日志文件被无用的错误消息填充。
此选项是实验性的。
end-close
将结束连接的（地址相关）方法更改为仅关闭文件描述符。当连接要被其他进程重用或共享时，这很有用（示例）。
通常，套接字连接将shutdown(2)终止，即使它由多个进程共享，也会终止套接字。 close(2)从进程中“取消链接”套接字，但只要仍有来自其他进程的链接，它就会保持活动状态。
同样，当一个 EXEC 或 SYSTEM 类型的地址结束时，socat 通常会显式地终止子进程。使用此选项，它只会关闭文件描述符。
shut-none
将关闭连接写入部分的（地址相关）方法更改为不执行任何操作。
shut-down
将关闭连接写入部分的（地址相关）方法更改为 shutdown(fd, SHUT_WR)。仅对套接字有用。
shut-close
将关闭连接写入部分的（地址相关）方法更改为 close(fd)。
shut-null
当一个地址指示 EOF 时，socat会向另一个地址的写通道发送一个 0 大小的数据包，以传输 EOF 条件。这对 UDP 和其他数据报协议很有用。已经使用选项null-eof针对 netcat 和 socat 进行了测试。
null-eof
通常socat将忽略到达数据报套接字的空（零大小有效负载）数据包，因此它可以在端口扫描中幸存下来。使用此选项， socat 将空数据报包解释为 EOF 指示符（请参阅 shut-null）。
ioctl-void=<request>
ioctl()使用请求值作为第二个参数和 NULL 作为第三个参数调用 。此选项允许使用未在 socat 中显式实现的 ioctl。
ioctl-int=<request>:<value>
调用ioctl()请求值作为第二个参数，整数值作为第三个参数。
ioctl-intp=<request>:<value>
ioctl()使用请求值作为第二个参数和指向整数值的指针作为第三个参数调用 。
ioctl-bin=<request>:<value>
ioctl()使用请求值作为第二个参数和指向给定数据值的指针作为第三个参数调用 。此数据必须以 <dalan>形式指定。
ioctl-string=<request>:<value>
ioctl()使用请求值作为第二个参数和指向给定字符串的指针作为第三个参数调用 。<dalan>形式。

NAMED 项组

这些选项适用于文件系统条目。
请注意，对于 UNIX 域客户端地址，这意味着绑定条目，而不是目标/对等条目。
另请参阅选项用户、组和 模式。

user-early=<user>
使用系统调用在访问文件系统条目之前 更改它的<user> （所有者）。chown()此调用可能需要 root 权限。
group-early=<group>
使用系统调用在访问文件系统条目之前 更改它的<group> 。chown()此调用可能需要组成员身份或 root 权限。
perm-early=<mode>
使用系统调用在访问文件系统条目之前 更改它的 <mode> [ mode_t ]。chmod()此调用可能需要所有权或 root 权限。
umask=<mode>
在访问文件系统条目之前将进程的 umask 设置为 <mode> [ mode_t ]（对 UNIX 域套接字有用！）。此调用可能会影响socat进程的所有进一步操作！
unlink-early
在打开文件之前，甚至在应用用户早期等之前取消链接（删除）文件。
unlink
在访问文件之前取消链接（删除）文件，但在用户早期等之后。
unlink-late
打开文件后取消链接（删除）文件，使其在短暂的竞争条件后无法被其他进程访问。
unlink-close
关闭地址时删除地址文件系统条目。对于命名管道、 UNIX域套接字和pty地址的符号链接，默认为1；对于创建的文件、 打开的文件和 通用打开的文件，默认值为 0。

OPEN 选项组

OPEN 组选项允许使用 open() 系统调用设置标志。例如，选项 'creat' 设置 O_CREAT 标志。
另请参阅选项append和 nonblock。

creat=<bool>
如果文件不存在，则创建文件（示例）。
dsync=<bool>
阻止 write() 调用，直到元信息被物理写入媒体。
excl=<bool>
使用选项 creat，如果文件存在，这是一个错误。
largefile=<bool>
在 32 位系统上，允许大于 2^31 字节的文件。
noatime
设置 O_NOATIME 选项，因此读取不会更改访问时间戳。
noctty=<bool>
不使该文件成为控制终端。
nofollow=<bool>
不遵循符号链接。
nshare=<bool>
不允许与其他进程共享此文件。
rshare=<bool>
不允许其他进程打开此文件进行写入。
rsync=<bool>
阻塞 write() 直到元信息被物理写入媒体。
sync=<bool>
阻塞 write() 直到数据被物理写入介质。
rdonly=<bool>
以只读方式打开文件。
wronly=<bool>
打开文件仅用于写入。
trunc
在打开文件期间将文件截断为大小为 0。

REG 和 BLK 选项组

这些选项通常应用于 UN*X 文件描述符，但它们的语义仅对支持随机访问的文件有意义。

seek=<offset>
应用 lseek(fd, <offset>, SEEK_SET) (or lseek64 ) 系统调用，从而将文件指针绝对定位到 <offset> [ off_t或off64_t ]。请注意，缺失值默认为 1，而不是 0。
seek-cur=<offset>
应用 lseek(fd, <offset>, SEEK_CUR) (or lseek64 ) 系统调用，从而将文件指针 <offset> [ off_t或 off64_t ] 字节相对于其当前位置（通常为 0）定位。请注意，缺失值默认为 1，而不是 0。
seek-end=<offset>
应用 lseek(fd, <offset>, SEEK_END) (or lseek64 ) 系统调用，从而将文件指针 <offset> [ off_t或 off64_t ] 字节相对于文件当前结尾定位。请注意，缺失值默认为 1，而不是 0。
ftruncate=<offset>
应用 ftruncate(fd, <offset>) （或 ftruncate64 如果可用）系统调用，从而在 <offset> [ off_t或 off64_t ] 位置截断文件。请注意，缺失值默认为 1，而不是 0。

secrm=<bool>
unrm=<bool>
compr=<bool>
fs-sync=<bool>
immutable=<bool>
fs-append=<bool>
nodump=<bool>
fs-noatime=<bool>
journal-data=<bool>
notail=<bool>
dirsync=<bool>
这些选项更改操作系统和支持这些功能的文件系统上的非标准文件属性，例如带有 ext2fs 和后继者、xfs 或 reiserfs 的 Linux。有关这些选项的信息，请参见 man 1 chattr。请注意，在创建文件和应用这些选项之间可能存在竞争条件。

PROCESS 选项组

该组的选项更改进程属性，而不仅仅是影响一个数据通道。对于 EXEC 和 SYSTEM 地址以及带有选项 FORK 的 LISTEN 和 CONNECT 类型地址，这些选项适用于子进程而不是主 socat 进程。

chroot=<directory>
处理完地址后对<directory> 执行 chroot() 操作（示例）。此调用可能需要 root 权限。
chroot-early=<directory>
在打开地址之前对<directory> 执行 chroot() 操作。此调用可能需要 root 权限。
setgid=<group>
处理地址后 更改进程的主<group> 。此调用可能需要 root 权限。请注意，此选项不会删除其他组相关权限。
setgid-early=<group>
与setgit 类似，但在打开地址之前执行。
setuid=<user>
处理地址后 更改进程的<user> （所有者）。此调用可能需要 root 权限。请注意，此选项不会删除组相关权限。检查选项 su是否更适合您的需求。
setuid-early=<user>
与setuid类似，但在打开地址之前执行。
su=<user>
在处理地址（示例）后 更改进程的<user> （所有者）和组。此调用可能需要 root 权限。
su-d=<user>
的简称substuser-delayed。 在处理地址（示例）后更改进程的<user> （所有者）和组。用户和他的组在可能的 . 此调用可能需要 root 权限。 chroot()
setpgid=<pid_t>
使进程成为指定进程组 <pid_t>的成员。如果没有给出值，或者该值为 0 或 1，则该进程将成为新进程组的领导者。
setsid
使进程成为新会话的领导者（示例）。

READLINE 选项组

这些选项适用于 readline 地址类型。

history=<filename>
从/向<filename> 读取和写入历史记录（示例）。
noprompt
从 1.4.0 版本开始，socat 默认尝试确定一个提示 - 然后传递给 readline 调用 - 通过记住输出的最后一个不完整的行。使用此选项，socat 不会将提示传递给 readline，因此它会在终端的第一列开始行编辑。
noecho=<pattern>
为提示指定常规模式，以防止以下输入行显示在屏幕上和添加到历史记录中。提示定义为在最后一个换行符之后和输入字符之前输出到 readline 地址的文本。该模式是一个正则表达式，例如“^[Pp]assword:.*$”或“([Uu]ser:|[Pp]assword:)”。有关详细信息，请参见正则表达式 (7)。（示例）
prompt=<string>
将字符串作为提示传递给 readline 函数。readline 在单步浏览历史时打印此提示。如果此字符串与交互程序在另一个 socat 地址上发出的恒定提示匹配，则可以实现一致的外观。

应用选项组

该组包含在数据级别工作的选项。请注意，这些选项仅适用于 socat 传输的“原始”数据，而不适用于 PROXY等地址使用的协议数据。

cr
在此通道上写入/读取时，将默认行终止字符 NL ('\n', 0x0a) 转换为 CR ('\r', 0x0d)。
crnl
在此通道上写入/读取时，将默认行终止字符 NL ('\n', 0x0a) 转换为 CRNL ("\r\n", 0x0d0a) （示例）。注意：socat 只是去除所有 CR 字符。
ignoreeof
当此通道上发生 EOF 时，socat会忽略它并尝试读取更多数据（如“tail -f”）（示例）。
readbytes=<bytes>
socat仅从该地址读取这么多字节（该地址仅提供这么多字节用于传输，并假装在 EOF 之后）。必须大于 0。
lockfile=<filename>
如果 lockfile 存在，则退出并出错。如果 lockfile 不存在，则创建它并继续，在退出时取消链接 lockfile。
waitlock=<filename>
如果 lockfile 存在，则等待它消失。当 lockfile 不存在时，创建它并继续，在退出时取消链接 lockfile。
escape=<int>
指定在输入流上触发 EOF 的字符的数字代码。它在原始模式下的终端上很有用（示例）。

SOCKET 选项组

这些选项适用于所有类型的套接字，例如 IP 或 UNIX 域。大多数都是通过 setsockopt() 电话申请的。

bind=<sockname>
bind() 使用系统调用 将套接字绑定到给定的套接字地址 。<sockname> 的形式取决于套接字域：IP4 和 IP6 允许 [hostname|hostaddress][:(service|port)] 形式（示例），UNIX 域套接字需要<filename>，VSOCK 允许 [cid][ 形式：（港口）]。
connect-timeout=<seconds>
在 <seconds> [ timeval ] 后以错误状态中止连接尝试。
so-bindtodevice=<interface>
将套接字绑定到给定的<interface>。此选项可能需要 root 权限。
broadcast
对于数据报套接字，允许发送到广播地址和接收寻址到广播地址的数据包。
debug
启用套接字调试。
dontroute
仅与直接连接的对等方通信，不使用路由器。
keepalive
启用在套接字上发送保活。
linger=<seconds>
阻塞 shutdown() 或 close() 直到数据传输完成或给定的超时 [ int ] 过期。
oobinline
在输入数据流中放置带外数据。
priority=<priority>
为传出数据包 设置定义的协议 <priority> [ <int> ]。
rcvbuf=<bytes>
socket() 在调用 <bytes> [ int ] 之后设置接收缓冲区的大小 。对于 TCP 套接字，此值对应于套接字的最大窗口大小。
rcvbuf-late=<bytes>
当套接字已经连接到 <bytes> [ int ] 时，设置接收缓冲区的大小。对于 TCP 套接字，此值对应于套接字的最大窗口大小。
rcvlowat=<bytes>
指定在套接字层将缓冲数据传递给socat之前 接收到的最小字节数 [ int ] 。
reuseaddr
允许其他套接字绑定到一个地址，即使它的一部分（例如本地端口）已被socat使用（示例）。
sndbuf=<bytes>
socket() 在调用 <bytes> [ int ] 之后设置发送缓冲区的大小 。
sndbuf-late=<bytes>
当套接字连接到 <bytes> [ int ] 时设置发送缓冲区的大小。
sndlowat=<bytes>
指定发送缓冲区中的最小字节数，直到套接字层将数据发送到 <bytes> [ int ]。
pf=<string>
强制使用指定的 IP 版本或协议。<string> 可以是“ip4”或“ip6”之类的东西。结果值用作 socket() or socketpair() 调用的第一个参数。此选项影响地址解析以及绑定和范围选项所需的语法。
socktype=<type>
将套接字的类型（指定为 socket() or socketpair() 调用的第二个参数）设置为 <type> [ int ]。地址解析不受此选项的影响。在 Linux 下，1 表示面向流的套接字，2 表示数据报套接字，3 表示原始套接字，5 seqpacket（流保持数据包边界）。
protocol
将套接字的协议（指定为 socket() or socketpair() 调用的第三个参数）设置为 <protocol> [ int ]。地址解析不受此选项的影响。6 表示 TCP，17 表示 UDP。
reuseport
设置 SO_REUSEPORT 套接字选项。
so-timestamp
设置 SO_TIMESTAMP 套接字选项。这允许接收和记录时间戳辅助消息。
setsockopt=<level>:<optname>:<optval>
使用给定参数调用setsockopt()套接字。level [ int ] 用作第二个参数setsockopt()并指定层，例如 SOL_TCP 用于 TCP（Linux 上为 6），或 SOL_SOCKET 用于套接字层（Linux 上为 1）。optname[ int ] 是第三个参数，setsockopt()它告诉我们要设置哪个套接字选项。对于实际数字，您可能需要查找系统的相应包含文件。对于第 4 和第 5 个setsockopt()参数， value[ dalan ] 指定按指针传递给函数的任意字节序列，并带有自动派生的长度参数。
setsockopt-int=<level>:<optname>:<optval>
类似setsockopt，但 <optval> 是指向 int [ int ] 的指针
setsockopt-listen=<level>:<optname>:<optval>
类似setsockopt，但对于侦听类型地址，它应用于侦听套接字而不是连接的套接字。
setsockopt-string=<level>:<optname>:<optval>
喜欢setsockopt，但 <optval> 是一个字符串。该字符串以空字符结尾传递给函数，并且长度参数自动从数据中派生。

UNIX 选项组

这些选项适用于基于 UNIX 域的地址。

unix-tightsocklen=[0|1]
在套接字操作上，传递一个不包括整个 struct sockaddr_un 记录但（除了其他组件）文件名或抽象字符串的相关部分的套接字地址长度。默认值为 1。
IP4 和 IP6 选项组

这些选项可用于基于 IPv4 和 IPv6 的套接字。

tos=<tos>
将传出数据包的 TOS（服务类型）字段设置为 <tos> [ byte ]（参见 RFC 791）。
ttl=<ttl>
将传出数据包的 TTL（生存时间）字段设置为 <ttl> [ byte ]。
ip-options=<data>
设置 IP 选项，如源路由。必须以二进制形式给出，推荐格式是前导“x”后跟偶数个十六进制数字。此选项可以多次使用，数据被附加。例如，要使用松散源路由通过某个网关连接到主机 10.0.0.1，请使用网关作为地址参数并使用选项设置松散源路由 ip-options=x8307040a000001 。
IP 选项在 RFC 791 中定义。
mtudiscover=<0|1|2>
取 0、1、2 表示从不、不想要或始终在此套接字上使用路径 MTU 发现。
ip-pktinfo
设置 IP_PKTINFO 套接字选项。这允许接收和记录包含目标地址和接口 (Linux) 的辅助消息（示例）。
ip-recverr
设置 IP_RECVERR 套接字选项。这允许接收和记录包含详细错误信息的辅助消息。
ip-recvopts
设置 IP_RECVOPTS 套接字选项。这允许接收和记录 IP 选项辅助消息（Linux、*BSD）。
ip-recvtos
设置 IP_RECVTOS 套接字选项。这可以接收和记录 TOS（服务类型）辅助消息 (Linux)。
ip-recvttl
设置 IP_RECVTTL 套接字选项。这允许接收和记录 TTL（生存时间）辅助消息（Linux、*BSD）。
ip-recvdstaddr
设置 IP_RECVDSTADDR 套接字选项。这允许接收和记录包含目标地址 (*BSD) 的辅助消息（示例）。
ip-recvif
设置 IP_RECVIF 套接字选项。这允许接收和记录接口辅助消息（*BSD）（示例）。
ip-add-membership=<multicast-address:interface-address>
ip-add-membership=<multicast-address:interface-name>
ip-add-membership=<multicast-address:interface-index>
ip-add-membership=<multicast-address:interface-address:interface-name>
ip-add-membership=<multicast-address:interface-address:interface-index>
使指定多播组的套接字成员。这目前仅适用于 IPv4。该选项采用多播组的 IP 地址和有关所需网络接口的信息。最常见的语法是第一个语法，而其他语法仅在提供struct mreqn(Linux) 的系统上可用。
可以使用实用程序 procan显示活动网络接口的索引。
ip-add-source-membership=<multicast-address:interface-address:source-address>
使指定源的指定多播组的套接字成员，即只传送来自该地址的多播流量。这目前仅适用于 IPv4。
ip-multicast-if=<hostname>
指定要用于多播流量的网络接口的主机名或地址。
ip-multicast-loop=<bool>
指定传出多播流量是否应循环回接口。
ip-multicast-ttl=<byte>
设置用于传出多播流量的 TTL。默认值为 1。
ip-transparent
设置 IP_TRANSPARENT 套接字选项。此选项可能需要 root 权限。
res-debug
res-aaonly
res-usevc
res-primary
res-igntc
res-recurse
res-defnames
res-stayopen
res-dnsrch
这些选项设置相应的解析器（名称解析）选项标志。附加“=0”以清除默认选项。有关这些选项的更多信息，请参见 man resolver(5)。注意：这些选项仅对它们应用到的地址有效。

IP6 选项组

这些选项只能用于基于 IPv6 的套接字。有关可应用于 IPv4 和 IPv6 套接字的选项， 请参阅IP 选项。

ipv6only=<bool>
设置 IPV6_V6ONLY 套接字选项。如果为 0，则 TCP 堆栈还将接受在同一端口上使用 IPv4 协议的连接。默认值取决于系统。
ipv6-recvdstopts
设置 IPV6_RECVDSTOPTS 套接字选项。这允许接收和记录包含目标选项的辅助消息。
ipv6-recvhoplimit
设置 IPV6_RECVHOPLIMIT 套接字选项。这允许接收和记录包含 hoplimit 的辅助消息。
ipv6-recvhopopts
设置 IPV6_RECVHOPOPTS 套接字选项。这允许接收和记录包含跃点选项的辅助消息。
ipv6-recvpktinfo
设置 IPV6_RECVPKTINFO 套接字选项。这允许接收和记录包含目标地址和接口的辅助消息。
ipv6-unicast-hops=link(TYPE_INT)(<int>)
设置 IPV6_UNICAST_HOPS 套接字选项。这会设置传出单播数据包的跳数限制 (TTL)。
ipv6-recvrthdr
设置 IPV6_RECVRTHDR 套接字选项。这允许接收和记录包含路由信息的辅助消息。
ipv6-tclass
设置 IPV6_TCLASS 套接字选项。这设置了传出数据包的传输类别。
ipv6-recvtclass
设置 IPV6_RECVTCLASS 套接字选项。这允许接收和记录包含传输类的辅助消息。

TCP 选项组

这些选项可以应用于 TCP 套接字。它们通过调用 setsockopt() 适当的参数来工作。

cork
不发送小于 MSS（最大分段大小）的数据包。
defer-accept
在侦听时，仅当来自对等方的数据到达时才接受连接。
keepcnt=<count>
将关闭套接字之前的保活次数设置为 <count> [ int ]。
keepidle=<seconds>
在发送第一个 keepalive 到 <seconds> [ int ] 之前设置空闲时间。
keepintvl=<seconds>
将两个 keepalive 之间的间隔设置为 <seconds> [ int ]。
linger2=<seconds>
将套接字保持在 FIN-WAIT-2 状态的时间设置为 <seconds> [ int ]。
mss=<bytes>
socket() 在调用 <bytes> [ int ] 之后设置 MSS（最大段大小） 。然后将该值与 SYN 或 SYN/ACK 数据包一起建议给对等方（示例）。
mss-late=<bytes>
将连接建立后的套接字的 MSS 设置为 <bytes> [ int ]。
nodelay
关闭用于测量 RTT（往返时间）的 Nagle 算法。
rfc1323
启用 RFC1323 TCP 选项：TCP 窗口比例、往返时间测量 (RTTM) 和防止包装序列号 (PAWS) (AIX)。
stdurg
启用符合 RFC1122 的紧急指针处理 (AIX)。
syncnt=<count>
设置连接到 <count> [ int ] 期间的最大 SYN 重传次数。
md5sig
启用在数据包上生成 MD5 摘要 (FreeBSD)。
noopt
禁用 TCP 选项（FreeBSD、MacOSX）。
nopush
设置 TCP_NOPUSH 套接字选项（FreeBSD、MacOSX）。
sack-disable
禁用选择性确认功能 (OpenBSD)。
signature-enable
启用在数据包上生成 MD5 摘要 (OpenBSD)。
abort-threshold=<milliseconds>
设置在已建立连接 (HP-UX) 上等待对等方应答的时间。
conn-abort-threshold=<milliseconds>
设置在初始连接期间等待服务器应答的时间 (HP-UX)。
keepinit
设置在 connect() 期间在放弃之前等待服务器应答的时间。值以半秒为单位，默认为 150 (75s) (Tru64)。
paws
启用“防止包装序列号”功能 (Tru64)。
sackena
启用选择性确认 (Tru64)。
tsoptena
启用允许在现有连接 (Tru64) 上重新计算 RTT 的时间戳选项。

UDP 选项组

此选项可应用于 UDP 数据报套接字。

udp-ignore-peerport>
地址 UDP-DATAGRAM 期望传入响应来自其第二个参数中指定的端口。使用此选项，它接受来自任何端口的数据包。

SCTP 选项组

这些选项可以应用于 SCTP 流套接字。

sctp-nodelay
设置禁用 Nagle 算法的 SCTP_NODELAY 套接字选项。
sctp-maxseg=<bytes>
将 SCTP_MAXSEG 套接字选项设置为 <bytes> [ int ]。然后将该值与 SYN 或 SYN/ACK 数据包一起建议给对等方。

UDP、TCP 和 SCTP 选项组

在这里，我们找到与网络端口机制相关的选项，因此可以与 UDP、TCP 和 SCTP 客户端和服务器地址一起使用。

sourceport=<port>
对于传出（客户端）TCP 和 UDP 连接，它 使用额外 调用设置源<port> 。bind()使用 TCP 或 UDP 侦听地址，如果客户端不使用此源端口，socat 会立即关闭连接。UDP-RECV、UDP-RECVFROM、UDP-SENDTO 和 UDP-DATAGRAM 地址在数据包不匹配时忽略该数据包。（示例）。
lowport
带有此选项的传出（客户端）TCP 和 UDP 连接使用 640 和 1023 之间的未使用的随机源端口，包括。在 UNIX 类操作系统上，这需要 root 权限，因此表明客户端进程是由本地 root 授权的。如果客户端不使用源端口 <= 1023，则带有此选项的 TCP 和 UDP 侦听地址会立即关闭连接。此机制在某些情况下可以提供有限的授权。

SOCKS 选项组

使用 SOCKS 类型地址时，可以设置一些特定于袜子的选项。

socksport=<tcp service>
使用<TCP service> 覆盖默认的“socks”服务或 socks 服务器端口的 1080 端口。
socksuser=<user>
将用户名字段中的 <user> [ string ] 发送到 socks 服务器。默认为实际用户名（$LOGNAME 或 $USER）（示例）。

HTTP 选项组

可以与 HTTP 类型地址一起提供的选项。当前实现的唯一 HTTP 地址是proxy-connect。

proxyport=<TCP service>
用<TCP service> 覆盖默认的 HTTP 代理端口 8080 。
ignorecr
HTTP 协议要求使用 CR+NL 作为行终止符。当代理服务器违反此标准时，socat 可能无法理解其答案。此选项指示 socat 将 NL 解释为行终止符并忽略答案中的 CR。尽管如此，socat 还是将 CR+NL 发送到代理。
proxy-authorization=<username>:<password>
向代理服务器提供“基本”身份验证。该选项的参数与 base64 编码形式的“Proxy-Authorization: Basic”标头一起使用。
注意：进程列表中本地机器上的每个用户都可以看到用户名和密码；用户名和密码未加密（base64 编码）传输到代理服务器，并且可能被嗅探。
proxy-authorization-file=<filename>
与选项proxy-authorization类似，但凭据是从文件中读取的，因此在进程列表中不可见。
resolve
默认情况下，socat 向代理发送一个包含目标主机名的 CONNECT 请求。使用此选项，socat 在本地解析主机名并发送 IP 地址。请注意，根据 RFC 2396，仅实现对 IPv4 地址的名称解析。

范围选项组

这些选项检查是否应授予连接客户端访问权限。它们可以应用于侦听和接收网络套接字。tcp-wrappers 选项属于这一组。

range=<address-range>
接受连接后，测试对等方是否在range 内。对于 IPv4 地址，地址范围采用地址/位的形式，例如 10.0.0.0/8，或地址：掩码，例如 10.0.0.0:255.0.0.0（示例）；对于 IPv6，它是 [ip6-address]/bits，例如 [::1]/128。如果客户端地址不匹配，socat拒绝连接尝试，发出警告，并继续侦听/接收。
tcpwrap[=<name>]
使用 Wietse Venema 的 libwrap (tcpd) 库来确定是否允许客户端连接。默认情况下，配置文件是 /etc/hosts.allow 和 /etc/hosts.deny，有关详细信息，请参阅“man 5 hosts_access”。可选的 <name> （类型string ）作为守护进程名称（ example ）传递给包装函数。如果省略，则传递 socats 调用 (argv[0]) 的基本名称。如果 tcpwrap 和 range 选项都应用于地址，则必须同时满足这两个条件才能允许连接。
allow-table=<filename>
采用指定的文件而不是 /etc/hosts.allow。
deny-table=<filename>
采用指定的文件而不是 /etc/hosts.deny。
tcpwrap-etc=<directoryname>
在指定目录中查找 hosts.allow 和 hosts.deny。被选项hosts-allow 和hosts-deny覆盖。

LISTEN 选项组

特定于侦听套接字的选项。

backlog=<count>
listen() 将通过系统调用 传递的积压值设置 为 <count> [ int ]。默认值为 5。
accept-timeout=<seconds>
在 <seconds> [ timeval ] 后以错误状态 结束等待连接。
max-children=<count>
限制并发子进程的数量 [ int ]。默认为无限制。

儿童选项组

通过子进程具有多个连接的地址的选项。

fork
建立连接后，在子进程中处理其通道并保持父进程尝试通过侦听或循环连接来产生更多连接（示例）。
OPENSSL-CONNECT 和 OPENSSL-LISTEN 在实际分叉子节点时有所不同： OPENSSL-LISTEN在SSL 握手之前分叉，而 OPENSSL-CONNECT 在之后分叉。 重试和永远选项不会被子进程继承。
在某些操作系统（例如 FreeBSD）上，此选项不适用于 UDP-LISTEN 地址。

执行选项组

调用程序的地址选项。

path=<string>
覆盖 PATH 环境变量以使用<string> 搜索程序 。该 $PATH 值在子进程中也有效。
login
调用前缀 argv[0] 为 execvp() “-”，从而使 shell 表现为登录 shell。

FORK 选项组

EXEC 或 SYSTEM 地址使用子进程调用程序并在socat和程序之间传输数据。进程间通信机制可以受到以下选项的影响。默认情况下，a socketpair() 被创建并分配给子进程的 stdin 和 stdout，而 stderr 是从socat进程继承的，子进程使用文件描述符 0 和 1 与主 socat 进程通信。

nofork
不派生子进程来执行程序，而是直接从实际的 socat 实例调用 execvp() 或 system()。这避免了程序与其对等方之间另一个进程的开销，但引入了很多限制：
此选项只能应用于第二个socat地址。
它不能应用于双重地址的一部分。
第一个 socat 地址不能是 OPENSSL 或 READLINE
socat 选项 -b、-t、-D、-l、-v、-x 变得无用
对于这两个地址，选项 ignoreeof、cr 和 crnl 变得无用
对于第二个地址（带有选项 nofork 的地址），无法应用选项 append、cloexec、flock、user、group、mode、nonblock、perm-late、setlk 和 setpgid。其中一些可以用于第一个地址。
pipes
为进程间通信创建一对未命名的管道，而不是套接字对。
openpty
使用创建的伪终端 openpty() 而不是默认（socketpair 或 ptmx）与子进程建立通信。
ptmx
使用通过打开/dev/ptmx或/dev/ptc而不是默认（socketpair） 创建的伪终端与子进程建立通信。
pty
使用伪终端而不是套接字对与子进程建立通信。使用可用机制创建 pty。如果 openpty 和 ptmx 都可用，它使用 ptmx 因为这是 POSIX 兼容的（示例）。
ctty
使 pty 成为子进程的控制 tty（示例）。
stderr
通过将 stderr 设置 dup() 为 stdout ( example )，将子进程的 stderr 定向到其输出通道。
fdin=<fdnum>
将子进程输入通道分配给其文件描述符 <fdnum> 而不是 stdin (0)。从子进程启动的程序必须使用这个 fd 从socat读取数据（示例）。
fdout=<fdnum>
将子进程输出通道分配给其文件描述符 <fdnum> 而不是 stdout (1)。从子进程启动的程序必须使用此 fd 将数据写入socat（示例）。
sighup, sigint,sigquit
让 socat 将这种类型的信号传递给子进程。如果没有地址有这个选项，socat 会在这些信号上终止。

TERMIOS 选项组

对于在 tty 上工作的地址（例如，stdio、file:/dev/tty、exec:...、pty），UN*X termios 机制中定义的终端参数可用作地址选项参数。请注意，您的交互式终端的参数更改在socat终止后仍然有效，因此您之后可能需要在 shell 中输入“reset”或“stty sane”。对于带有选项 PTY 的 EXEC 和 SYSTEM 地址，这些选项适用于子进程的 pty。

b0
断开终端。
b19200
将串行线速度设置为 19200 波特。其他一些费率是可能的；使用类似的东西socat -hh |grep ' b[1-9]'来查找您的实施支持的所有速度。
注意：在某些操作系统上，这些选项可能不可用。请改用ispeed或ospeed 。
echo=<bool>
启用或禁用本地回显。
icanon=<bool>
设置或清除规范模式，启用行缓冲和一些特殊字符。
raw
设置原始模式，从而传递几乎未处理的输入和输出。此选项已过时，请改用选项rawer或cfmakeraw。
rawer
使终端比原始选项更原始。此选项隐式关闭回声。（示例）。
cfmakeraw
cfmakeraw()通过调用或模拟此调用来 设置原始模式。此选项隐式关闭回声。
ignbrk=<bool>
忽略或解释 BREAK 字符（例如，^C）
brkint=<bool>
bs0
bs1
bsdly=<0|1>
clocal=<bool>

cr0
cr1
cr2
cr3
将回车延迟分别设置为 0、1、2 或 3。0 表示无延迟，其他值取决于终端。

crdly=<0|1|2|3>
cread=<bool>
crtscts=<bool>

cs5
cs6
cs7
cs8
将字符大小分别设置为 5、6、7 或 8 位。

csize=<0|1|2|3>
cstopb=<bool>
设置两个停止位，而不是一个。
dsusp=<byte>
设置挂起当前前台进程并重新激活 shell 的 VDSUSP 字符的值（除 Linux 之外的所有）。
echoctl=<bool>
在帽子符号中回显控制字符（例如 ^A）
echoe=<bool>
echok=<bool>
echoke=<bool>
echonl=<bool>
echoprt=<bool>
eof=<byte>
eol=<byte>
eol2=<byte>
erase=<byte>
discard=<byte>
ff0
ff1
ffdly=<bool>
flusho=<bool>
hupcl=<bool>
icrnl=<bool>
iexten=<bool>
igncr=<bool>
ignpar=<bool>
imaxbel=<bool>
inlcr=<bool>
inpck=<bool>
intr=<byte>
isig=<bool>
ispeed=<unsigned-int>
设置此线路上传入数据的波特率。
另见：ospeed , b19200
istrip=<bool>
iuclc=<bool>
ixany=<bool>
ixoff=<bool>
ixon=<bool>
kill=<byte>
lnext=<byte>
min=<byte>
nl0
将换行延迟设置为 0。
nl1
nldly=<bool>
noflsh=<bool>
ocrnl=<bool>
ofdel=<bool>
ofill=<bool>
olcuc=<bool>
onlcr=<bool>
onlret=<bool>
onocr=<bool>
opost=<bool>
启用或禁用输出处理；例如，将 NL 转换为 CR-NL。
ospeed=<unsigned-int>
设置此线路上传出数据的波特率。
参见：ispeed，b19200
parenb=<bool>
启用输出奇偶校验生成和输入奇偶校验。
parmrk=<bool>
parodd=<bool>
pendin=<bool>
quit=<byte>
reprint=<byte>
sane
使终端进入有用的默认状态。
start=<byte>
stop=<byte>
susp=<byte>
swtc=<byte>
tab0
tab1
tab2
tab3
tabdly=<unsigned-int>
time=<byte>
tostop=<bool>
vt0
vt1
vtdly=<bool>
werase=<byte>
xcase=<bool>
xtabs
i-pop-all
使用 UNIX System V STREAMS，从堆栈中删除所有驱动程序。
i-push=<string>
使用 UNIX System V STREAMS，将具有给定名称（字符串）的驱动程序（模块）推入堆栈。例如，要确保 Solaris 上的字符设备支持 termios 等，请使用以下选项： i-pop-all,i-push=ptem,i-push=ldterm,i-push=ttcompat

PTY 选项组

这些选项旨在与pty地址类型一起使用。

link=<filename>
生成指向实际伪终端 (pty) 的符号链接。这可能有助于解决 pty 生成的名称或多或少不可预测的问题，从而难以直接自动访问 socat 生成的 pty。使用此选项，用户可以在文件层次结构中指定一个“修复”点，以帮助他访问实际的 pty（示例）。从socat版本 1.4.3 开始，当地址关闭时符号链接被删除（但请参见选项unlink-close）。
wait-slave
阻止打开阶段，直到进程打开 pty 的从属端。通常，socat 在生成 pty 并打开下一个地址或进入传输循环后继续。使用 wait-slave 选项，socat 会等到某个进程打开 pty 的从属端，然后再继续。此选项仅在操作系统提供poll() 系统调用时才有效。而且它取决于 pty 的未记录行为，因此它不适用于所有操作系统。它已在 Linux、FreeBSD、NetBSD 和带有 openpty 的 Tru64 上成功测试。
pty-interval=<seconds>
当wait-slave选项设置时，socat 会定期检查 HUP 条件，poll()用于查找 pty 的从端是否已打开。默认轮询间隔为 1 秒。使用 pty-interval 选项 [ timeval ] 更改此值。

OPENSSL 选项组

这些选项适用于openssl和 openssl-listen地址类型。

cipher=<cipherlist>
指定可用于连接的密码列表。有关<cipherlist> 的语法、值和默认值的详细信息，ciphers 请参见CIPHER LIST FORMAT的手册页 。
可以给出几个密码字符串，用“：”分隔。一些简单的密码字符串：
3DES
使用具有三重 DES 的密码套件。
MD5
使用带有 MD5 的密码套件。
空值
使用未经身份验证的密码套件。
空值
不使用加密。
高的
使用具有“高”加密的密码套件。
注意peer必须支持selected属性，否则协商会失败。
method=<ssl-method>
此选项基于已弃用的功能，并且仅在 使用选项构建socat--with-openssl-method时可用。使用选项min-proto-version 和max-proto-version 代替。设置要使用的协议版本。有效字符串（不区分大小写）为：
SSL2
选择 SSL 协议版本 2。
SSL3
选择 SSL 协议版本 3。
SSL23
选择最佳的可用 SSL 或 TLS 协议。
TLS1
选择 TLS 协议版本 1。
TLS1.1
选择 TLS 协议版本 1.1。
TLS1.2
选择 TLS 协议版本 1.2。如果未提供此选项，OpenSSL 会与其对等方协商该方法。
min-proto-version
此选项告诉 OpenSSL 使用此或更高版本的 SSL/TLS 协议版本，并拒绝接受较低/较旧的协议。有效的语法是：
SSL2
选择 SSL 协议版本 2。
SSL3
选择 SSL 协议版本 3。
TLS1
TLS1.0
选择 TLS 协议版本 1。
TLS1.1
选择 TLS 协议版本 1.1。
TLS1.2
选择 TLS 协议版本 1.2。
TLS1.3
选择 TLS 协议版本 1.3。
openssl-max-proto-version
此选项类似于min-proto-version，但是它不允许使用更高的协议版本。用于测试对等体。
verify=<bool>
控制对等方证书的检查。默认值为 1（真）。禁用验证可能会为所有人打开您的套接字，使加密无用！
cert=<filename>
指定带有证书和私钥的文件以进行身份​​验证。证书必须采用 OpenSSL 格式 (*.pem)。对于 openssl-listen，强烈建议使用此选项。除了 cipher aNULL，没有给出证书时会出现“no shared ciphers”错误。
key=<filename>
指定带有私钥的文件。私钥可能在此文件中，也可能在带有cert选项的文件中。必须证明它是证书所有者的一方需要私钥。
dhparams=<filename>
使用 Diffie Hellman 参数指定文件。这些参数也可能在使用cert 选项给出的文件中，在这种情况下不需要 dhparams 选项。
cafile=<filename>
指定具有受信任（根）授权证书的文件。该文件必须是 PEM 格式并且应该包含一个或多个证书。检查其对等方身份验证的一方仅信任此文件中的证书。
capath=<dirname>
指定具有受信任（根）证书的目录。该目录必须包含 PEM 格式的证书及其哈希（请参阅 OpenSSL 文档）
egd=<filename>
在某些系统上，openssl 需要一个明确的随机数据源。指定像 egd 这样的熵收集守护进程提供随机数据的套接字名称，例如 /dev/egd-pool。
pseudo
在 openssl 无法找到熵源且无法使用熵收集守护程序的系统上，此选项激活提供伪熵的机制。这是通过以微秒为单位为 libc 伪随机数生成器提供初始值的当前时间来实现的。然后为 openssl 提供 random() 调用的输出。
注意：此机制不足以生成安全密钥！
compress
启用或禁用对连接的压缩。将此设置为“无”禁用压缩，将其设置为“自动”让 OpenSSL 选择双方支持的最佳可用算法。默认设置是不触摸任何与压缩相关的设置。注意：需要 OpenSSL 0.9.8 或更高版本，并且使用 OpenSSL 0.9.8 禁用压缩会影响该过程中的所有新连接。
commonname=<string>
指定对等证书必须匹配的公用名。使用 OPENSSL-CONNECT地址，这会覆盖给定的主机名或 IP 目标地址；使用 OPENSSL-LISTEN这将打开对等证书公用名的检查。此选项仅在选项验证未禁用且所选密码提供对等证书 时才有意义 。
no-sni=<bool>
不要使用选择所需服务器证书的客户端服务器名称指示 (SNI) 功能。注意：自socat版本 1.7.4.0
起自动使用 SNI ，并使用commonname或给定的主机名。
snihost=<string>
将客户端服务器名称指示 (SNI) 主机名设置为不同于寻址的服务器名称或公用名称。当服务器证书具有多个主机名或通配符名称时，这可能很有用，因为 SNI 主机名以明文形式传递给服务器并且可能被窃听；使用此选项，可以传输所需证书的模拟名称。
fips
如果已编译，则启用 FIPS 模式。有关 FIPS 加密实施标准的信息，请参阅http://oss-institute.org/fips-faq.html。此模式可能需要使用启用 FIPS 的 openssl 版本生成所涉及的证书。在一个 socat 地址上设置或清除此选项会影响此进程的所有 OpenSSL 地址。

重试选项组

控制某些系统调用重试的选项，尤其是连接尝试。

retry=<num>
连接或侦听尝试中止之前的重试次数。默认值为 0，表示仅尝试一次。
interval=<timespec>
连续尝试之间的时间（秒，[ timespec ]）。默认值为 1 秒。
forever
执行无限次重试。

TUN 选项组

控制 Linux TUN/TAP 接口设备地址的选项。

tun-device=<device-file>
指示 socat 为 TUN 克隆设备采用另一条路径。默认为 /dev/net/tun。
tun-name=<if-name>
为生成的网络接口指定一个特定名称，而不是系统生成的名称（tun0、tun1 等）
tun-type=[tun|tap]
设置TUN设备的类型；使用此选项生成 TAP 设备。有关这些类型之间的区别，请参阅 Linux 文档。当您尝试在两个 TUN 设备之间建立隧道时，它们的类型应该相同。
iff-no-pi
设置 IFF_NO_PI 标志，该标志控制设备是否在隧道中包含其他数据包信息。当您尝试在两个 TUN 设备之间建立隧道时，这些标志应该具有相同的值。
iff-up
将 TUN 网络接口状态设置为 UP。强力推荐。
iff-broadcast
设置 TUN 网络接口的 BROADCAST 标志。
iff-debug
设置 TUN 网络接口的 DEBUG 标志。
iff-loopback
设置 TUN 网络接口的 LOOPBACK 标志。
iff-pointopoint
设置 TUN 设备的 POINTOPOINT 标志。
iff-notrailers
设置 TUN 设备的 NOTRAILERS 标志。
iff-running
设置 TUN 设备的 RUNNING 标志。
iff-noarp
设置 TUN 设备的 NOARP 标志。
iff-promisc
设置 TUN 设备的 PROMISC 标志。
iff-allmulti
设置 TUN 设备的 ALLMULTI 标志。
iff-master
设置 TUN 设备的 MASTER 标志。
iff-slave
设置 TUN 设备的 SLAVE 标志。
iff-multicast
设置 TUN 设备的 MULTICAST 标志。
iff-portsel
设置 TUN 设备的 PORTSEL 标志。
iff-automedia
设置 TUN 设备的 AUTOMEDIA 标志。
iff-dynamic
设置 TUN 设备的动态标志。


数据值
本节说明地址参数和地址选项可以采用的不同数据类型。

地址范围
目前仅针对 IPv4 和 IPv6 实施。请参阅地址选项 “范围”
布尔
“0”或“1”；如果省略值，则取“1”。
字节
一个无符号整数，用 strtoul() 、 小于或等于 读取UCHAR_MAX 。
命令行
指定程序名称及其参数的字符串，由单个空格分隔。
数据
这是一个更通用的数据规范。给定的文本字符串包含有关目标数据类型和值的信息。通常，前导字符指定以下数据项的类型。在其特定上下文中，可能存在默认数据类型。
目前只实现了以下规范：
一世
一个有符号整数，按主机字节顺序存储。
示例： i-1000 （整数 -1000）
一世
一个无符号整数，按主机字节顺序存储。
l
一个有符号长整数，按主机字节顺序存储。
大号
一个无符号长整数，按主机字节顺序存储。
s
一个有符号的短整数，按主机字节顺序存储。
小号
一个无符号短整数，按主机字节顺序存储。
b
一个有符号字节（有符号字符）。
乙
一个无符号字节（无符号字符）。
X
以下是偶数个十六进制数字，存储为字节序列。
示例： x7f000001（IP 地址 127.0.0.1）
"
以下是与常见转换一起使用的字符串 \n \r \t \f \b \a \e \0; 字符串必须以 '"' 结尾。请注意引号和反斜杠需要从 shell 和socat 转换中转义。
示例： “Hello world！\n”
'
单个字符，具有通常的转换。请注意，引号和反斜杠需要从 shell 和socat转换中转义。
示例： 'a'
数据项可以用空格分隔，无需再次重复类型说明符。
目录
具有通常 UN*X 目录名称语义的字符串。
设施
小写字符的系统日志设施的名称。
fdnum
一个 unsigned int 类型，用 读取 strtoul() ，指定一个 UN*X 文件描述符。
文件名
具有通常 UN*X 文件名语义的字符串。
团体
如果第一个字符是十进制数字，则使用 strtoul() 指定组 id 的无符号整数读取该值。否则，它必须是现有的组名。
整数
一个遵循 strtol() 函数规则的以“0”为底的数字，即十进制数、以“0”开头的八进制数或以“0x”开头的十六进制数。该值必须适合 C int。
界面
一个字符串，指定网络接口的设备名称，如 ifconfig 或 procan 所示，例如“eth0”。
IP地址
数字和点表示法的 IPv4 地址，括号中的十六进制表示法的 IPv6 地址，或解析为 IPv4 或 IPv6 地址的主机名。
示例：127.0.0.1、[::1]、www.dest-unreach.org、dns1
IPv4 地址
数字和点表示法的 IPv4 地址或解析为 IPv4 地址的主机名。
示例：127.0.0.1、www.dest-unreach.org、dns2
IPv6 地址
用括号括起来的十六进制数字和冒号表示法的 IPv6 地址，或解析为 IPv6 地址的主机名。
示例：[::1]、[1234:5678:9abc:def0:1234:5678:9abc:def0]、ip6name.domain.org
长
用 读取的数字 strtol() 。该值必须适合 C long。
长长的
用 读取的数字 strtoll() 。该值必须适合 C long long。
off_t
一个实现依赖的有符号数，通常为 32 位，使用 strtol 或 strtoll 读取。
off64_t
一个实现依赖的有符号数，通常为 64 位，使用 strtol 或 strtoll 读取。
mode_t
一个无符号整数，用 读取 strtoul() ，指定模式（权限）位。
pid_t
一个数字，用 读取 strtol() ，指定进程 ID。
港口
指定 TCP 或 UDP 端口的 uint16_t（16 位无符号数），读取时使用 strtoul() .
协议
一个无符号的 8 位数字，用 . 读取 strtoul() 。
尺寸_t
具有 size_t 限制的无符号数，用 . 读取 strtoul 。
袜子名称
一个套接字地址。请参阅地址选项“绑定”
细绳
一系列字符，不包含“\0”，并且根据命令行中的位置，“:”、“,”或“!!”。请注意，您可能必须在命令行中转义 shell 元字符。
TCP服务
服务名称，不以数字开头，由 解析 getservbyname() ，或读取的无符号整数 16 位数字 strtoul() 。
时间间隔
指定秒数的双浮点数；该数字被映射到一个结构 timeval，由秒和微秒组成。
时间规范
指定秒数的双浮点数；该数字被映射到一个结构 timespec，由秒和纳秒组成。
UDP服务
服务名称，不以数字开头，由 解析 getservbyname() ，或读取的无符号整数 16 位数字 strtoul() 。
无符号整数
用 读取的数字 strtoul() 。该值必须适合 C 无符号整数。
用户
如果第一个字符是十进制数字，则使用 strtoul() 指定用户 ID 的无符号整数读取该值。否则，它必须是现有用户名。
VSOCK cid
指定 VSOCK 上下文标识符 (CID) 的 uint32_t（32 位无符号数），读取时使用 strtoul() . 有几个特殊地址： VMADDR_CID_ANY (-1U) 表示任意地址进行绑定；VMADDR_CID_HOST (2) 是主机的知名地址。
VSOCK 端口
指定 VSOCK 端口的 uint32_t（32 位无符号数），读取时使用 strtoul() .

例子

socat - TCP4:www.domain.org:80
在STDIO (-) 和 TCP4连接 之间传输数据到主机 www.domain.org 的端口 80。此示例生成类似于 telnet 或 netcat 的交互式连接。stdin 终端参数没有改变，所以你可以用 ^D 关闭继电器或用 ^C 中止它。


socat -d -d READLINE,history=$HOME/.http_history \
TCP4:www.domain.org:www,crnl
这与前面的示例类似，但您可以以类似 bash 的方式 ( READLINE ) 编辑当前行并使用 历史文件 .http_history；socat打印有关进度的消息（-d -d）。端口由服务名称 (www) 指定，并使用正确的网络行终止字符 ( crnl ) 而不是 NL。


socat TCP4-LISTEN:www TCP4:www.domain.org:www
安装一个简单的 TCP 端口转发器。使用 TCP4-LISTEN它会在本地端口“www”上侦听，直到有连接进来，接受它，然后连接到远程主机 ( TCP4 ) 并开始数据传输。它不会接受第二个连接。


socat -d -d -lmlocal2 \
TCP4-LISTEN:80,bind=myaddr1,su=nobody,fork,range=10.0.0.0/8,reuseaddr \
TCP4:www.domain.org:80,bind=myaddr2
TCP 端口转发器，每一端都绑定到另一个本地 IP 地址（绑定）。这个例子处理几乎任意数量的并行或连续连接，方法是在每个连接之后 分叉一个新进程 accept() 。它通过在分叉后向用户nobody提供一点安全性 ；它只允许来自私有 10 网络（范围）的连接；由于reuseaddr，它允许在主进程终止后立即重新启动，即使某些子套接字没有完全关闭。使用-lmlocal2，socat 会记录到 stderr，直到成功到达接受循环。进一步的日志记录被定向到带有设施 local2 的 syslog。


socat TCP4-LISTEN:5555,fork,tcpwrap=script \
EXEC:/bin/myscript,chroot=/home/sandbox,su-d=sandbox,pty,stderr
一个接受连接（ TCP4-LISTEN） 的简单服务器，并为每个连接fork一个新的子进程；每个孩子都扮演着一个接力的角色。客户端必须匹配 /etc/hosts.allow 和 /etc/hosts.deny 中守护进程名称“script”的规则，否则将被拒绝访问（参见“man 5 hosts_access”）。对于EXEC运行程序，子进程 chroot到/home/sandbox，su到用户沙箱，然后启动程序/home/sandbox/bin/myscript。Socat和 myscript 通过伪 tty ( pty ) 进行通信；myscript 的 标准错误被重定向到标准输出，因此它的错误消息通过socat传输到连接的客户端。


socat EXEC:"mail.sh target@domain.com",fdin=3,fdout=4 \
TCP4:mail.relay.org:25,crnl,bind=alias1.server.org,mss=512
mail.sh是一个 shell 脚本，与socat一起分发，它实现了一个简单的 SMTP 客户端。它被编程为在其 FD 3（输入）和 4（输出）上“说”SMTP。fdin和fdout选项告诉socat使用这些FD 与程序通信。因为 mail.sh 继承了 stdin 和 stdout 而socat不使用它们，所以脚本可以从 stdin 读取邮件正文。Socat使 alias1 成为您的本地源地址（bind），注意正确的网络线路终止（crnl），并且每个数据包最多发送 512 个数据字节（mss）。


socat -,escape=0x0f /dev/ttyS0,rawer,crnl
通过串行线打开交互式连接，例如与调制解调器通话。rawer将控制台和 ttyS0 的终端参数设置为可行的值，crnl 转换为正确的换行符。escape允许使用字符 control-O 终止 socat 进程。考虑使用READLINE代替第一个地址。


socat UNIX-LISTEN:/tmp/.X11-unix/X1,fork \
SOCKS4:host.victim.org:127.0.0.1:6000,socksuser=nobody,sourceport=20
使用UNIX-LISTEN，socat打开一个监听 UNIX 域套接字/tmp/.X11-unix/X1。此路径对应于您机器上的本地 XWindow display :1，因此接受到 DISPLAY=:1 的 XWindow 客户端连接。Socat然后与SOCKS4服务器 host.victim.org 对话，由于其静态 IP 过滤器中与 FTP 相关的弱点，该服务器可能允许 基于sourceport 20 的连接。socat 假装被socksuser调用没有人，并请求连接到环回端口 6000（只有弱 sockd 配置才允许这样做）。因此，我们获得了与受害者 XWindow 服务器的连接，如果它不需要 MIT cookie 或 Kerberos 身份验证，我们就可以开始工作了。请注意，一次只能有一个连接，因为 TCP 只能使用一组给定的地址和端口建立一个会话。


socat -u /tmp/readdata,seek-end=0,ignoreeof -
这是单向数据传输 ( -u ) 的示例。Socat从文件 /tmp/readdata （隐式地址GOPEN）传输数据，从当前结尾开始（seek-end =0 让socat从文件的当前结尾开始读取；使用seek =0 或不使用 seek 选项首先读取现有数据）在类似“tail -f”的模式（ignoreeof）中。“文件”也可能是一个侦听的 UNIX 域套接字（此时不要使用搜索选项）。


(sleep 5; echo PASSWORD; sleep 5; echo ls; sleep 1) |
socat - EXEC:'ssh -l user server',pty,setsid,ctty
EXEC '将 ssh 会话连接到服务器。使用pty在socat和 ssh之间进行通信，使其成为 ssh 的控制 tty ( ctty )，并使此 pty 成为新进程组 ( setsid ) 的所有者，因此 ssh 接受来自socat的密码。


socat -u TCP4-LISTEN:3334,reuseaddr,fork \
OPEN:/tmp/in.log,creat,append
实现了一个简单的基于网络的消息收集器。对于连接到端口 3334 的每个客户端，都会生成一个新的子进程（选项fork）。客户端发送的所有数据都附加到文件 /tmp/in.log 中。如果文件不存在，socat creat就可以了。选项reuseaddr允许立即重新启动服务器进程。


socat READLINE,noecho='[Pp]assword:' EXEC:'ftp ftp.server.com',pty,setsid,ctty
围绕EXEC 'uted ftp 客户端实用程序 包装命令行历史记录 ( READLINE )。这允许编辑和重用 FTP 命令，以便相对舒适地浏览 ftp 目录层次结构。密码回显！ pty需要让 ftp 发出提示。然而，密码和 FTP 提示可能会出现一些混淆。


socat PTY,link=$HOME/dev/vmodem0,rawer,wait-slave \
EXEC:'"ssh modemserver.us.org socat - /dev/ttyS0,nonblock,rawer"'
在客户端上生成一个伪终端设备 ( PTY )，可以通过符号链接 $HOME/dev/vmodem0 访问该设备。需要串行线路或调制解调器的应用程序可以配置为使用$HOME/dev/vmodem0；它的流量将通过 ssh 定向到调制解调器服务器，其中另一个 socat 实例将其链接到 /dev/ttyS0。

socat TCP4-LISTEN:2022,reuseaddr,fork \
PROXY:proxy:www.domain.org:22,proxyport=3128,proxyauth=user:pass
启动一个在端口 2022 上接受连接的转发器，并使用 CONNECT 方法引导它们通过在主机代理上的端口 3128 ( proxyport ) 上侦听的代理守护进程，在该方法中，它们通过“pass” ( proxyauth ) 被验证为“用户”。然后，代理应在端口 22 上建立与主机 www.domain.org 的连接。


socat - SSL:server:4443,cafile=server.crt,cert=client.pem
是一个 OpenSSL 客户端，它尝试与 SSL 服务器建立安全连接。选项cafile指定包含信任证书的文件：只有当服务器提供这些证书之一并证明它拥有相关私钥时，我们才信任服务器。否则终止连接。使用cert指定包含客户端证书和相关私钥的文件。如果服务器希望进行客户端身份验证，则需要这样做；许多互联网服务器没有。
第一个地址 ('-') 几乎可以用任何其他 socat 地址替换。


socat OPENSSL-LISTEN:4443,reuseaddr,pf=ip4,fork,cert=server.pem,cafile=client.crt PIPE
是一个接受 TCP 连接的 OpenSSL 服务器，提供来自文件 server.pem 的证书并强制客户端提供根据 cafile.crt 验证的证书。
第二个地址（'PIPE'）几乎可以被任何其他 socat 地址替换。
有关生成和分发 OpenSSL 密钥和证书的说明，请参阅附加的 socat 文档socat-openssl.txt。

echo |socat -u - file:/tmp/bigfile,create,largefile,seek=100000000000
创建一个 100GB 的稀疏文件；这需要支持此的文件系统类型（ext2、ext3、reiserfs、jfs；不是 minix、vfat）。写入 1 个字节的操作可能需要很长时间（reiserfs：几分钟；ext2：“无”时间），并且生成的文件可能会仅使用其 inode 占用一些磁盘空间（reiserfs：2MB；ext2：16KB）。

socat tcp-l:7777,reuseaddr,fork system:'filan -i 0 -s >&2',nofork
侦听端口 7777 上的传入 TCP 连接。对于每个接受的连接，调用一个 shell。这个 shell 的标准输入和标准输出直接连接到 TCP 套接字（nofork）。shell 启动 filan 并让它将套接字地址打印到 stderr（您的终端窗口）。

echo -e "\0\14\0\0\c" |socat -u - file:/usr/bin/squid.exe,seek=0x00074420
用作原始二进制编辑器：它将 4 字节 000 014 000 000 写入可执行文件 /usr/bin/squid，偏移量为 0x00074420（这是一个真实世界的补丁，用于使 Cygwin 中的 squid 可执行文件在 Windows 下运行，实际为 2004 年 5 月） .

socat - tcp:www.blackhat.org:31337,readbytes=1000
连接到未知服务并防止被淹没。


socat -U TCP:target:9999,end-close TCP-L:8888,reuseaddr,fork
将来自端口 8888 上不同 TCP 流的数据合并到一个流到 target:9999。end-close选项可防止由第二个地址分叉的子进程终止与 9999 的共享连接（close(2) 只是取消链接只要父进程存在就保持活动状态的 inode；shutdown(2) 将主动终止联系）。


socat - UDP4-DATAGRAM:192.168.1.0:123,sp=123,broadcast,range=192.168.1.0/24
向网络 192.168.1.0/24 发送广播并接收那里时间服务器的回复。忽略来自该网络外部主机的 NTP 数据包。


socat - SOCKET-DATAGRAM:2:2:17:x007bxc0a80100x0000000000000000,bind=x007bx00000000x0000000000000000,setsockopt-int=1:6:1,range=x0000xc0a80100x0000000000000000:x0000xffffff00x0000000000000000
在语义上等同于前面的示例，但所有参数都以通用形式指定。setsockopt-int 的值 6 是 SO_BROADCAST.


socat - IP4-DATAGRAM:255.255.255.255:44,broadcast,range=10.0.0.0/8
使用协议 44 向本地网络发送广播。仅接受来自私有地址范围的回复。


socat - UDP4-DATAGRAM:224.255.0.1:6666,bind=:6666,ip-add-membership=224.255.0.1:eth0
使用 UDP 将数据从标准输入传输到指定的多播地址。本地和远程端口都是 6666。告诉接口 eth0 也接受给定组的多播数据包。本地网络上的多个主机可以运行此命令，因此任何主机发送的所有数据都将被所有其他主机接收。请注意，失败的可能原因有很多，包括 IP 过滤器、路由问题、操作系统选择错误的接口、网桥或交换机配置错误。


socat UDP:host2:4443 TUN:192.168.255.1/24,up
使用 UDP-L 和 tun 地址 192.168.255.2 与可能运行类似进程的 host2 建立虚拟（但不是私有！）网络的一侧。它们可以使用地址 192.168.255.1 和 192.168.255.2 相互联系。请注意，例如通过 TCP 或 SSL 的流式传输不能保证保留数据包边界，因此可能会导致数据包丢失。


socat - VSOCK-CONNECT:2:1234
在 1234 端口上与主机建立 VSOCK 连接（通过众所周知的 CID=2 始终可以访问主机）。

socat - VSOCK-LISTEN:1234
侦听 1234 端口上的 VSOCK 连接。

socat - VSOCK-CONNECT:31:4321,bind:5555
与在 1234 端口上具有 CID=31 的来宾建立 VSOCK 连接，将本地套接字绑定到 5555 端口。

socat VSOCK-LISTEN:3333,reuseaddr,fork VSOCK-CONNECT:42,3333
在端口 3333 上启动一个接受 VSOCK 连接的转发器，并将它们定向到同一端口上 CID=42 的来宾。

socat VSOCK-LISTEN:22,reuseaddr,fork TCP:localhost:22
将 VSOCK 连接从 22 端口转发到本地 SSH 服务器。在 VM 中运行它允许您使用 VSOCK 从主机通过 SSH 连接，如下例所示。

socat TCP4-LISTEN:22222,reuseaddr,fork VSOCK-CONNECT:33:22
将 TCP 连接从 22222 端口转发到 CID=33 监听 VSOCK 端口 22 的来宾。在主机中运行它，允许您通过运行“ssh -p 22222 user@localhost”的 SSH 连接，如果来宾运行上面的示例。


socat PTY,link=/var/run/ppp,rawer INTERFACE:hdlc0
规避了 pppd 需要串行设备，因此可能无法在以网络设备为代表的同步线路上工作的问题。socat 创建一个 PTY 让 pppd 开心，绑定到网络 接口 hdlc0，并且可以在两个设备之间传输数据。然后在设备上使用 pppd /var/run/ppp。


socat -T 1 -d -d TCP-L:10081,reuseaddr,fork,crlf SYSTEM:"echo -e \"\\\"HTTP/1.0 200 OK\\\nDocumentType: text/plain\\\n\\\ndate: \$\(date\)\\\nserver:\$SOCAT_SOCKADDR:\$SOCAT_SOCKPORT\\\nclient: \$SOCAT_PEERADDR:\$SOCAT_PEERPORT\\\n\\\"\"; cat; echo -e \"\\\"\\\n\\\"\""
创建一个简单的 HTTP 回显服务器：每个连接的 HTTP 客户端都会收到一个有效的 HTTP 回复，其中包含有关服务器主机看到的客户端地址和端口、主机地址（在多宿主服务器上可能有所不同）和原始地址的信息客户请求。


socat -d -d UDP4-RECVFROM:9999,so-broadcast,so-timestamp,ip-pktinfo,ip-recverr,ip-recvopts,ip-recvtos,ip-recvttl!!- SYSTEM:'export; sleep 1' |grep SOCAT
在端口 9999 上等待传入的 UDP 数据包并打印 socat 提供的环境变量。在基于 BSD 的系统上，您必须替换 ip-pktinfo为ip-recvdstaddr, ip-recvif。特别有趣的是 SOCAT_IP_DSTADDR：它包含数据包的目标地址，可以是单播、多播或广播地址。


echo -e "M-SEARCH * HTTP/1.1\nHOST: 239.255.255.250:1900\nMAN: \"ssdp:discover\"\nMX: 4\nST: \"ssdp:all\"\n" |./socat - UDP-DATAGRAM:239.255.255.250:1900,crlf
向本地网络发送 SSDP（简单服务发现协议）查询并收集并输出收到的答案。


诊断
Socat使用允许按严重性过滤消息的日志记录机制。提供的严重性或多或少与适当的 syslog 优先级兼容。使用一次或最多四次 -d 命令行选项，可以选择发出的消息的最低优先级。每条消息都包含一个指定消息严重性的大写字符（F、E、W、N、I 或 D 之一）

致命的：
需要无条件和立即终止程序的条件。
错误：
妨碍正确程序处理的条件。通常程序会终止（参见选项 -s）。
警告：
某些功能无法正常运行或处于无法保证正确的进一步处理的状态，但可能是可能的。
注意：
程序的有趣动作，例如在某种服务器模式下 监督socat 。
信息：
描述程序的作用，以及它发生的原因。允许监控文件描述符的生命周期。
调试：
程序如何工作、所有系统或库调用及其结果的描述。
日志消息可以写入标准错误、文件或系统日志。

在退出时，如果socat由于 EOF 或不活动超时而终止，则返回状态 0，错误为正值，致命错误为负值。


文件
/usr/bin/socat
/usr/bin/filan
/usr/bin/procan


环境变量
输入变量将信息从环境传送到 socat，输出变量由 socat 设置以用于执行的脚本和程序。

在以“SOCAT”开头的输出变量中，此前缀实际上被可执行文件的大写名称或选项 -lp的值替换。

SOCAT_DEFAULT_LISTEN_IP（输入）
（值 4 或 6）如果没有给出pf（协议系列）选项，则设置用于 listen、recv 和 recvfrom 地址的 IP 版本 。被 socat 选项-4或-6覆盖。
SOCAT_PREFERRED_RESOLVE_IP（输入）
（值 0、4 或 6）在地址类型、选项pf（协议系列）或地址格式未指定版本时，设置解析目标主机名时使用的 IP 版本。如果名称解析未返回匹配条目，则采用第一个结果（具有不同的 IP 版本）。值为 0 时，socat 始终选择第一条记录及其 IP 版本。
SOCAT_FORK_WAIT（输入）
指定成功 fork() 后父进程和子进程休眠的时间（秒）。对调试很有用。
SOCAT_VERSION（输出）
Socat 将此变量设置为其版本字符串，例如"1.7.0.0"对于发布版本或例如"1.6.0.1+envvar"对于临时版本；可以在 socat 调用的脚本中使用。
SOCAT_PID（输出）
Socat 将此变量设置为其进程 ID。在fork地址选项的情况下， SOCAT_PID 获取子进程 ID。用于exec和system的分叉不会更改 SOCAT_PID。
SOCAT_PPID（输出）
Socat 将此变量设置为其进程 ID。在fork的情况下， SOCAT_PPID 保留主进程的 pid。
SOCAT_PEERADDR（输出）
对于被动套接字地址（所有 LISTEN 和 RECVFROM 地址），此变量设置为描述对等套接字地址的字符串。不包括端口信息。
SOCAT_PEERPORT（输出）
使用适当的被动套接字地址（TCP、UDP 和 SCTP - LISTEN 和 RECVFROM），此变量设置为包含对等端口号的字符串。
SOCAT_SOCKADDR（输出）
对于所有 LISTEN 地址，此变量设置为描述本地套接字地址的字符串。不包括端口信息示例
SOCAT_SOCKPORT（输出）
对于TCP-LISTEN、 UDP-LISTEN和 SCTP-LISTEN地址，此变量设置为本地端口。
SOCAT_TIMESTAMP（输出）
对于应用了地址选项so-timestamp的所有 RECVFROM 地址，socat 将此变量设置为结果时间戳。
SOCAT_IP_OPTIONS（输出）
对于应用了地址选项ip-recvopts的所有基于 IPv4 的 RECVFROM 地址，socat 用接收到的数据包的 IP 选项填充此变量。
SOCAT_IP_DSTADDR（输出）
对于应用了地址选项ip-recvdstaddr (BSD) 或 ip-pktinfo （其他平台）的所有基于 IPv4 的 RECVFROM 地址，socat 将此变量设置为接收数据包的目标地址。这对于识别广播和多播寻址的数据包特别有用。
SOCAT_IP_IF（输出）
对于应用了地址选项ip-recvif (BSD) 或 ip-pktinfo （其他平台）的所有基于 IPv4 的 RECVFROM 地址，socat 将此变量设置为接收数据包的接口的名称。
SOCAT_IP_LOCADDR（输出）
对于应用了地址选项ip-pktinfo的所有基于 IPv4 的 RECVFROM 地址，socat 将此变量设置为接收数据包的接口的地址。
SOCAT_IP_TOS（输出）
对于应用了地址选项ip-recvtos的所有基于 IPv4 的 RECVFROM 地址，socat 将此变量设置为接收数据包的 TOS（服务类型）。
SOCAT_IP_TTL（输出）
对于应用了地址选项ip-recvttl的所有基于 IPv4 的 RECVFROM 地址，socat 将此变量设置为接收到的数据包的 TTL（生存时间）。
SOCAT_IPV6_HOPLIMIT（输出）
对于应用了地址选项ipv6-recvhoplimit的所有基于 IPv6 的 RECVFROM 地址，socat 将此变量设置为接收数据包的 hoplimit 值。
SOCAT_IPV6_DSTADDR（输出）
对于应用了地址选项ipv6-recvpktinfo 的所有基于 IPv6 的 RECVFROM 地址，socat 将此变量设置为接收数据包的目标地址。
SOCAT_IPV6_TCLASS（输出）
对于应用了地址选项ipv6-recvtclass的所有基于 IPv6 的 RECVFROM 地址， socat将此变量设置为接收数据包的传输类。
SOCAT_OPENSSL_X509_ISSUER（输出）
对等证书的颁发者字段
SOCAT_OPENSSL_X509_SUBJECT（输出）
对等证书的主题字段
SOCAT_OPENSSL_X509_COMMONNAME（输出）
来自对等证书主题的 commonName 条目。多个值用“//”分隔。
SOCAT_OPENSSL_X509_*（输出）
来自对等证书主题的所有其他条目
SOCAT_OPENSSL_X509V3_DNS（输出）
来自对等证书扩展的 DNS 条目 - subjectAltName 字段。多个值用“//”分隔。
主机名（输入）
用于确定日志记录的主机名（请参阅 -lh）。
日志名称（输入）
如果没有给出socksuser ，则用作 socks 客户端用户名的名称 。
使用选项su和 su-d，将 LOGNAME 设置为给定的用户名。
用户（输入）
如果没有给出socksuser并且 LOGNAME 为空，则用作 socks 客户端用户名的名称 。
使用选项su和 su-d， USER 设置为给定的用户名。
外壳（输出）
使用选项su和 su-d， SHELL 被设置为给定用户的登录 shell。
路径（输出）
可以使用选项路径设置exec和 系统地址。
主页（输出）
使用选项su和 su-d， HOME 设置为给定用户的主目录。

FSF（GNU，http : //www.fsf.org/ ）项目及其免费和可移植的开发软件以及许多其他有用的工具和库。

Linux 开发者社区( http://www.linux.org/ ) 提供免费 的开源操作系统。

Open Group ( http://www.unix-systems.org/ ) 在Internet 上免费提供其标准规范。

Socat主页http://www.dest-unreach.org/socat/
~~~

~~~bash
socat TCP-LISTEN:8091,TCP-LISTEN:8092

socat TCP:192.168.0.46:8091 TCP:127.0.0.1:22

socat TCP:192.168.0.46:8091 TCP:127.0.0.1:22
EXEC:/bin/bash



socat tcp4-listen:8091 tcp4-listen:8092
~~~

