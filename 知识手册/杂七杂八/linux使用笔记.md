2023年 07月 05日 星期三 17:28:29 CST
今天巡查服务器的时候突然发现服务器的显卡挂掉了，
Failed to initialize NVML: Driver/library version mismatch
这个文章中有解决问题的方法，同时对显卡驱动进行了版本停留，关闭了自动更新。
https://blog.csdn.net/weixin_43568307/article/details/128187469

为了服务器软件的版本稳定，遂关掉 unattended-upgrade 自动更新
``` bash
sudo dpkg-reconfigure unattended-upgrades
# 在弹出的界面中选择 no
```


解决方案：

总体的思路分为两步：

    退出当前内核使用的显卡模块
    重新加载升级后版本的显卡驱动作为我们的内核模块

指令
``` bash
sudo rmmod nvidia
sudo nvidia-smi   #nvidia-smi发现没有kernel mod的时候，会自动装载
```
如果用户正在使用到nvidia的内核，则执行sudo rmmod nvidia指令的时候，会提示如下错误

``` bash
$ sudo rmmod nvidia
rmmod: ERROR: Module nvidia is in use by: nvidia_modeset nvidia_uvm
```
查看nvidia模块使用情况

``` bash
$lsmod | grep nvidia
nvidia_uvm            647168  0
nvidia_drm             53248  0
nvidia_modeset        790528  1 nvidia_drm
nvidia              12144640  152 nvidia_modeset,nvidia_uvm            12144640  152 nvidia_modeset,nvidia_uvm
```
这就需要我们来手动地关闭nvidia_modset和nvidia_uvm服务。
查看所有的nvidia相关的进程
``` bash
sudo lsof -n -w  /dev/nvidia*
```
然后，我们通过kill -9 PID 终止掉和nvidia_uvm,nvidia_modeset有关的进程。然后再重新执行如下的指令
``` bash
sudo rmmod nvidia
sudo nvidia-smi   #nvidia-smi发现没有kernel mod的时候，会自动装载
```
最后，正常情况下问题就解决了。
如果还没有解决，通过如下指令终止nvidia的相关服务
``` bash
sudo rmmod nvidia_uvm
sudo rmmod nvidia_drm
sudo rmmod nvidia_modeset
sudo rmmod nvidia
```
再执行
``` bash
sudo rmmod nvidia
sudo nvidia-smi   #nvidia-smi发现没有kernel mod的时候，会自动装载
```
此方法对服务器比较有用，个人电脑的程序占用比较多，如果真的想用只能在注销状态下更换显卡驱动。最简单的方法就是直接重启。
暂停后面的更新。

``` bash
sudo dpkg --list | grep nvidia-*
sudo apt-mark hold nvidia-dkms-525
sudo dpkg-reconfigure unattended-upgrades
```

怎么禁止/开启Ubuntu自动更新升级


1、打开终端，修改配置文件1。输入：
``` bash
sudo gedit /etc/apt/apt.conf.d/10periodic
```
如果要禁止自动更新配置文件设为如下：
``` bash
APT::Periodic::Update-Package-Lists "0";
APT::Periodic::Download-Upgradeable-Packages "0";
APT::Periodic::AutocleanInterval "0";
APT::Periodic::Unattended-Upgrade "0";
```


如果要打开自动更新配置文件设为如下：

``` bash
APT::Periodic::Update-Package-Lists "2";
APT::Periodic::Download-Upgradeable-Packages "1";
APT::Periodic::AutocleanInterval "0";
APT::Periodic::Unattended-Upgrade "1";
```

保存退出。

2、修改配置文件2，输入：
``` bash
sudo gedit /etc/apt/apt.conf.d/20auto-upgrades
```
如果要禁止自动更新配置文件设为如下：

``` bash
APT::Periodic::Update-Package-Lists "0";
APT::Periodic::Download-Upgradeable-Packages "0";
APT::Periodic::AutocleanInterval "0";
APT::Periodic::Unattended-Upgrade "0";
```

如果要打开自动更新配置文件设为如下：

``` bash
APT::Periodic::Update-Package-Lists "2";
APT::Periodic::Download-Upgradeable-Packages "1";
APT::Periodic::AutocleanInterval "0";
APT::Periodic::Unattended-Upgrade "1";
```

保存退出。

3、两个文件都要修改，直接修改配置文件可能需要重启。
输入：

``` bash
reboot
```







