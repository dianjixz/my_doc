resize2fs:调整ext文件系统的空间大小

 搭配逻辑卷lv使用方法：resize2fs /PATH/TO/LV_NAME 3G  调整LV的逻辑边界大小为3G
 resize2fs -p /PATH/TO/LV :-p表示扩展逻辑边界跟物理边界一样大。


 对img进行校验
~~~
e2fsck -p -f linuxroot.img
~~~
对img大小进行重新设定
~~~
resize2fs  -M linuxroot.img

resize2fs  -p linuxroot.img
~~~




运行dd命令刻录完镜像后可以用：sync命令来结束dd占用。

查看dd进度
~~~
sudo watch -n 5 pkill -USR1 ^dd$
~~~

动态调整分区大小的命令：
~~~
tunefs - 调整现有的UFS文件系统
growfs - 扩展现有的UFS文件系统
~~~


查看已经连接的设备
~~~
root@lsgxbsd:~ # camcontrol devlist
~~~
~~~
1. 删除ad0上所有分区

# gpart destroy -F /dev/da1

注: 没有数据情况才可以这样

2. 创建gpt分区信息表
# gpart create -s GPT /dev/da1 创建新的GPT分区表
# gpart create -s MBR /dev/da1 创建新的MBR分区表
# gpart create -s BSD /dev/da1 创建新的BSD分区表

3. 创建分区
# gpart add -b 34 -s 512k -t freebsd-boot /dev/da1
# gpart add -s 10g -t freebsd-ufs -l sys /dev/da1
# gpart add -s 4g -t freebsd-swap -l swap /dev/da1
# gpart add -s 20g -t freebsd /dev/da1

# gpart bootcode -b /boot/pmbr -p /boot/gptzfsboot -i 1 /dev/da1 （建议所有盘有freebsd-boot分区的盘都做）
# newfs -i 4096 /dev/da1p2 (启动分区和swa分区无需格式化)
# mount /dev/da1p2 /mnt/image/
# df -th
# umount /mnt/image/


-b 开始扇区,可省略
-s 分区大小,单位可用扇区,K,M等
-t 分区类型, 还有freebsd-ufs
-l 给分区打标签

4. 显示信息
# gpart show -l /dev/da1
# gpart show -r /dev/da1
# gpart show -p /dev/da1

5. 删除分区
# gpart delete -i 2 /dev/da1


# sysctl kern.geom.debugflags=16
# camcontrol devlist
# dd if=FreeBSD-10.1-RELEASE-i386-memstick.img of=/dev/da0 bs=64k

# mdconfig -a -t vnode -f /tmp/bootable.iso -u 3
# gnop create /dev/md3
# gnop list
# gpart show -p /dev/md3
# mount /dev/md3p3 /mnt/image
# mdconfig -d -u 3

查看磁盘信息
fdisk /dev/ad0
fdisk -s /dev/ad0
修改磁盘信息(问答式的交互)
fdisk -u /dev/ad0

# fsck -y /
# mount -u /
# mount -a
# mount -o remount, rw /

 

设置临时IP地址和默认路由
ifconfig em0 inet 192.168.195.99 netmask 255.255.255.0
ifconfig em0 inet6 accept_rtadv
route delete default
route add default 192.168.195.2
route add 192.168.195.0/24 192.168.195.2
route add 192.168.1.0/24 192.168.1.1
route add 192.168.2.0/24 192.168.2.1
route add 192.168.185.0/24 192.168.185.201
route flush
netstat -nr
netstat -nat

 

重启sshd服务

FreeBSD默认是不让root通过ssh登陆，遂修改相关的配置文件：
FreeBSD#vi /etc/sshd/sshd_config
#PermitRootLogin no
将这句修改成：
PermitRootLogin yes
保存退出，重启sshd服务却报错了：sshd re-exec requires execution with an absolute path
直接进入/etc/rc.d目录，输入sshd start，一样错误提示，
FreeBSD的命令执行方法或许变了，必须这样来：
FreeBSD#cd /etc/rc.d
FreeBSD#./sshd start|stop|restart
或直接输入完整的路径并带上命令操作：
FreeBSD#/etc/rc.d/sshd start|stop|restart





