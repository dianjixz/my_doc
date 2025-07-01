获取raspberry linux 源码：https://github.com/RPi-Distro/rpi-source




Make arm64 deb packages for the offical Raspberry Pi Foundation arm64 kernels, tested with ubuntu 23.04 
https://gist.github.com/satmandu/a507c59d84737f6d29ff353395819d51


树莓派学习笔记——获取树莓派CPU温度
https://blog.csdn.net/xukai871105/article/details/38349209
无需安装工具即可查看：

 cat /sys/class/thermal/thermal_zone0/temp
 >>> 62838
 
 # 或者直接以度为单位显示
 echo $[$(cat /sys/class/thermal/thermal_zone0/temp)/1000]°
 >>> 63

显示数字为千分之一度。所以说，除以1000就是当前温度值。

可以设置watch实时观看：

 watch -n 0.1 echo CPU: $[$(cat /sys/class/thermal/thermal_zone0/temp)/1000]°




 树莓派共享 WiFi 在线配置工具
 https://tech.biko.pub/tool#/rpi-share-wifi

 ``` bash
 #复制如下代码到树莓派终端执行：

wget -qO- https://tech.biko.pub/resource/rpi-share-wifi-setup.sh | \
sudo R_SSID="dian-qi-guan-tou" \
 R_PWD="12345678" \
 R_HOST_IP="192.168.2.1" \
 R_MASK="255.255.255.0" \
 R_CLIENT_IP_RANGE="192.168.2.10,192.168.2.20" \
 bash

#执行成功后，搜索 WiFi 名称 dian-qi-guan-tou 连接即可上网。
```


``` bash
# 停止共享 WiFi
wget -qO- https://tech.biko.pub/resource/rpi-share-wifi-stop.sh | sudo bash

```


``` bash
# 将系统事件写入硬件时钟
sudo hwclock -w 

#查看硬件时钟时间
sudo hwclock -r


```

树梅派设置静态 ip


树梅派自动扩容：
``` bash
if [ -f "/etc/resize.init" ] ; then
	rm /etc/resize.init
	raspi-config nonint do_expand_rootfs
	sync
	reboot
fi



```


可以在 webmin 上添加一个探针组件，让 webmin 全方位拥有服务器监控控制能力。
# webmin 在线管理树莓派系统

webmin 是一个 web 的图形化管理系统，可以管理包括系统、网络、服务、防火墙、用户、软件包、文件共享等。webmin 是开源的，可以免费使用。

webmin 的安装方法如下：

1. 在树莓派上安装 webmin，可以使用 apt-get 命令：

```bash
sudo apt-get update
sudo apt-get install webmin
```
2. 安装完成后，webmin 的默认端口是 10000，可以在浏览器中输入树莓派的 IP 地址和端口号（例如：http://192.168.1.100:10000）来访问 webmin 的登录页面。
3. 在登录页面中，输入用户名和密码，用户名默认是 root，密码是树莓派的密码。
4. 登录成功后，就可以使用 webmin 来管理树莓派了。例如，可以查看系统信息、管理网络、管理服务、管理防火墙、管理用户、管理软件包、管理文件共享等。
















