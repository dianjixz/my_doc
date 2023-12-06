ubuntu 双系统和 windows 系统时间保持同步的命令。
``` bash
timedatectl set-local-rtc 1 --adjust-system-clock

# 网络时间同步
sudo apt-get install ntpdate

sudo ntpdate cn.pool.ntp.org
```



Ubuntu 离线安装gcc,g++,make依赖包

版权声明：本文为CSDN博主「崔二旦」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/cuierdan/article/details/131192779


下载依赖包

``` bash
# 把缓存文件夹里的deb包清理掉
 sudo apt-get clean
# 下载 build-essential包，因为build-essential包中有gcc,g++,make等依赖包。
# 下面命令是只下载不安装。
 sudo apt-get install -d build-essential
# 进入缓存文件夹查询已下载的deb包。
 cd /var/cache/apt/archives

```

安装依赖包
``` bash
sudo dpkg -i   *.deb
```


ubuntu双系统设置启动顺序
```bash
vim /etc/default/grub

update-grub

```



ubuntu 文件管理器
nautilus



通过使用默认文件管理器，以下内容适用于所有桌面环境：
xdg-open file