https://blog.csdn.net/qq_39753097/article/details/83349718?utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.nonecase&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.nonecase
Linux网络基本配置/IP/网关
一般是临时的修改，重启后失效，如：ifconfig、route、ip addr等。



eth0：网络接口
link encap : 网络类型
HWaddr : 网卡物理地址
Inet addr ：（internet address） IP地址
Bcast：广播地址
Mask：子网掩码
UP：正在使用的网络接口
RX packets,TX packets : 接收和传输的数据包个数
RX byte，TX byte : 表示接收和传输的具体数目
Interrupt：终端信息
Base address：内存地址






https://www.jb51.net/article/180111.htm







ifconfig eth0 192.168.120.56 netmask 255.255.255.0
#临时配置网卡ip子网码

route
#查看网关
添加路由条目
即时生效 ：route add default gw 192.168.1.1（自己设定）//添加默认网关

重启生效： vim  /etc/sysconfig/network
         --增加或修改下面一行
           NETWORKING=yes  

　　    GATEWAY=192.168.1.1
route命令

设置和查看路由表都可以用 route 命令，设置内核路由表的命令格式是：
Linux下用ifconfig命令设置IP、掩码、网关:https://blog.csdn.net/qq_28090573/article/details/82714028
# route  [add|del] [-net|-host] target [netmask Nm] [gw Gw] [[dev] If]

其中：

    add : 添加一条路由规则
    del : 删除一条路由规则
    -net : 目的地址是一个网络
    -host : 目的地址是一个主机
    target : 目的网络或主机
    netmask : 目的地址的网络掩码
    gw : 路由数据包通过的网关
    dev : 为路由指定的网络接口
https://www.cnblogs.com/zhaojingyu/p/9110961.html
使用 route 命令添加    

　　使用route 命令添加的路由，机器重启或者网卡重启后路由就失效了，方法：    

　　//添加到主机的路由    

　　# route add –host 192.168.168.110 (自己设置)  dev eth0    

　　# route add –host 192.168.168.119 (自己设置)  gw 192.168.168.1 (已存在的)    

　　//添加到网络的路由 

　　# route add –net IP (主机 ）netmask MASK eth0    

　　# route add –net IP netmask MASK gw IP (自己设置)    

　　# route add –net IP /24 eth1    

　　//添加默认网关    

　　# route add default gw IP (自己设置)   

　　//删除路由    

　　# route del –host 192.168.168.110 dev eth0 
3.配置网卡的硬件地址（MAC）

ifconfig eth0 hw ether xx：xx：xx：xx：xx：xx就将网卡的硬件地址更改了，此时你就可以骗过局域网内的IP地址绑定了。
原文链接：https://blog.csdn.net/qq_39753097/article/details/83349718



vim /etc/resolv.conf #设置你所要使用的DNS当自己不知道ip时，优先级最高,最多可写四个
在真机切换到超级用户 --> 去网关 --> systemctl restart network 重启网络 -->连接wifi --> 查看网关
/etc/resolv.conf是DNS客户机配置文件，用于设置DNS服务器的IP地址及DNS域名，还包含了主机的域名搜索顺序。
https://www.cnblogs.com/zhoading/p/9602423.html



linux interfaces配置文件详解
https://blog.csdn.net/mountzf/article/details/52035499
/etc/network/interfaces

　　 auto lo
　　 iface lo inet loopback
　　
　　 # The primary network interface
　　 auto eth0
　　 iface eth0 inet static
　　      address 192.168.0.42
　　      network 192.168.0.0
　　      netmask 255.255.255.0
　　      broadcast 192.168.0.255
　　      gateway 192.168.0.1




在系统的终端处修改打印级别，例如，屏蔽所有的内核printk打印，那么只需要将第一个数值调到最小值1或0即可
# echo 1 4 1 7 > /proc/sys/kernel/printk
or
# echo 0 4 0 7 > /proc/sys/kernel/printk



wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wpa_supplicant.conf -O/etc/wifi/sockets


wpa_supplicant_overlay.conf:
disable_scan_offload=1

wpa_supplicant.conf:
ctrl_interface=/etc/wifi/sockets
disable_scan_offload=1
update_config=1
wowlan_triggers=any


wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wpa_supplicant.conf -O/etc/sockets



