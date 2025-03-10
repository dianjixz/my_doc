 

1、双向同步：unison+inotify

2、单向同步：rsync+inotify

[lsyncd](https://axkibe.github.io/lsyncd/)



https://www.cnblogs.com/shengulong/p/6760605.html





https://www.cnblogs.com/zuiyirenjian/archive/2012/08/11/2633675.html



https://www.iteye.com/blog/davidlee1986-1919416



# unison

一:Unison简介
Unison是windows和unix平台下都可以使用的文件同步工具，它能使两个文件夹（本地或网络上的）保持内容的一致。Unison有文字界面和图形界面，这里只介绍如何在文字界面下使用. unison拥有其它一些同步工具或文件系统的相同特性，但也有自己的特点：
   跨平台使用；
   对内核和用户权限没有特别要求；
   unison是双向的，它能自动处理两分拷贝中更新没有冲突的部分，有冲突的部分将会显示出来让用户选择更新策略；
   只要是能连通的两台主机，就可以运行unison，可以直接使用socket连接或安全的ssh连接方式，对带宽的要求不高，使用类似rsync的压缩传输协议。

二:安装unison
unison各种版本下载地址：
http://www.seas.upenn.edu/~bcpierce/unison//download.html
unison编译器下载地址：
http://caml.inria.fr/pub/distrib/ocaml-3.10
其它有用链接；
http://worldhello.net/doc/bcp/unison.html
http://www.seas.upenn.edu/~bcpierce/unison//contrib.html
http://www.cis.upenn.edu/~bcpierce/unison/download/releases/stable/unison-manual.html#rshmeth
http://linux.chinaunix.net/bbs/viewthread.php?tid=888250&extra=page%3D1%26amp%3Bfilter%3Dreward
从以上地址可以下载各种平台,各种版本的unison,有基于源码安装的,有二进制的,我下载的是二进制的,可以直接使用.这里介绍源码安装:
源码安装unison
Linux下从源码包编译安装，需要一个叫做Objective Caml compiler的工具，版本至少3.0.7，可以从这里下载：http://caml.inria.fr/
Ocaml安装脚本如下：
\# tar -zxf ocaml-3.09.3.tar.gz
\# cd ocaml-3.09.3
\# ./configure
\# make world opt
\# make install
Unison对版本要求很高，进行同步的两台主机需要相同版本的unison，所以这里使用和windows一致的版本2.13.16，unison-2.13.16.tar.gz
安装脚本如下：
\# tar -zxf unison-2.13.16.tar.gz
\# cd unison-2.13.16
\# make UISTYLE=text
\# make install
之后将生成可执行文件unison，将其cp到系统PATH即可。
\# cp ./unison /usr/local/bin

三:配置双机ssh信任
由于unison在远程同步文件夹要登陆远程服务器,因此要配置两机互相信任
本例假设本地机为：10.178.1.132(linux)  远程机：10.178.1.110(solaris)

\1.  在两台机器上创建 RSA密钥
以下操作要在本地机和远程机上都执行一遍
  （1）以 root 用户登录
  （2）在 root 用户的 主目录内创建.ssh 目录并设置正确的权限
  [root@gsgatzhapp1 ~]# mkdir ~/.ssh
  [root@gsgatzhapp1 ~]# chmod 700 ~/.ssh
 （3）使用 ssh-keygen 命令生成第 2 版本的 SSH 协议的 RSA 密钥
  [root@gsgatzhapp1 ~]# ssh-keygen -t rsa
  Generating public/private rsa key pair.
  Enter file in which to save the key (/root/.ssh/id_rsa):
  Enter passphrase (empty for no passphrase):
  Enter same passphrase again:
  Your identification has been saved in /root/.ssh/id_rsa.
  Your public key has been saved in /root/.ssh/id_rsa.pub.
  The key fingerprint is:
  17:e4:7c:79:8d:a0:00:3b:d9:f7:7a:56:f3:ac:54:4d oracle@gsgatzhapp1
  在提示保存私钥（key）和公钥（public key）的位置时，使用默认值。如果需要私钥密码（passphrase），则输入一个私钥密码（如果使用私钥密码，使用 ssh 执行远程命令时需要输入私钥密码，因此，本案例中未使用私钥密码），因此，直接回车即可。

  \2. 添加密钥到授权密钥文件（authorized key file）中
  （1）以 root 用户登录
  （2）在本地机上执行
   [root@gsgatzhapp1 ~] # cd ~/.ssh
   [root@gsgatzhapp1.ssh]#ssh 10.178.1.132 cat /root/.ssh/id_rsa.pub >> authorized_keys
   [oracle@gsgatzhapp1.ssh]#ssh 10.178.1.110 cat /root/.ssh/id_rsa.pub >> authorized_keys
   [oracle@gsgatzhapp1.ssh]#scp authorized_keys 10.178.1.110:/root/.ssh/
   [oracle@gsgatzhapp1 .ssh]# chmod 600 /root/.ssh/authorized_keys
 （3）在远程机10.178.1.110 上：
   bash-2.05# chmod 600 /root/.ssh/authorized_keys
 （4）测试
   完成后，在 gsgatzhapp1 上执行：
   [root@gsgatzhapp1 ~]# ssh 10.178.1.132 date
   [root@gsgatzhapp1 ~]#ssh 10.178.1.110 date
   如果不需要输入密码就出现系统日期，说明 SSH 配置成功。

四:unison的使用

Unison可以在一台主机上使用，同步两个文件夹，也可以在网络上是用。
1:本地使用
使用方法：
\#unison 111 222 #同步本地的111和222文件夹
Contacting server...
Looking for changes
Warning: No archive files were found for these roots. This can happen either
because this is the first time you have synchronized these roots,
or because you have upgraded Unison to a new version with a different
archive format.
Update detection may take a while on this run if the replicas are
large.
Unison will assume that the 'last synchronized state' of both replicas
was completely empty. This means that any files that are different
will be reported as conflicts, and any files that exist only on one
replica will be judged as new and propagated to the other replica.
If the two replicas are identical, then no changes will be reported.
Press return to continue.[<spc>] Reconciling changes
111     222       
     <---- file    aaaaaaaaaaaaa [f] ?
Commands:
 <ret> or f or <spc>  follow unison's recommendation (if any)
 I           ignore this path permanently
 E           permanently ignore files with this extension
 N           permanently ignore paths ending with this name
 m           merge the versions
 d           show differences
 x           show details
 l           list all suggested changes
 p or b        go back to previous item
 g           proceed immediately to propagating changes
 q           exit unison without propagating any changes
 /           skip
 \> or .        propagate from left to right
 < or ,        propagate from right to left

<---- file    aaaaaaaaaaaaa [f] f

Proceed with propagating updates? [] y
Propagating updates

UNISON started propagating changes at 15:06:08 on 27 Aug 2007
[BGN] Copying aaaaaaaaaaaaa
 from /222
 to /111
[END] Copying aaaaaaaaaaaaa
UNISON finished propagating changes at 15:06:08 on 27 Aug 2007
Saving synchronizer state
Synchronization complete (1 item transferred, 0 skipped, 0 failures)
如果检测到两个文件夹有所不同，unison会提示，让你选择相应的操作。如上例所示.
表示右边222的文件夹有新的文件，是否同步到左边的111文件夹，f表示force，然后将确认,进行更新,如果输入？会有更详细的介绍。

2: unison远程使用
使用方法：
\# unison <本地目录> ssh://remotehostname(IP)/<远程目录的绝对路径>
例如:
\# unison /home/AAA ssh://username@remotehostname(ip)//DB/path/BBB
表示将本机的目录/home/AAA和远端主机的/DB/path/BBB进行同步。一般的，需要两台机能ssh连接。
注意 在主机和目录间又多加了一个 "/"

3:unison参数说明
Unison有很多参数,这里只介绍经常使用的几个,详细的请参看unison手册:
"   -testserver
测试连通性，连接到服务器即退出。示例:
$ unison / ssh://opensou1@bluehost/  -servercmd=~/bin/unison -testserver
如果服务器端 unison 可执行文件不在默认目录下，甚至没有 unison 命令（需要你编译一个上传到服务器），则需要使用 -servercmd 参数告诉要执行的服务器 unison 命令位置。
使用 -testserver 参数，则成功链接即退出，也不会去执行目录的比较等后续操作。
"   -servercmd xxx
告诉 unison， 服务器端的 unison 命令是什么。参见上面的示例。
"   -auto
接受缺省的动作，然后等待用户确认是否执行。
"   -batch
batch mode, 全自动模式，接受缺省动作，并执行。
"   -ignore xxx
增加 xxx 到忽略列表中
"   -ignorecase [true|false|default]
是否忽略文件名大小写
"   -follow xxx
是否支持对符号连接指向内容的同步
"   owner = true (保持同步过来的文件属主)
"   group = true (保持同步过来的文件组信息)
"   perms = -1  (保持同步过来的文件读写权限)
"   repeat = 1  (间隔1秒后,开始新的一次同步检查)
"   retry = 3  (失败重试)
"   sshargs = -C (使用ssh的压缩传输方式)
"   xferbycopying = true

"   -immutable xxx
不变目录，扫描时可以忽略
"   -silent
安静模式
"   -times
同步修改时间
"   -path xxx 参数
只同步 -path 参数指定的子目录以及文件，而非整个目录。-path 可以多次出现,例如
 unison /home/username ssh://remotehost//home/username \
   -path shared \
   -path pub \
   -path .netscape/bookmarks.html

4:通过配置文件来使用unison
尽管可以完全通过命令行的方式来指定unison运行所需要的参数，但我们还是推荐使用配置文件来进行配置使用unison，原因很简单，看配置文件比看命令行容易理解，而且可管理性更强。
默认的配置文件夹位于~currentuser/.unison，即当前用户的home目录下，windows则位于C:\Documents and Settings\currentuser\.unison，默认的配置文件名是default.prf.
运行这样的命令:
\#unison exitgogo
Unison将默认读取~currentuser/.unison/exitgogo.prf文件里的配置信息.我的配置信息在/root/.unison/exitgogo.prf,因此我们可以根据上面参数的介绍,把所有的参数配置信息写入到一个.prf的文件中.
下面是我的一个web应用中两个文件夹同步的配置信息:
root = /sina/webdata
root = ssh://root@192.168.60.121//sina/webdata
\#force =/sina/webdata
ignore = Path as/*
\#prefer = ssh://root@192.168.60.121//sina/webdata
batch = true
\#repeat = 1
\#retry = 3
owner = true
group = true
perms = -1
fastcheck=false
rsync =false
\#debug=verbose
sshargs = -C
xferbycopying = true
log = true
logfile = /root/.unison/sina_122.1547.log
说明如下:
两个root表示需要同步的文件夹
force表示以本地的/var/www/bbsnew文件夹为标准，将该目录同步到远端。注意,如果指定了force参数,那么unison就变成了单项同步了,也就是说会以force指定的文件夹为准进行同步.
Unison本身是可以双向同步的,但是要做到双向同步,就不要设置force参数,如果设置了force参数,就成了单项同步了,此时unison类似与sync.
Unison双向同步基本原理是:假如有A B两个文件夹,A文件夹把自己的改动同步到B,B文件夹也把自己的改动同步到A,最后A B两文件夹的内容相同,是 A B文件夹的合集.
Unison双向同步的一个缺点是,对于一个文件在两个同步文件夹中都被修改时,unison是不会去同步的,因为unison无法判断以那个为准.
ignore = Path表示忽略/sina/webdata下面的WEB-INF/tmp目录,即同步时不同步它。
batch = true,表示全自动模式，接受缺省动作，并执行
-fastcheck true表示同步时使用文件的创建时间来比较两地文件，如果这个选项为false，unison则将比较两地文件的内容.建议设置为true
log = true表示在终端输出运行信息。
logfile则指定了同时将输出写入log文件。

五:unison FAQ
如何在和远程服务器同步大量数据，上传一部分数据后，超时：
9% 559:15 ETARead from remote host bluehost: Connection reset by peer
Fatal error: Lost connection with the server
实际操作中，最好的方法是,第一次先把要上传的文件打成包，用 ftp 上传，然后展开到服务器中，之后执行一次 unison 同步即可。

原文地址 http://www.263mail.com.cn/blog/ + http://ixdba.blog.51cto.com/2895551/584334

完！



[unison@server2 ~]$ vim /home/unison/.unison/default.prf


#Unison preferences file
root = /home/unison/test
root = ssh://unison@192.168.137.61//home/unison/test/
#force =
#ignore =
batch = true
#repeat = 1
#retry = 3
owner = true
group = true
perms = -1
fastcheck = false
rsync = false
sshargs = -C
xferbycopying = true
log = true
logfile = /home/unison/.unison/unison.log


相关注解如下：
force表示会以本地所指定文件夹为标准，将该目录同步到远端。这里需要注意，如果指定了force参数，那么Unison就变成了单项同步了，也就是说会以force指定的文件夹为准进行同步，类似与rsync。
Unison双向同步基本原理是：假如有A B两个文件夹，A文件夹把自己的改动同步到B，B文件夹也把自己的改动同步到A，最后A B两文件夹的内容相同，是AB文件夹的合集。
Unison双向同步的一个缺点是，对于一个文件在两个同步文件夹中都被修改时，unison是不会去同步的，因为unison无法判断以那个为准。
ignore = Path表示忽略指定目录，即同步时不同步它。
batch = true，表示全自动模式，接受缺省动作，并执行。
-fastcheck true 表示同步时仅通过文件的创建时间来比较，如果选项为false，Unison则将比较两地文件的内容。
log = true 表示在终端输出运行信息。
logfile 指定输出的log文件。


另外，Unison有很多参数，这里仅介绍常用的几个，详细的请参看Unison手册。
-auto //接受缺省的动作，然后等待用户确认是否执行。
-batch //batch mode, 全自动模式，接受缺省动作，并执行。
-ignore xxx //增加 xxx 到忽略列表中
-ignorecase [true|false|default] //是否忽略文件名大小写
-follow xxx //是否支持对符号连接指向内容的同步
owner = true //保持同步过来的文件属主
group = true //保持同步过来的文件组信息
perms = -1 //保持同步过来的文件读写权限
repeat = 1 //间隔1秒后,开始新的一次同步检查
retry = 3 //失败重试
sshargs = -C //使用ssh的压缩传输方式
xferbycopying = true"
-immutable xxx //不变目录，扫描时可以忽略
-silent //安静模式
-times //同步修改时间
-path xxx 参数 //只同步 -path 参数指定的子目录以及文件，而非整个目录，-path 可以多次出现。


PS：Windows下的unison配置文件默认位于C:\Documents and Settings\currentuser\.unison目录，默认的配置文件名是default.prf。
-----------------------------------
©著作权归作者所有：来自51CTO博客作者xmlgrg的原创作品，请联系作者获取转载授权，否则将追究法律责任
双向同步软件Unison的安装与配置
https://blog.51cto.com/xmlgrg/1792167



# rsync

~~~
sshpass -p "root" rsync -rvlzt -e 'ssh -p 15630' ./dist root@127.0.0.1:/root
~~~



# 2.3 rsync三种工作方式

以下是rsync的语法：

```
Local:  rsync [OPTION...] SRC... [DEST]
 
Access via remote shell:
  Pull: rsync [OPTION...] [USER@]HOST:SRC... [DEST]
  Push: rsync [OPTION...] SRC... [USER@]HOST:DEST
 
Access via rsync daemon:
  Pull: rsync [OPTION...] [USER@]HOST::SRC... [DEST]
        rsync [OPTION...] rsync://[USER@]HOST[:PORT]/SRC... [DEST]
  Push: rsync [OPTION...] SRC... [USER@]HOST::DEST
        rsync [OPTION...] SRC... rsync://[USER@]HOST[:PORT]/DEST
```

由此语法可知，rsync有三种工作方式：

(1).本地文件系统上实现同步。命令行语法格式为上述"Local"段的格式。

(2).本地主机使用远程shell和远程主机通信。命令行语法格式为上述"Access via remote shell"段的格式。

(3).本地主机通过网络套接字连接远程主机上的rsync daemon。命令行语法格式为上述"Access via rsync daemon"段的格式。

前两者的本质是通过管道通信，即使是远程shell。而方式(3)则是让远程主机上运行rsync服务，使其监听在一个端口上，等待客户端的连接。

# 2.4 选项说明和示例

接下来是rsync的选项说明。

```
-v：显示rsync过程中详细信息。可以使用"-vvvv"获取更详细信息。
-P：显示文件传输的进度信息。(实际上"-P"="--partial --progress"，其中的"--progress"才是显示进度信息的)。
-n --dry-run  ：仅测试传输，而不实际传输。常和"-vvvv"配合使用来查看rsync是如何工作的。
-a --archive  ：归档模式，表示递归传输并保持文件属性。等同于"-rtopgDl"。
-r --recursive：递归到目录中去。
-t --times：保持mtime属性。强烈建议任何时候都加上"-t"，否则目标文件mtime会设置为系统时间，导致下次更新
          ：检查出mtime不同从而导致增量传输无效。
-o --owner：保持owner属性(属主)。
-g --group：保持group属性(属组)。
-p --perms：保持perms属性(权限，不包括特殊权限)。
-D        ：是"--device --specials"选项的组合，即也拷贝设备文件和特殊文件。
-l --links：如果文件是软链接文件，则拷贝软链接本身而非软链接所指向的对象。
-z        ：传输时进行压缩提高效率。
-R --relative：使用相对路径。意味着将命令行中指定的全路径而非路径最尾部的文件名发送给服务端，包括它们的属性。用法见下文示例。
--size-only ：默认算法是检查文件大小和mtime不同的文件，使用此选项将只检查文件大小。
-u --update ：仅在源mtime比目标已存在文件的mtime新时才拷贝。注意，该选项是接收端判断的，不会影响删除行为。
-d --dirs   ：以不递归的方式拷贝目录本身。默认递归时，如果源为"dir1/file1"，则不会拷贝dir1目录，使用该选项将拷贝dir1但不拷贝file1。
--max-size  ：限制rsync传输的最大文件大小。可以使用单位后缀，还可以是一个小数值(例如："--max-size=1.5m")
--min-size  ：限制rsync传输的最小文件大小。这可以用于禁止传输小文件或那些垃圾文件。
--exclude   ：指定排除规则来排除不需要传输的文件。
--delete    ：以SRC为主，对DEST进行同步。多则删之，少则补之。注意"--delete"是在接收端执行的，所以它是在
            ：exclude/include规则生效之后才执行的。
-b --backup ：对目标上已存在的文件做一个备份，备份的文件名后默认使用"~"做后缀。
--backup-dir：指定备份文件的保存路径。不指定时默认和待备份文件保存在同一目录下。
-e          ：指定所要使用的远程shell程序，默认为ssh。
--port      ：连接daemon时使用的端口号，默认为873端口。
--password-file：daemon模式时的密码文件，可以从中读取密码实现非交互式。注意，这不是远程shell认证的密码，而是rsync模块认证的密码。
-W --whole-file：rsync将不再使用增量传输，而是全量传输。在网络带宽高于磁盘带宽时，该选项比增量传输更高效。
--existing  ：要求只更新目标端已存在的文件，目标端还不存在的文件不传输。注意，使用相对路径时如果上层目录不存在也不会传输。
--ignore-existing：要求只更新目标端不存在的文件。和"--existing"结合使用有特殊功能，见下文示例。
--remove-source-files：要求删除源端已经成功传输的文件。
```





用法1：本地用法

类似于cp、dd命令，实现备份文件的复制（备份)
~~~
# rsync /etc/passwd /home/passwd.bak
# rsync -b --suffix=.bak2 --backup-dir=/tmp/ /etc/passwd /home/passwd.bak
--suffix=xxx        指定旧备份文件的后缀名
--backup-dir=xxxx   指定将旧备份文件移动到哪个位置下
~~~


用法2：远程shell
~~~
利用ssh实现数据的远程传输，类似于 scp
拉取：rsync -v -e "ssh -p 22" root@192.168.31.201:/home/passwd /tmp/
推送：rsync -v /root/c.sh root@192.168.31.201:/home/
说明

    rsync默认也是基于ssh来实现的，如果目标主机的ssh端口不是22，那么在用rsync的必须用-e选项
    rsync通常都需要首先做密钥分发
    rsync在远程传输文件的时候，仅仅会传输新文件
    在进行文件传输过程中，必须使用对方主机上的一个用户，还必须知道其密码，而且该用户还要有权限

用法3：守护进程模式

rsync的配置文件： /etc/rsyncd.conf(默认不存在)
案例：

将192.168.31.200和192.168.31.202上的数据备份到192.168.31.201上，要求：
1）备份到201的/data/backup
2)201上的rsync以ruser身份运行
准备工作：关闭防火墙，关闭selinux
第一步：配置192.168.31.201（用于接收各主机节点传递过来的备份）
~~~
    创建目录
~~~
# mkdir -pv /data/backup
~~~

    
    创建用于运行rsync进程的用户ruser
~~~
# useradd  -r  -s /sbin/nologin -u 361 ruser
~~~

    
    修改目录的属主和属组
~~~
# chown -R 361.361 /data
~~~

    
    修改（创建）rsync的配置文件：/etc/rsyncd.conf
~~~
# vim /etc/rsyncd.conf 
pid file=/var/lock/subsys/rsync.pid
lock file=/var/lock/subsys/rsync.lock
uid=361
gid=361
log file=/var/log/rsync.log
timeout=100

[dir1]
path=/data/backup
max connections=100
use chroot=yes
read only=no
list=yes
auth users=suser
secrets file=/etc/rsync.pwd
hosts allow=192.168.31.0/24
~~~

    
    创建虚拟用户文件，并创建虚拟用户
~~~
# echo "suser:123" >>/etc/rsync.pwd
~~~

    
    修改虚拟用户文件的权限为600
~~~
# chmod 600  /etc/rsync.pwd
~~~

    
    启动rsync服务
~~~
# rsync --daemon --config=/etc/rsyncd.conf
~~~

    
    检查是否启动成功
~~~
# ss -tnl | grep 873
~~~


【至此，rsyncd就配置完成了，然后就可以在其他节点向rsync传递数据了】
第二步：在192.168.31.200和192.168.31.202上对192.168.31.201做拉取和推送操作

准备工作：在201的/data/backup目录下创建一个文件touch rsync.txt

    执行推送和拉取 ，测试一下
    拉取：# rsync -avz suser@192.168.31.201::dir1 ./
    在任一客户端节点执行拉取操作，没有报错并且rsync.txt被拉取到当前位置，说明rsync配置没问题。
    推送：# rsync -avz /etc/passwd suser@192.168.31.201::dir1
    在任一客户端节点执行推送操作，没有报错并且在服务端/data/backup目录下有 passwd文件说明推送也能正常完成。
    在客户端创建密码文件（方便实现自动化）
~~~
# echo "123" >>/etc/rsync_tuser.passwd
~~~

    
    修改密码文件的权限为600
~~~
# chmod 600 /etc/rsync_tuser.passwd
~~~

    
    再次执行推送/拉取，用密码文件实现认证
~~~
# rsync -avz suser@192.168.31.201::dir2 ./ --password-file=/etc/rsync_tuser.passwd
~~~


补充：选项--exclude=filename
例子：

    排除单个文件：–exclude=5.txt
    排除多个文件：–exclude={5.txt, 7.txt}
    基于通配符做排除：–exclude=*txt

传输失败的原因传输失败的原因

    用户名或者密码写错了
    密码文件指定错了
    虚拟用户密码文件权限不是600
    模块对应的文件的属主不是rsync daemon的运行者身份
————————————————
版权声明：本文为CSDN博主「cx_baby」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/cx55887/article/details/82943760









`rsync` 是一个强大的文件同步工具，可以在本地和远程之间进行增量备份。以下是如何使用 `rsync` 将本地文件增量备份到远程服务器的步骤：

### 1. 安装 `rsync`
确保本地和远程服务器上都安装了 `rsync`。大多数 Linux 发行版默认已经安装了 `rsync`，如果没有安装，可以使用以下命令安装：

- **Debian/Ubuntu**:
  ```bash
  sudo apt-get install rsync
  ```

- **CentOS/RHEL**:
  ```bash
  sudo yum install rsync
  ```

### 2. 使用 `rsync` 进行增量备份
假设你要将本地的 `/path/to/local/dir` 目录备份到远程服务器的 `/path/to/remote/dir` 目录，远程服务器的 IP 地址为 `remote_ip`，用户名为 `remote_user`。

#### 基本命令：
```bash
rsync -avz --progress /path/to/local/dir/ remote_user@remote_ip:/path/to/remote/dir/
```

#### 参数解释：
- `-a`：归档模式，保留文件权限、时间戳、符号链接等。
- `-v`：详细输出，显示同步过程中的文件信息。
- `-z`：压缩传输，减少网络传输的数据量。
- `--progress`：显示传输进度。

#### 增量备份：
`rsync` 默认就是增量备份的，它只会传输源和目标之间有差异的文件。因此，你不需要额外的参数来实现增量备份。

### 3. 使用 SSH 进行安全传输
`rsync` 默认使用 SSH 进行远程传输，因此你需要确保 SSH 已经配置好，并且你可以通过 SSH 登录到远程服务器。

如果你使用非默认的 SSH 端口，可以通过 `-e` 参数指定 SSH 端口：

```bash
rsync -avz --progress -e 'ssh -p 2222' /path/to/local/dir/ remote_user@remote_ip:/path/to/remote/dir/
```

### 4. 排除特定文件或目录
如果你希望在备份时排除某些文件或目录，可以使用 `--exclude` 参数：

```bash
rsync -avz --progress --exclude '*.log' /path/to/local/dir/ remote_user@remote_ip:/path/to/remote/dir/
```

### 5. 定时备份
你可以将 `rsync` 命令添加到 `cron` 任务中，实现定时备份。例如，每天凌晨 2 点执行备份：

1. 打开 `crontab` 编辑器：
   ```bash
   crontab -e
   ```

2. 添加以下行：
   ```bash
   0 2 * * * rsync -avz --progress /path/to/local/dir/ remote_user@remote_ip:/path/to/remote/dir/
   ```

### 6. 其他常用选项
- `--delete`：删除目标目录中源目录不存在的文件。
- `--backup`：在覆盖文件之前备份目标文件。
- `--backup-dir`：指定备份文件的存储目录。

### 示例命令：
```bash
rsync -avz --progress --delete --backup --backup-dir=/path/to/backup/dir /path/to/local/dir/ remote_user@remote_ip:/path/to/remote/dir/
```

### 总结
`rsync` 是一个非常灵活的工具，适用于各种备份和同步场景。通过合理使用参数，你可以轻松实现本地到远程的增量备份。




