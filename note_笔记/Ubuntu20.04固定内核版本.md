由于安装完NVIDIA GPU驱动后，有内核版本更新重启后会导致无法使用nvidia-smi等问题，这里讲解一共三种方式固定内核版本。  
## 1.修改grub配置文件固定内核版本
``` bash
uname -r
#复制内核版本信息
5.4.0-88-generic
#编辑内核配置文件
vim /etc/default/grub
#修改本行，格式照抄，最后版本修改为自己的即可，修改完保存退出。
GRUB_DEFAULT="Advanced options for Ubuntu>Ubuntu, with Linux 5.4.0-88-generic"

#更新grub
(env) root@aicloud:/boot# update-grub
Sourcing file `/etc/default/grub'
Sourcing file `/etc/default/grub.d/init-select.cfg'
Generating grub configuration file ...
Found linux image: /boot/vmlinuz-5.4.0-89-generic
Found initrd image: /boot/initrd.img-5.4.0-89-generic
Found linux image: /boot/vmlinuz-5.4.0-88-generic
Found initrd image: /boot/initrd.img-5.4.0-88-generic
Adding boot menu entry for UEFI Firmware Settings
done
```


## 2.apt-mark固定内核  
``` bash
apt-mark hold 5.4.0-88-generic
```


## 3.禁止apt自动更新
``` bash
vim /etc/apt/apt.conf.d/10periodic
# APT::Periodic::Update-Package-Lists "0";   #这里的1改成0
# APT::Periodic::Download-Upgradeable-Packages "0";
# APT::Periodic::AutocleanInterval "0";
# APT::Periodic::Unattended-Upgrade "0"; # 每天的自动更新
# 重新加载配置
sudo systemctl restart apt-daily.timer apt-daily-upgrade.timer
```
————————————————
版权声明：本文为CSDN博主「technetfly」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/checknet/article/details/122257816