[D-BUS Service] 
Name=fi.epitest.hostap.WPASupplicant  
Exec=/sbin/wpa_supplicant -u -f /var/log/wpa_supplicant.log 
User=root


[D-BUS Service] 
Name=fi.epitest.hostap.WPASupplicant  
Exec=/sbin/wpa_supplicant -u -onl80211 -O/var/run/wpa_supplicant 
User=root




../configure CC=aarch64-linux-gnu-gcc CXX=aarch64-linux-gnu-g++ AR=aarch64-linux-gnu-ar RANLIB=aarch64-linux-gnu-ranlib --host=aarch64-linux-gnu --build=arm64 --disable-ipv6 --prefix=/home/nihao/tool/Python-3.9.5/build/nihao ac_cv_file__dev_ptc=no ac_cv_file__dev_ptmx=no --eable-loadable-sqlite-extensions

iScas1001


~~~


~~~
$$ 		Shell本身的PID（ProcessID，即脚本运行的当前 进程ID号）
$! 		Shell最后运行的后台Process的PID(后台运行的最后一个进程的 进程ID号)
$? 		最后运行的命令的结束代码（返回值）即执行上一个指令的返回值 (显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误)
$- 		显示shell使用的当前选项，与set命令功能相同
$* 		所有参数列表。如"$*"用「"」括起来的情况、以"$1 $2 … $n"的形式输出所有参数，此选项参数可超过9个。
$@ 		所有参数列表。如"$@"用「"」括起来的情况、以"$1" "$2" … "$n" 的形式输出所有参数。
$@ 		跟$*类似，但是可以当作数组用
$# 		添加到Shell的参数个数
$0 		Shell本身的文件名
$1～$n 		添加到Shell的各参数值。$1是第1参数、$2是第2参数…。 


~~~



我们在正常情况下切换虚拟终端只要用组合键：Ctrl+Alt+Fn ,这个n就是你要切换的虚拟终端的号，然后可以再按Ctrl+Alt+F7回到图形界面。

但是在vmware中这个办法经常不奏效，原因大家应该知道的，就是在vmware下默认按住 Ctrl+Alt组合键，是用来在虚拟机和物理机之间互换的。

但是有了chvt这个命令，就可以这样办拉。呵呵

比如你现在在图形界面，想切换到tty1，也就是一号终端，只要执行

chvt 1 即可

如果你想回到图形己面，只要执行

chvt 7 即可。。
————————————————
版权声明：本文为CSDN博主「杨瘦锅」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_21063873/article/details/50161893



