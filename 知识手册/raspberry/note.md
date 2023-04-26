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