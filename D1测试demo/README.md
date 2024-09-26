# D1测试说明

## HDMI功能测试  

测试展现方式：通过HDMI播放高清视频。  

测试方法：  

第一步：通过HDMI数据线连接D1板和显示屏幕  


第二步：输入以下命令，开启HDMI功能！  

~~~ sh

#!/bin/sh
cd /sys/kernel/debug/dispdbg ;								#进入显示模块目录
echo disp0 > name; 											#输入屏幕号
echo switch1 > command; 									#输入选择器
echo 4 10 0 0 0x4 0x101 0 0 0 8 > param; 					#输入显示参数
echo 1 > start;												#打开显示器

#echo 1 > /sys/class/disp/disp/attr/colorbar				#打开颜色测试
#echo 0 > /sys/class/disp/disp/attr/colorbar				#关闭颜色测试

exit 0
~~~

第三步：输入以下程序运行播放程序

~~~ bash
#!/bin/sh  
tplayerdemo /usr/lib/tt-data/01-1080P-HEVC-AAC-60F.mkv

exit 0
~~~

## 以太网测试
第一步，插上合适的网线  
第二步，运行下面命令  
~~~ sh
#!/bin/sh
/usr/bin/ethdhcp.sh  #开启以太网并获取dchp分配的地址

exit 0
~~~

> note: 使用 `ifconfig` 命令可以查询ip地址

## WIFI测试
在/bin目录下有wifi测试的一系列指令，可以使用这些指令进行测试！

~~~ sh
#!/bin/sh
#wifi_connect_ap_test			#连接wifi   
#wifi_disconnect_ap_test		#断开wifi
#wifi_reconnect_ap_test			#重联wifi
#wifi_scan_results_test			#扫描wifi

#.....
~~~

## 摄像头测试
第一步：插上符合UVC标准的免驱摄像头  

第二步：使用程序测试
> paizhao   #这个程序需要用adb命令复制到开发板内。运行后可以从摄像头中抓取一张图片。  
> 
> mjpg_streamer -i "input_uvc.so -d /dev/video0" -o "output_http.so -w /www/webcam"  
> #这个命令是不算采集摄像头图片上传到网页中，我们可以通过访问http://board_ip:8080来实时观看摄像头。  

## lcd显示屏测试
第一步：确认屏幕连接好  

设定要显示的屏幕设备：
export FRAMEBUFFER=/dev/fb1

第二步：运行如下命令，查看屏幕是否输出随机亮点。

> test_lcd  

第三步：运行如下命令在lcd显示屏上显示一张图片。

> fbviewer test.jpg


fbviewer 源码：https://github.com/godspeed1989/fbv.git
```bash
gcc -o fbviewer main.o jpeg.o png.o bmp.o fb_display.o transforms.o /usr/lib/aarch64-linux-gnu/libjpeg.so /usr/lib/aarch64-linux-gnu/libpng.so

mkfifo fbvpipe
fbv /usr/local/m5stack/logo.jpg < fbvpipe 2>&1 > /dev/null &
fbvpid="$!"
while true ; do printf "\r\n" > fbvpipe; sleep 1; kill -0 $fbvpid 2>/dev/null || { rm fbvpipe; break; } ;done &
```


使用 ffmpeg 将 MP4 视频转换为 PPM 图像序列,然后使用fbv播放。
``` bash
ffmpeg -i input.mp4 -vf "format=rgb24" -f image2pipe -vcodec ppm - | fbv -d /dev/fb0 -n -
```



test.jpg可由paizhao程序生成。
## debian测试，目前debian系统属于外挂，待内置后再提供测试


磁盘读写测试
测试工具： hdparm
测试 /dev/sda2
``` bash
hdparm -t --direct /dev/sda2
```


cpu压力测试：
测试工具: stress-ng

stress-ng --cpu 2 --io 1 --vm 1 --vm-bytes 128M --timeout 10000h


ncnn 测试
测试工具:benchncnn
benchncnn 4 1 0 0 -1







