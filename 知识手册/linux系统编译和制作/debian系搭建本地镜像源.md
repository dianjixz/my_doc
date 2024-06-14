 

[柴火堆里学IT](https://www.cnblogs.com/surplus/)

##              [     使用apt-mirror搭建debian镜像源         ](https://www.cnblogs.com/surplus/p/11441206.html)         

debian官方提供了脚本ftpsync来搭建源镜像，而 [apt-mirror](https://yq.aliyun.com/go/articleRenderRedirect?url=http%3A%2F%2Fapt-mirror.github.io%2F) 是一个更简单便捷的源镜像搭建工具。

**安装 apt-mirror**

```
sudo apt-get install apt-mirror
```

配置文件/etc/apt/mirror.list只要修改很少的地方，大部分使用默认值即可。

这里使用中科大镜像 ftp.cn.debian.org 作为上游镜像，只镜像 debian jessie amd64 架构，不镜像源代码包。

```
############# config ##################
#
# set base_path    /var/spool/apt-mirror
#
# set mirror_path  $base_path/mirror
# set skel_path    $base_path/skel
# set var_path     $base_path/var
# set cleanscript $var_path/clean.sh
# set defaultarch  　# 默认架构与镜像主机的架构一致,这里是amd64
# set defaultarch  amd64
# set postmirror_script $var_path/postmirror.sh
# set run_postmirror 0
set nthreads     20
set _tilde 0
#
############# end config ##############
 
deb http://ftp.cn.debian.org/debian jessie main contrib non-free
deb http://ftp.cn.debian.org/debian/ jessie-backports main contrib non-free
deb http://ftp.cn.debian.org/debian/ jessie-proposed-updates main contrib non-free
deb http://ftp.cn.debian.org/debian/ jessie-updates main contrib non-free
deb http://ftp.cn.debian.org/debian-security/ jessie/updates main contrib non-free
#deb-src http://ftp.us.debian.org/debian unstable main contrib non-free
 
# mirror additional architectures
#deb-alpha http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-amd64 http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-armel http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-hppa http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-i386 http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-ia64 http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-m68k http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-mips http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-mipsel http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-powerpc http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-s390 http://ftp.us.debian.org/debian unstable main contrib non-free
#deb-sparc http://ftp.us.debian.org/debian unstable main contrib non-free
 
clean http://ftp.cn.debian.org/debian
```

**开启自动同步**

只需 root 权限 cron 自动运行 apt-mirror 命令即可。

\# vim /etc/cron.d/apt-mirror
0 0 * * * apt-mirror > /var/spool/apt-mirror/var/cron.log 2>&1

或者crontab -e

```
# m h  dom mon dow   command
0 0 * * * apt-mirror
```

**发布镜像服务**

使用nginx发布源镜像

将 apt-mirror 的镜像目录链接到/var/www/mirror

```
# ln -sf /var/spool/apt-mirror/mirror/ftp.cn.debian.org/ mirror
```

然后将nginx默认主机default(或者单独虚拟主机)的根目录设置为/var/www/mirror,并开启目录列表

```
root /var/www/mirror
location / {
    autoindex on;
}
```

其他机器就可以正常使用新建的源镜像了。

 

 

另：

apt-mirror能够将官方镜像下载到本地，并保证目录结构与其一致，但是不能对镜像仓库进行修改。如果想要修改镜像仓库，需要使用reprepro。

1、安装apt-mirror

```php
# aptitude update
# aptitude install -y apt-mirror

# dpkg -L apt-mirror
/.
/usr
/usr/bin
/usr/bin/apt-mirror
/usr/share
/usr/share/man
/usr/share/man/man1
/usr/share/man/man1/apt-mirror.1.gz
/usr/share/doc
/usr/share/doc/apt-mirror
/usr/share/doc/apt-mirror/changelog.gz
/usr/share/doc/apt-mirror/changelog.Debian.gz
/usr/share/doc/apt-mirror/copyright
/etc
/etc/cron.d
/etc/cron.d/apt-mirror
/etc/apt
/etc/apt/mirror.list
/var
/var/spool
/var/spool/apt-mirror
/var/spool/apt-mirror/skel
/var/spool/apt-mirror/mirror
/var/spool/apt-mirror/var
```

2、修改配置文件，添加要下载的源（我这里添加的是debian_squeeze的源）

```bash
# vim /etc/apt/mirror.list

set base_path    /docker/debian_mirror
set mirror_path  $base_path/mirror
set skel_path    $base_path/skel
set var_path     $base_path/var
set cleanscript $var_path/clean.sh
set defaultarch  <running host architecture>
set postmirror_script $var_path/postmirror.sh
set run_postmirror 0
set nthreads     40
set _tilde 0

deb-amd64 http://archive.debian.org/debian squeeze main
deb-amd64 http://archive.debian.org/debian squeeze-lts main
```

3、手动运行apt-mirror下载镜像

```less
# apt-mirror
Downloading 28 index files using 28 threads...
Begin time: Mon Aug 21 11:24:11 2017
[28]... [27]... [26]... [25]... [24]... [23]... [22]... [21]... [20]... [19]... [18]... [17]... [16]... [15]... [14]... [13]... [12]... [11]... [10]... [9]... [8]... [7]... [6]... [5]... [4]... [3]... [2]... [1]... [0]...
End time: Mon Aug 21 11:24:18 2017

Processing tranlation indexes: [TT]

Downloading 32 translation files using 32 threads...
Begin time: Mon Aug 21 11:24:19 2017
[32]... [31]... [30]... [29]... [28]... [27]... [26]... [25]... [24]... [23]... [22]... [21]... [20]... [19]... [18]... [17]... [16]... [15]... [14]... [13]... [12]... [11]... [10]... [9]... [8]... [7]... [6]... [5]... [4]... [3]... [2]... [1]... [0]...
End time: Mon Aug 21 11:24:20 2017

Processing indexes: [PP]

32.8 GiB will be downloaded into archive.
Downloading 25138 archive files using 40 threads...
Begin time: Mon Aug 21 11:24:23 2017
[40]... [39]... [38]... [37]... [36]... [35]... [34]... [33]... [32]... [31]... [30]... [29]... [28]... [27]... [26]... [25]... [24]... [23]... [22]... [21]... [20]... [19]... [18]... [17]... [16]... [15]... [14]... [13]... [12]... [11]... [10]... [9]... [8]... [7]... [6]... [5]... [4]... [3]... [2]... [1]... [0]...
End time: Mon Aug 21 21:38:48 2017

0 bytes in 0 files and 0 directories can be freed.
Run /docker/debian_mirror/var/clean.sh for this purpose.
```

4、设置定时任务，定时与官方源同步

```markdown
# vim /etc/cron.d/apt-mirror
0 0 * * * apt-mirror > /docker/mirror.log 2>&1
```

5、设置发布目录软链接

```shell
# ln -sv /docker/debian_mirror/mirror/archive.debian.org /docker/mirror
```

6、配置nginx发布镜像目录

```lua
# cd /usr/local/nginx/conf/vhosts/
# vim debian_mirror.conf

server {
    listen 8081;
    server_name debian_mirror;

    root /docker/mirror
    location / {
        autoindex on;
    }

    access_log /usr/local/nginx/logs/debian_mirror.access.log oupeng_logs;
    error_log /usr/local/nginx/logs/debian_mirror.error.log;
}

# /usr/local/nginx/sbin/nginx -t
# /usr/local/nginx/sbin/nginx -s reload
```

7、找一台debian6的机器添加本地源测试

```yaml
# vim /etc/apt/sources.list
deb http://117.119.33.138:8081/debian squeeze main
deb http://117.119.33.138:8081/debian squeeze-lts main

# aptitude update
Hit http://117.119.33.138 squeeze Release.gpg
Ign http://117.119.33.138/debian/ squeeze/main Translation-en
Ign http://117.119.33.138/debian/ squeeze/main Translation-en_US
Get:1 http://117.119.33.138 squeeze-lts Release.gpg [819 B]
Ign http://117.119.33.138/debian/ squeeze-lts/main Translation-en
Ign http://117.119.33.138/debian/ squeeze-lts/main Translation-en_US
Hit http://117.119.33.138 squeeze Release
Get:2 http://117.119.33.138 squeeze-lts Release [34.3 kB]
Hit http://117.119.33.138 squeeze/main amd64 Packages
E: Release file expired, ignoring http://117.119.33.138:8081/debian/dists/squeeze-lts/Release (invalid since 524d 12h 54min 42s)
```

其中第二个源squeeze-lts会提示过期，在apt配置文件中添加一项配置即可

```yaml
# vim /etc/apt/apt.conf
Acquire::Check-Valid-Until false;

# aptitude update
Get:1 http://apt.oupeng.com squeeze Release.gpg [1,655 B]
Ign http://apt.oupeng.com/debian/ squeeze/main Translation-en
Ign http://apt.oupeng.com/debian/ squeeze/main Translation-en_US
Get:2 http://apt.oupeng.com squeeze-lts Release.gpg [819 B]
Ign http://apt.oupeng.com/debian/ squeeze-lts/main Translation-en
Ign http://apt.oupeng.com/debian/ squeeze-lts/main Translation-en_US
Get:3 http://apt.oupeng.com squeeze Release [96.0 kB]
Get:4 http://apt.oupeng.com squeeze-lts Release [34.3 kB]
Get:5 http://apt.oupeng.com squeeze/main amd64 Packages [6,527 kB]
Get:6 http://apt.oupeng.com squeeze-lts/main amd64 Packages [305 kB]
Hit http://apt.puppetlabs.com squeeze Release.gpg
Ign http://apt.puppetlabs.com/ squeeze/dependencies Translation-en
Ign http://apt.puppetlabs.com/ squeeze/dependencies Translation-en_US
Ign http://apt.puppetlabs.com/ squeeze/main Translation-en
Ign http://apt.puppetlabs.com/ squeeze/main Translation-en_US
Hit http://apt.puppetlabs.com squeeze Release
Ign http://apt.puppetlabs.com squeeze/main Sources
Ign http://apt.puppetlabs.com squeeze/dependencies Sources
Ign http://apt.puppetlabs.com squeeze/main amd64 Packages
Ign http://apt.puppetlabs.com squeeze/dependencies amd64 Packages
Hit http://apt.puppetlabs.com squeeze/main Sources
Hit http://apt.puppetlabs.com squeeze/dependencies Sources
Hit http://apt.puppetlabs.com squeeze/main amd64 Packages
Hit http://apt.puppetlabs.com squeeze/dependencies amd64 Packages
Fetched 6,965 kB in 2s (3,148 kB/s)

Current status: 43 updates [+41], 27994 new [+27654].
```

 

# 使用apt-mirror做一个本地ubuntu离线apt源

https://blog.csdn.net/yanjiee/article/details/85011779





另一种简易源的离线方法
其实ubuntu官网下载下来的iso里面是包含一些常用的软件源的，比如python和openssh等。我们也可以通过apt-cdrom这个方法，将一些常用软件的源离线使用，他肯定没有上面170G源完整，但是可以对付一些小场合。

先挂载iso
如果我们用iso制作完了U盘，那需要在U盘中再次拷贝一份这个iso，比如ubuntu.iso
使用命令将iso挂载到cdrom下：

sudo mount -o loop usb/ubunto.iso /media/cdrom
1
添加源
sudo apt-cdrom add -m
1
-m 表示不指定节点，使用默认的/media/cdrom作为节点
运行完可以看到在/etc/apt/sources.list下，多了一条cdrom的节点

更新
sudo apt-get update
————————————————
版权声明：本文为CSDN博主「yanjiee」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/yanjiee/article/details/85011779