**[bro](https://link.zhihu.com/?target=http%3A//bropages.org/)：以用例为主的帮助系统**

**[multitail](https://link.zhihu.com/?target=https%3A//www.vanheusden.com/multitail/)：多重  tail**通常你不止一个日志文件要监控，怎么办？终端软件里开多个 tab 太占地方，可以试试这个工具：

**[cheat](https://link.zhihu.com/?target=https%3A//github.com/chrisallenlane/cheat)：命令行笔记**就是各种 cheat sheet ，比如经常搞忘 redis 命令的话，你可以新建 ~/.cheat/redis 这个文件，写一些内容，比如：

**[dstat](https://link.zhihu.com/?target=http%3A//dag.wiee.rs/home-made/dstat/)：vmstat 代替者**可能不少人都用过，但是本问题下好像没人提？

**[ranger](https://link.zhihu.com/?target=https%3A//github.com/ranger/ranger)：内容浏览/预览**另一款内容预览软件，界面没有 mc 那么漂亮，标记拷贝也没它强，但是预览功能做的很不错，如果你习惯 vim 键位的话，你会发现ranger有些地方比 mc做的好不少：

**[mc](https://link.zhihu.com/?target=https%3A//midnight-commander.org/) ：内容浏览/预览** 

即便能舒适的呆在终端命令行里工作的人，有时候面对有些事情，也会感到烦躁，比如浏览一个目录的结构和里面文件内容，比如从源文件夹选择拷贝一批特定的文件到目标文件夹，这时候你需要 mc ，对就是前面 mcedit 的父项目 midnight commander ，安装方法同 mcedit：

**[glances](https://link.zhihu.com/?target=https%3A//nicolargo.github.io/glances/)：更强大的 htop / top 代替者**htop 代替 top，glances 代替 htop：



**[ncdu](https://link.zhihu.com/?target=https%3A//dev.yorhel.nl/ncdu)：可视化的空间分析程序**你发现有人把 /home 空间撑爆了，影响了大家的工作，你愤怒了一层层的 du，一层层的 cd，整个过程就像刨垃圾堆一样的恶心，后来发现了 ncdu 这个基于 ncurses 的空间分析程序：

**[pm](https://link.zhihu.com/?target=https%3A//github.com/Angelmmiguel/pm)：在 bash / zsh 中迅速切换项目目录**

**[owncloud](https://link.zhihu.com/?target=https%3A//owncloud.org/download/)：DropBox 的开源替代品，提供命令行客户端 owncloudcmd**

**[mcedit](https://link.zhihu.com/?target=https%3A//midnight-commander.org/)：终端下面的 NotePad++**

**[icdiff](https://link.zhihu.com/?target=https%3A//github.com/jeffkaufman/icdiff)：分屏显示 diff**

**[cppman](https://link.zhihu.com/?target=https%3A//github.com/aitjcize/cppman)：C++ 98/11/14 手册查询 for Linux/MacOS**

[tldr](https://link.zhihu.com/?target=https%3A//github.com/tldr-pages/tldr): 如果你经常不想详读man文档，那么你应该试试这个小工具。

[figlet](https://link.zhihu.com/?target=http%3A//www.figlet.org): 将输入字符转换成艺术字体。

[cloc](https://link.zhihu.com/?target=https%3A//github.com/cgag/loc): 用来统计代码行数，统计结果比较分类比较详细，支持大部分编程语言。

1. [WordGrinder](https://link.zhihu.com/?target=https%3A//cowlark.com/wordgrinder/)：它是一款使用起来很简单，但拥有足够的编写和发布功能的文字编辑器。它支持基本的格式和样式，并且你可以将你的文字以 Markdown、ODT、LaTeX 或者 HTML 等格式导出；

\2. [Proselint](https://link.zhihu.com/?target=http%3A//proselint.com/)：它是一款全能的实时检查工具。它会找出行话、大话、不正确日期和时间格式、滥用的术语[等等](https://link.zhihu.com/?target=http%3A//proselint.com/checks/)。它也很容易运行并忽略文本中的标记；

\3. [GNU Aspell](https://link.zhihu.com/?target=http%3A//aspell.net/)：它能够交互式地检测文本文档，能高亮显示拼写错误，还能在拼写错误的上方提供正确的拼写建议。Aspell 在进行拼写检查的时候，同样能够忽略许多语法标记；

[Alex](https://link.zhihu.com/?target=https%3A//github.com/get-alex/alex)：它是一个简单但很有用的小工具。适用于明文文本或者格式为 Markdown 或 HTML 的文档。Alex 会对“性别偏好、极端主义、种族相关、宗教，或者文章中其他不平等的措辞”产生警告。如果你想要试试看 Alex，这里有一个在线 [demo](https://link.zhihu.com/?target=https%3A//alexjs.com/%23demo)；

nmon：它能够帮你进行电脑的性能监控，包括 CPU，内存，磁盘 IO，网络 IO，并且界面很炫酷，是不是很像黑客，快去试试吧 [nmon for Linux | Main](https://link.zhihu.com/?target=http%3A//nmon.sourceforge.net/pmwiki.php)

axel：多线程断点下载工具，非常好用。例如下图中这样，指定了 8 个线程同时下载。

 [SpaceVim](https://link.zhihu.com/?target=https%3A//github.com/SpaceVim/SpaceVim)：这是一个 vim 插件，使你的 Vim 变成带代码自动补全等功能的更加强大的代码编辑器！

\9. [thefuck](https://link.zhihu.com/?target=https%3A//github.com/nvbn/thefuck)：你 git branch 打成 branch 了，然后命令行报错，你是不是心里会冒出一句 fuck？那你就在命令行里输入 fuck 然后回车！咦，成功了！

\9. [thefuck](https://link.zhihu.com/?target=https%3A//github.com/nvbn/thefuck)：你 git branch 打成 branch 了，然后命令行报错，你是不是心里会冒出一句 fuck？那你就在命令行里输入 fuck 然后回车！咦，成功了！

昨天刚下了一个感觉很实用的工具：[PM2](https://link.zhihu.com/?target=http%3A//pm2.keymetrics.io/) **用以在后台运行和管理进程**

~~~
# 开始
pm2 start <id>
# 停止
pm2 stop <id>
后台运行担心看不到 log？pm2 也已经帮你考虑了：
pm2 monit
~~~

[Httpie](https://link.zhihu.com/?target=https%3A//github.com/jakubroztocil/httpie)一个 HTTP 命令行客户端，使用起来比 curl 更为友好，并支持输出 JSON、语法高亮等特性，为开发者呈现更为直观的 UI，与服务器间的调试体验也更为人性化。

[transfer.sh](https://link.zhihu.com/?target=https%3A//github.com/dutchcoders/transfer.sh)可通过简单的命令行来快速完成文件共享操作。

[stegify](https://link.zhihu.com/?target=https%3A//github.com/DimitarPetrov/stegify)一款神奇的命令行工具，能将任意文件隐藏到图片里面。如下所示，第 3 张[图片](https://www.zhihu.com/search?q=图片&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A719555045})为 1、2 两张图片的合成品，但是单从视觉上却完全看不出来：

strace，打印出进程调用了哪些系统调用。

objdump，打印目标码和反汇编结果

jq，命令行的json查询和格式化工具，适合查看rest接口的输出

awk, grep, sed：文本处理神器。

sort, uniq：排序去重。

[history](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Sed)非常简单，尤其是在你依赖于复制使用过的命令时，这个命令特别有用。

- linux: [alebcay/awesome-shell](https://link.zhihu.com/?target=https%3A//github.com/alebcay/awesome-shell)
- mac: [herrbischoff/awesome-osx-command-line](https://link.zhihu.com/?target=https%3A//github.com/herrbischoff/awesome-osx-command-line)
- 大杂烩: [sindresorhus/awesome](https://link.zhihu.com/?target=https%3A//github.com/sindresorhus/awesome)



我个人重度使用的: [robbyrussell/oh-my-zsh](https://link.zhihu.com/?target=https%3A//github.com/robbyrussell/oh-my-zsh), [tmux/tmux](https://link.zhihu.com/?target=https%3A//github.com/tmux/tmux), [moby/moby](https://link.zhihu.com/?target=https%3A//github.com/moby/moby), [ansible/ansible](https://link.zhihu.com/?target=https%3A//github.com/ansible/ansible), 然后就是常用的命令行grep/sed/awk之类的了。

BTW, 推荐[Idnan/bash-guide](https://link.zhihu.com/?target=https%3A//github.com/Idnan/bash-guide), 把bash搞熟了才能玩转"神器"嘛



添一个http-server  [indexzero/http-server](https://link.zhihu.com/?target=https%3A//github.com/indexzero/http-server)

很多时候需要局域网共享文件，有些大文件也没法用微信/qq传，或者是从服务器上需要下载文件到本地scp会很慢，如果用python -m SimpleHTTPServer 会发现服务器性能很差，且不支持并发下载。这时就可以用http-server。

作者：Golang发烧友
链接：https://www.zhihu.com/question/59227720/answer/2243895047
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



### **1、fish shell**

毋庸置疑，在终端工具中，Shell 是使用最频繁也最重要的工具。过去，我曾经使用过 Bash 和 Z Shell，而如今，这个Fish Shell，也是一个非常优秀的终端 Shell 工具，拥有许多开箱即用的功能，例如语法自动推荐补全、语法高亮显示或使用快捷键在最近访问的文件夹之间来回切换。

![img](https://pic3.zhimg.com/50/v2-fec3266b35d4a178b67763585ed41e58_720w.jpg?source=1940ef5c)![img](https://pic3.zhimg.com/80/v2-fec3266b35d4a178b67763585ed41e58_720w.jpg?source=1940ef5c)

**下载地址：[https://fishshell.com/](https://link.zhihu.com/?target=https%3A//fishshell.com/)**

### **2、Starship**

如果必须要从本篇文章中选择一个我最喜欢的终端工具——那非 Starship 莫属。Starship 可以适用于任何 Shell。你只需要安装它，然后在相应的配置文件.bashrc/.zshrc/config.fish添加一行配置，剩下的工作交给它来完成就好了。

![img](https://pic1.zhimg.com/50/v2-6f7a188cd36c68a2af1a7b1bbe7eb4d2_720w.jpg?source=1940ef5c)![img](https://pic1.zhimg.com/80/v2-6f7a188cd36c68a2af1a7b1bbe7eb4d2_720w.jpg?source=1940ef5c)

**它可以做到：**

- 根据你是否在代码仓库中添加了新文件、是否修改了文件、是否暂存了文件等情况，用相应的符号表示 git 仓库的状态。
- 根据你所在的 Python 项目目录，展示 Python 的版本号，这也适用于 Go/Node/Rust/Elm 等其他编程语言环境。
- 展示上一个命令执行所用的时间，指令运行时间必须在毫秒级别。
- 如果上一个命令执行失败，会展示相应的错误提示符。

![img](https://pic1.zhimg.com/50/v2-eba6f8429d095124fc9adf463d1e9ab8_720w.jpg?source=1940ef5c)![img](https://pic1.zhimg.com/80/v2-eba6f8429d095124fc9adf463d1e9ab8_720w.jpg?source=1940ef5c)

还有不计其数的其他信息可以展示。但是，它能以更加友好的形式智能地给你呈现！比如，如果你不在 git 存储库中，它将隐藏 git 信息。如果您不在 Python 项目中，则不会有 Python 版本信息，因为显示它没有什么意义。它永远不会给你展示多余信息，始终保持终端的美观，优雅和简约。

**下载地址：[https://starship.rs/](https://link.zhihu.com/?target=https%3A//starship.rs/)**

### **3、z**

“z” 可以让你快速地在文件目录之间跳转。它会记住你访问的历史文件夹，经过短暂的学习后，你就可以使用z path_of_the_folder_name命令在目录之间跳转了。

![img](https://pic1.zhimg.com/50/v2-0c7f296a7649f6454dcbd5d4f48fde36_720w.jpg?source=1940ef5c)



作者：Golang发烧友
链接：https://www.zhihu.com/question/59227720/answer/2243895047
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



### **8、virtualenv 和 virtualfish**

Virtualenv 是用于在 Python 中创建虚拟环境的工具。

![img](https://pic3.zhimg.com/50/v2-8ca20fc612f088ada9ca060da14c173d_720w.jpg?source=1940ef5c)![img](https://pic3.zhimg.com/80/v2-8ca20fc612f088ada9ca060da14c173d_720w.jpg?source=1940ef5c)

VirtualFish 则是 Fish Shell 的虚拟环境管理器。它提供了许多命令来执行快速创建、列出或删除虚拟环境等操作。

**virtualenv 下载地址：[https://pypi.org/project/virtualenv/](https://link.zhihu.com/?target=https%3A//pypi.org/project/virtualenv/)**
**virtualfish 下载地址：[https://github.com/justinmayer/virtualfish](https://link.zhihu.com/?target=https%3A//github.com/justinmayer/virtualfish)**

### **9、pyenv、nodenv 和 rbenv**

pyenv 可以轻松实现 Python 版本的切换。

![img](https://pica.zhimg.com/50/v2-7468a460ca8e223e767a3eb48f7389a8_720w.jpg?source=1940ef5c)![img](https://pica.zhimg.com/80/v2-7468a460ca8e223e767a3eb48f7389a8_720w.jpg?source=1940ef5c)

Pyenv、nodenv 和 rubyenv 是用于管理计算机上不同版本的 Python、Node 和 Ruby 的工具。

假设你要在计算机上安装两个版本的 Python。比如，你正在从事两个不同的 Python 项目，或者因为特殊情况仍然需要使用 Python2。不同 Python 版本在电脑上管理很复杂。你需要确保不同的项目具有正确版本的软件依赖包。如果不小心，很容易弄乱这种脆弱的配置。

该工具为版本管理提供了很多帮助，并将这一噩梦变得易于管理。它可以全局或“按文件夹”切换 Python 版本，而且每个版本都是相互隔离的。

**pyenv 下载地址：[https://github.com/pyenv/pyenv](https://link.zhihu.com/?target=https%3A//github.com/pyenv/pyenv)**
**nodenv 下载地址：[https://github.com/nodenv/nodenv](https://link.zhihu.com/?target=https%3A//github.com/nodenv/nodenv)**
**rbenv 下载地址：[https://github.com/rbenv/rbenv](https://link.zhihu.com/?target=https%3A//github.com/rbenv/rbenv)**

### **10、pipx**

Virtualenv 解决了 Python 程序包管理中的许多问题，但是还有一个方案可以解决。如果我想在全局环境下安装 Python 软件包（比如它是一个独立的工具，正如前面提到的glances 工具），那么我会遇到全局安装带来的问题。在虚拟环境之外安装软件包不是一个好主意，将来可能会导致意想不到的问题。另一方面，如果我决定使用虚拟环境，那么每次我要运行程序时都需要激活该虚拟环境。这也不是最方便的解决方案。

![img](https://pic2.zhimg.com/50/v2-49d4e702470c345db5bd5e80796e1ca9_720w.jpg?source=1940ef5c)![img](https://pic2.zhimg.com/80/v2-49d4e702470c345db5bd5e80796e1ca9_720w.jpg?source=1940ef5c)

事实证明，pipx工具可以解决上面提到的问题。它将 Python 软件依赖包安装到单独的环境中（因此不会存在依赖项冲突的问题）。与此同时，这些工具提供的 CLI 命令在全局环境内也可用。因此，我无需激活任何环境——pipx会帮我完成这个操作！

**pipx 下载地址：[https://github.com/pipxproject/pipx](https://link.zhihu.com/?target=https%3A//github.com/pipxproject/pipx)**

### **11、ctop 和 lazydocker**

![img](https://pic3.zhimg.com/50/v2-1b7152803ad212bb67d333a372c16672_720w.jpg?source=1940ef5c)



[ansi](https://link.zhihu.com/?target=https%3A//github.com/fidian/ansi) - 在纯bash中的ANSI转义码 - 更改文本颜色，定位光标，等等

[bashful](https://link.zhihu.com/?target=https%3A//github.com/jmcantrell/bashful) - 一个库的集合，以简化编写Bash脚本

[dispatch](https://link.zhihu.com/?target=https%3A//github.com/Mosai/workshop/blob/master/doc/dispatch.md) - 50行可移植shell脚本中的命令行参数解析器。

[shutit](https://link.zhihu.com/?target=https%3A//ianmiell.github.io/shutit/) - 基于bash和pexpect的自动化框架

[rebash](https://link.zhihu.com/?target=https%3A//github.com/jandob/rebash) - 脚本库/框架。特点：进口，例外，文件测试...

[is.sh](https://link.zhihu.com/?target=https%3A//github.com/qzb/is.sh) - 内建测试命令的替代方法，它会让你的“if”语句变得非常漂亮

**edex-ui**edex-ui是一个跨平台、可定制、充满科幻感终端模拟器，支持高级监控和触摸屏。

cowsay是一个有趣的程序，可以生成带有文本内容的牛的ASCII艺术图片。

ranger：命令行文件管理器，vim风格，好用。

fzf/fasd/fd: 快速查找文件，也能模糊搜索命令行历史。

ripgrep：文件内容全文检索。

**[autojump](https://link.zhihu.com/?target=https%3A//github.com/wting/autojump)**   **�**� ⭐ （记忆进过的文件夹，下次可用 j + 文件夹名 直接跳进， 很方便）

dtrx 有了这个工具你基本再也不用记住如何在linux上面解压各种后缀的文件了，什么tar.gz,tgz,xz,bz2,zip.rar等通通搞定，

pydf 更加美观的显示磁盘占用情况

作者：郑叶平
链接：https://www.zhihu.com/question/59227720/answer/163966430
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。









作者：交叉熵业巨头
链接：https://www.zhihu.com/question/59227720/answer/163933473
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。





作者：程序员客栈
链接：https://www.zhihu.com/question/59227720/answer/578875058
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

[gitsome](https://link.zhihu.com/?target=https%3A//github.com/donnemartin/gitsome)一个非常实用的 GitHub 命令行工具，支持在命令行中查看 GitHub 通知、搜索创建 issue、浏览动态、生成 LICENSE 等功能，十分强大。















作者：韦易笑
链接：https://www.zhihu.com/question/59227720/answer/286665684
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
