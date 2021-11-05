 

# netcat的基本使用

netccat是一个通过TCP/UDP在网络中进行读写数据工具（命令），被称为“瑞士军刀”，主要用于调试领域、传输领域甚至黑客攻击领域。利用该工具，可以将网络中一端的数据完整的发送至另一台主机终端显示或存储，常见的应用为文件传输、与好友即时通信、传输流媒体或者作为用来验证服务器的独立的客户端。当然，也可以在脚本中使用该工具。







2.2netcat常用功能
2.2.1端口扫描
1.查看某IP端口开放状态
命令：netcat -v ip port
实例1：netcat -v www.baidu.com 90

通过查看百度的90端口，可以发现该端口为闭合状态。
实例2：netcat -v www.baidu.com 80

通过查看百度的80端口，可以发现该端口为开放状态。
2.查看某IP的端口段（注意扫描速度会很慢）
命令：netcat -v ip port1-port2
实例：netcat -v www.baidu.com 78-82


2.2.2即时通信
netcat可以作为简单的聊天工具使用，这个功能在只能连接局域网的网络环境中特别方便。
假设现在有两台主机A和B，A想要和B进行简单的信息通信，那么可以指定其中一台主机作为server，绑定自己的一个端口作为通信端口（例如将主机A作为server，将其10000作为自己的通信端口）。
命令：netcat -l port
实例：netcat -l 10000

然后将主机B作为客户端去连接主机A的1000端口（前提是要知道主机A的IP地址）
命令：netcat server_ip port
实例：netcat 192.168.237.131 10000

在server端和client端都搭建好了以后，就可以互相进行即时通信


注意在使用netcat进行即时通信的时候，要首先搭建server端然后搭建client端。

2.2.3文件传输
在两台主机之间传送文件除了可以使用ftp软件，也可以使用netcat进行文件传输。

2.2.3.1单个文件传输
假设现在有两台主机A和B，A想要和B进行文件传送，那么可以指定其中一台主机作为server，绑定自己的一个端口作为通信端口（例如将主机A作为server，将其10000作为自己的传送端口）
命令：netcat -l port < file
实例：首先使用“cd”命令进入传送文件所在目录，然后输入：netcat -l 1000 < test.txt

然后在主机B中接收文件
命令：netcat -n server_ip port > file
实例：首先使用“cd”命令进入想要保存文件的目录，然后输入：netcat -n 192.168.237.131 10000 >receive.txt（注意接收文件名可以和传送文件的文件名不一样）

这样作为server端的主机中的文件就传送至client端，但是每次只能传送一个文件。

2.2.3.2多个文件传输
若我们要发送多个文件或者整个目录，也可以实现，此时需要一个tar压缩工具。
在主机A进行发送：
命令：tar -cvf - server_dir_name | netcat -l port
实例：首先使用“cd”命令进入传送文件夹所在目录（/home/dlri/Desktop/Tests/），然后输入tar -cvf - /home/dlri/Desktop/Tests/ | netcat -l 10000

然后在主机B指定位置接收：
命令：netcat -n server_ip port | tar -xvf -
实例：首先使用“cd”命令进入想要保存文件的目录，然后输入netcat -n 192.168.237.132 10000 | tar -xvf -

当然，可以使用bzip2或其他压缩工具进行压缩传送。

2.2.4视频流传输
主机A作为server，将视频发至指定端口：
命令：cat video.avi | netcat -l port
实例：首先使用“cd”命令进入想要保存文件的目录，然后输入：cat a.avi | netcat -l 10000
主机B作为client，接收视频：
命令：netcat server_ip port | mplayer -vo x11 -cache 3000 -
实例：netcat 192.168.237.132 1000 | mplayer -vo x11 -cache 3000 -

2.2.5指定源端口
在2.2.2中，client接收使用的随机端口接收server端的数据，要想使用特定端口接收数据可以使用参数“-p”来指定
命令：netcat server_ip port1 -p port2
实例：netcat 192.168.237.131 10000 -p 1234

注意使用1024以内的端口需要root权限。

2.2.6指定源地址
在2.2.3中，client使用一个网卡接收文件，当client有多块网卡的时候，可以通过参数“-s”指定接收文件的网卡。
命令：netcat server_ip port -s client_ip > file
实例：netcat 192.168.237.131 10000 -s 192.168.237.132 > receive.txt


2.2.7创建远程shell
常见的netcat分为GNU和OPENBSD版本，Ubuntu14.04自带OPENBSD版本，若想使用netcat创建远程shell，则要下载安装GNU版本。
下载地址：http://netcat.sourceforge.net/

解压：
下载完成后，将netcat tar包解压至指定文件夹中，例如解压至/opt： tar -zxvf netcat-0.7.1.tar.gz -C /opt

编译安装：
进入解压后的文件夹，依次输入：
./configure
make
sudo make install

安装完成。

因为Ubuntu已经安装了netcat，所以出现了重名，安装完后输入：/usr/local/bin/netcat -h，若出现红框所标识的内容，则表示安装成功。

现将主机A作为server：
命令：/usr/local/bin/netcat -l -p port -e /bin/bash
实例：/usr/local/bin/netcat -l -p 10000 -e /bin/bash

主机B作为client：
命令：/usr/local/bin/netcat server_ip port
实例：/usr/local/bin/netcat 192.168.237.132 10000

然后主机B就能登陆到主机A的shell：
在主机B使用“ls -l”命令查看到的内容：

在主机A使用“ls -l”命令查看到的内容：

需要注意的是，黑客可能会利用这个功能来进行远程控制（被控制的主机被称为“肉鸡”）。

以上。

参考文档：
https://blog.csdn.net/chengtong222/article/details/64131358
https://www.oschina.net/translate/linux-netcat-command
http://www.jb51.net/LINUXjishu/409856.html
https://blog.csdn.net/robertsong2004/article/details/50751339


























————————————————
版权声明：本文为CSDN博主「wangqingchuan92」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/wangqingchuan92/article/details/79666885
