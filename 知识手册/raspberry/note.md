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