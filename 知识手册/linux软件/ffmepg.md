ffmpeg显示图片到Framebuffer
sudo ffmpeg -c:v png -i ./demo.png -pix_fmt rgb565le -f fbdev /dev/fb1



ffplay -i rtsp://10.254.239.1:8554/axstream0 -fflags nobuffer

ffmpeg命令行拉TCP的RTSP流的方法及使用测试记录——RTP over RTSP(TCP)
https://blog.csdn.net/xuw_xy/article/details/116153413


TCP推流命令如下：

ffmpeg -re -i a.mp4 -c copy -f flv tcp://127.0.0.1:1234/live/stream

ffplay -listen 1 -i tcp://127.0.0.1:1234/live/stream -fflags nobuffer

ffmpeg.exe -listen 1 -i tcp://127.0.0.1:1234/live/stream -c copy -f flv output.flv -y

https://blog.csdn.net/u012117034/article/details/123470123




1--FFplay播放控制

        可通过播放一个视频进行测试：

ffplay test1.mp4

1 退出播放：q键、ESC键

2 全屏切换：f键（进入或退出全屏）

3 暂停：p键、空格键

4 静音切换：m键

5 改变音量：9键减少音量、0键增加音量

6 循环切换音频流：a键

7 循环切换视频流：v键

8 循环切换字幕流：t键

9 循环切换节目：c键

10 循环切换过滤器或显示模式：w键

11 逐帧播放：s键

12 快进后退10秒：left键（←)后退10秒，right键（→）前进10秒；

13 快进后退60秒：down键（↓)后退60秒，up键（↑）前进60秒；
2--FFplay命令参数
1 强制显示宽高，用于调整窗口大小

    -x width
    -y height

2 设置帧尺寸，仅适用于没有包含帧大小的视频

-video_size size

3 设置像素格式

-pixel_format format

4 以全屏模式启动

-fs

5 禁用音频、视频和字幕

    -an # 禁用音频
    -vn # 禁用视频
    -sn # 禁用字幕

6 设置开始播放的位置

    -ss pos
    # pos可以是数字，比如-ss 100表示从100秒的位置开始播放
    # pos也可以是hour:min:second的格式，比如-ss 00:01:50表示从1分50秒的位置开始播放

7 设置播放视频的长度，即持续时间

    -t duration
    # duration的格式与上面的pos相同

8 设置按字节进行定位拖动

-bytes

9 自定义左右键拖动间隔，以秒为基本单位，默认为10秒

-seek_interval interval

10 关闭图形化显示窗口，不显示视频

-nodisp

11 设置无边框窗口

-noborder

12 设置起始音量

-volume vol

13 强制使用设置的格式进行解析

-f fmt

14 设置窗口标题，默认为输入文件名

-window_title title

15 设置播放循环次数

-loop number

16 设置显示模式，默认为0显示视频

    -showmode mode
    # 0显示视频
    # 1显示音频波形
    # 2显示音频频谱

17 设置视频滤镜和音频滤镜

    -vf filtergraph # 设置视频滤镜
    -af filtergraph # 设置音频滤镜

3--FFplay高级命令参数
1 打印多个回放统计信息

    -stats
    # 统计信息包括现实流持续时间、编解码参数、流的当前位置等

2 设置同步类型

    -sync type
    # type分为audio、video和external等类型

3 指定音频流、视频流和字幕流索引

    -ast index # 播放流索引为index的音频流
     
    -vst index # 播放流索引为index的视频流
     
    -sst index # 播放流索引为index的字幕流

4 设置视频播放完毕自动退出

-autoexit

5 强制使用设置的多媒体解码器

    -codec:media_specifier codec_name
    # -codec:v h264_qsv表示强制视频采用h264_qsv进行解码

6 强制使用解码器进行音频解码、视频解码和字幕解码

    -acodec codec_name #强制使用设置的音频解码器进行音频解码
     
    -vcodec codec_name #强制使用设置的视频解码器进行视频解码
     
    -scodec codec_name #强制使用设置的字幕解码器进行字幕解码

7 不限制输入缓冲区大小

    -infbuf
    # 尽可能快地从输入中读取尽可能多的数据，禁用时需采用-noinfbuf
————————————————
版权声明：本文为CSDN博主「憨豆的小泰迪」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_43863869/article/details/128461745



报错解决：
```

ffmpeg -i r1ori.mp4  filter.mp4

[libx264 @ 0x55e8d0373600] height not divisible by 2 (1510x1017)
Error initializing output stream 0:0 -- Error while opening encoder for output stream #0:0 - maybe incorrect parameters such as bit_rate, rate, width or height
Conversion failed!

ffmpeg -i blue.webm -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" blue.mp4
显卡加速转换：ffmpeg -hwaccel cuvid -i input.flv -f mp4 -c:v hevc_nvenc -c:a copy output.mp4
不过感觉没什么加速的效果。
```

ffmpeg -re -stream_loop -1 -i file.ts -c copy -f rtsp -rtsp_transport tcp rtsp://localhost:8554/mystream


ffmpeg查看图像颜色空间   ffmpeg -pix_fmts

1. 单图像jpg转yuv

倘若当前目录下有个jpg文件，名为000001_353x500.jpg，可用如下语句将其转换为yuv格式，并将新yuv文件命名为000001_353x500.yuv.

ffmpeg -pix_fmt yuv420p -s 353x500 -i 000001_353x500.jpg 000001_353x500.yuv

这里，-pix_fmt表示pixel format，指定像素格式，这里指定为yuv420p格式；
-s表示size，后以宽x高格式指定图像的宽高大小；
-i表示input，后接输入文件的url；

最后，指定output输出文件的url.


2. 单图像yuv转jpg

倘若当前目录下有个yuv文件，名为000001_353x500.yuv，可用如下命令将其转换为jpg格式，并将新jpg文件命名为000001_353x500.jpg.

ffmpeg  -s 353x500 -i 000001.yuv 000001.jpg -y

这里，-s表示size，后接了输入yuv文件的图像大小；
-i表示input文件的url；
然后接output输出文件的url；
最后，-y是一个通用选项，表示当前目录下倘若存在相同输出文件名，则直接替代它。

如果不加-y，遇到输出url已存在一个相同输出文件名文件，命令行会中断当前程序，询问是否覆盖文件：

File '000001_353x500.yuv' already exists. Overwrite? [y/N]

y覆盖，N不覆盖。


ffmpeg -listen 1 -i tcp://0.0.0.0:10000 -c copy -f rtsp rtsp://192.168.2.21:8554/nihao