Linux系统克隆为iso镜像盘(类似win gost)

原创 王艳辉  Linux技术宅  2024-01-11 07:30 发表于北京
图片
点击上方：Linux技术宅，关注我！！！
Every Day
绝句
[作者] 杜甫 [朝代] 唐
两个黄鹂鸣翠柳，一行白鹭上青天。
窗含西岭千秋雪，门泊东吴万里船。
图片
简介
    Mondo Rescue（以下简称Mondo）可以说是Linux 下的Ghost，它可以将你的系统像照相一样备份至磁带，CD-R，CD-RW，NFS或硬盘分区。Mondo广泛支援LVM，RAID，ext2, ext3, JFS, XFS,ReiserFS,VFAT等。这个软件可以让大家一步一步地将Linux系统备份出来，一旦今后出了问题，只要用创建好的MondoCD将系统还原至上次备份时的状态即可。
    与Ghost相似，在进行备份的使用，应把应用与数据的备份分开处理。也就是说，使用Mondo来备份系统的基本应用配置，生产数据使用资料备份的方式处理，如定时备份到磁带等。
官网：http://www.mondorescue.org/
依赖下载：http://www.mondorescue.org/ftp
图片
准备
    备份前，先把需要部署的应用都安装和配置完成。然后，重新改系统，挂载需要备份的分区，停止已经启动的应用，建议把系统保留在“干净”的状态下进行备份的工作。备份时的方式有多种，建议先备份到本地磁盘上。所以，需要在本地准备一个空闲空间，可足够存放后续备份的光盘镜像。
图片
安装软件
环境
# 最小化安装
OS: CentOS Linux release 7.9.2009 (Core)
安装wget
# 保证可访问互联网或者使用本地YUM源
[root@localhost ~]# yum install -y wget
配置软件源
[root@localhost ~]# wget ftp://ftp.mondorescue.org/centos/7/x86_64/mondorescue.repo -O /etc/yum.repos.d/mondo.repo
[root@localhost ~]# sed -i "s/gpgcheck=1/gpgcheck=0/g" /etc/yum.repos.d/mondo.repo

[root@localhost ~]# cat /etc/yum.repos.d/mondo.repo
[mondorescue]
name=centos 7 x86_64 - mondorescue Vanilla Packages
baseurl=ftp://ftp.mondorescue.org//centos/7/x86_64
enabled=1
gpgcheck=0
gpgkey=ftp://ftp.mondorescue.org//centos/7/x86_64/mondorescue.pubkey ftp://ftp.mondorescue.org//centos/7/x86_64/mondorescue-0x141B9FF237DB9883.pubkey
安装mondo
[root@localhost ~]# yum -y install mondo
软件包离线安装(在软件下载目录执行)，可后台回复“mondo”获取：
rpm -ivh *.rpm --nodeps --force
图片
开始克隆
执行命令
[root@localhost ~]# mondoarchive
选择备份到本地硬盘。
图片
设置生成的镜像文件存放路径。
图片
选择压缩格式gzip。
图片
选择压缩率。
图片
设置如果系统比较庞大，为方便今后刻录光盘，可把镜像分块，在这里输入每个镜像光盘的大小：（一般，CD最大700MB，DVD最大4480MB）。
图片
设置镜像名称。生成多个镜像时会自动添加序号进行区分。
图片
设置备份路径，默认即可（根路径）。
图片
设置无需备份的目录，以‘|’隔开，默认不备份目录（/tmp和/proc）。
图片
设置临时目录，默认即可。
图片
设置暂存目录，默认即可。
图片
是否要备份扩展属性？
图片
备份的内核版本，默认即可。
图片
创建后是否验证？默认即可。
图片
操作是否继续？默认即可。
图片
开始克隆备份...，时间根据使用空间及备份系统数据大小决定。
图片
备份文件系统。
图片
备份大文件。
图片
创建ISO镜像。
图片
验证ISO镜像压缩包。
图片
验证ISO镜像的大文件。
图片
克隆备份完成。
图片
查看备份文件（具体路径请查看第二步设置）。
[root@localhost ~]# ll /tmp/mondo/
total 1356260
-rw-r--r--. 1 root root 1388810240 Jan  9 14:47 test-1.iso
[root@localhost ~]# du -sh /tmp/mondo/test-1.iso
1.3G    /tmp/mondo/test-1.iso
图片
系统恢复
导出上面生成的ISO文件，安装系统方式：直接使用ISO文件或者刻录光盘后时使用。
输入nuke自动安装。
图片
图片
输入exit退出，重启进入系统。
图片
登录系统（账号密码与原系统一样）
图片

图片
Other
目标系统与克隆系统中磁盘有差异的，恢复时会提示修改相关配置。如有重要数据请保持磁盘信息一致。


Linux技术宅
一个比较宅的技术小白，本公众号主要用于Linux、Python、SQL、网络通信等方面相关知识的分享！
54篇原创内容
公众号
图片
分享、在看与点赞
只要你点，我们就是胖友
图片
​​

王艳辉
 谢谢你的支持！ 
喜欢作者
Linux
61
ISO
3
ISO · 目录
上一篇
制作基于KVM / QEMU的Windows 10虚拟机
个人观点，仅供参考
阅读 1743

Linux技术宅

关注
分享
收藏
2
10
​
人划线
