lalserver是纯Golang开发的流媒体（直播音视频网络传输）服务器。目前已支持RTMP, RTSP(RTP/RTCP), HLS, HTTP[S]/WebSocket-FLV/TS, GB28181协议。并支持通过插件形式进行二次开发扩展。

https://pengrl.com/lal/#/


✒ lalserver 特性

    全平台
        支持linux/macOS/windows多系统开发、调试、运行
        支持多arch运行。比如amd64/arm64/arm32/ppc64le/mipsle/s390x
        支持交叉编译。可在任一平台编译出其他平台的可执行文件
        运行无依赖。可执行文件无任何环境、库安装依赖，可单文件独立运行
        提供各平台可执行文件，可免编译直接运行(前提是开放全部源码哈)
        支持docker。支持amd和arm架构
    高性能。多核多线程扩展，支持高并发，性能和同类型c/c++开发服务处于同一水平
    高可用。完善的单元测试。并且有多个线上环境应用
    多种直播流封装协议
        支持RTMP, RTSP(RTP/RTCP), HLS, HTTP[S]/WebSocket-FLV/TS, GB28181
        支持不同封装协议间相互转换
        支持通过插件形式进行二次开发，扩展其他协议
    多种编码格式。视频支持H264/AVC，H265/HEVC，音频支持AAC，G711A/U
    多种格式录制。支持FLV，长MPEGTS，HLS录制(HLS直播与录制可同时开启)
    HTTPS。支持HTTPS-FLV，HTTPS-TS，HLS over HTTPS拉流
    WebSocket。支持Websocket-FLV，WebSocket-TS拉流
    TLS/SSL。所有协议都支持相应的TLS/SSL加密传输。比如RTMPS，RTSPS，HTTPS，WebSockets
    HLS。支持LIVE实时直播、VOD全列表直播。切片文件支持多种删除方式。支持内存切片。支持统计HLS播放者数量
    RTSP。支持over TCP(interleaved模式)。支持basic/digest auth验证。
    RTMP。支持给单视频添加静音音频数据，支持合并发送。兼容对接各种常见RTMP实现
    RTP。支持解析扩展头
    HTTP API接口。用于获取服务信息，向服务发送命令。
    HTTP Notify事件回调。获取时间通知，业务方可以轻松定制自身的逻辑。
    支持多种方式鉴权
    支持分布式集群
    静态pull回源。通过配置文件配置回源地址
    静态push转推。支持转推多个地址。通过配置文件配置转推地址
    支持按需回源。没有观众的回源拉流可以主动关闭
    CORS跨域。支持HTTP-FLV，HTTP-TS，HLS跨域拉流
    HTTP文件服务器。比如HLS切片文件可直接播放，不需要额外的HTTP文件服务器
    监听端口复用。HTTP-FLV，HTTP-TS，HLS可使用相同的端口。over HTTPS类似
    秒开播放。GOP缓冲
    支持(在服务端)自动叠加静音音频
    支持先拉流后推流
    支持推流断开后重连，拉流端无感知。
    支持自定义debug格式流录制、流回放，快速复现、定位问题
    自带Web UI页面


下载镜像：
``` bash
docker pull q191201771/lal
```
运行镜像：
``` bash
docker run -it -p 1935:1935 -p 8080:8080 -p 4433:4433 -p 5544:5544 -p 8083:8083 -p 8084:8084 -p 30000-30100:30000-30100/udp q191201771/lal /lal/bin/lalserver -c /lal/conf/lalserver.conf.json
```
如果想构建自己的Docker镜像，可参考 附录-构建自己的Docker镜像


协议 	url地址 	协议标准端口


RTMP推流 	rtmp://127.0.0.1:1935/live/test110 	1935


RTSP推流 	rtsp://localhost:5544/live/test110 	554

RTMP拉流 	rtmp://127.0.0.1:1935/live/test110 	1935

HTTP-FLV拉流 	http://127.0.0.1:8080/live/test110.flv 80 

HTTP-FLV拉流    https://127.0.0.1:4433/live/test110.flv (https地址)  443

WebSocket-FLV拉流 ws://127.0.0.1:8080/live/test110.flv                                  80 
WebSocket-FLV拉流 wss://127.0.0.1:4433/live/test110.flv (websockets地址)                443

HLS(m3u8+ts)拉流 http://127.0.0.1:8080/hls/test110.m3u8 (直播地址格式1)                     80
HLS(m3u8+ts)拉流 http://127.0.0.1:8080/hls/test110/playlist.m3u8 (直播地址格式2)            80
HLS(m3u8+ts)拉流 http://127.0.0.1:8080/hls/test110/record.m3u8 (全量录播地址)               80

RTSP拉流 rtsp://localhost:5544/live/test110 554

HTTP-TS拉流 http://127.0.0.1:8080/live/test110.ts (http地址)                                80 
HTTP-TS拉流 https://127.0.0.1:4433/live/test110.ts (https地址)                              443 
HTTP-TS拉流 ws://127.0.0.1:8080/live/test110.ts (websocket地址)                             80 
HTTP-TS拉流 wss://127.0.0.1:4433/live/test110.ts (websockets地址)                           443






协议支持列表
推流客户端 	RTMP 	RTSP
ffmpeg 	✔ 	✔
obs 	✔ 	X
拉流客户端 	RTMP 	RTSP 	HTTP-FLV 	HTTPS-FLV 	HLS 	HTTP-TS 	WebSocket-FLV 	WebSocket-TS 	备注
ffmpeg/ffplay 	✔ 	✔ 	✔ 	✔ 	✔ 	✔ 	X 	X 	
vlc 	✔ 	✔ 	✔ 	X 	✔ 	✔ 	X 	X 	
MPV 	✔ 	✔ 	✔ 	✔ 	✔ 	✔ 	X 	X 	
flv.js 	X 	X 	✔ 	? 	X 	X 	✔ 	X 	
hls.js 	X 	X 	X 	X 	✔ 	X 	X 	X 	
西瓜播放器(js) 	X 	X 	✔ 	? 	✔ 	X 	✔ 	X 	
MPlayer 	? 	? 	? 	? 	? 	? 	? 	? 	
部分客户端使用说明

TODO(chef): 这部分内容需要整理

    1 rtmp推流
        1.1 obs
            1.1.1 obs采集摄像头
        1.2 ffmpeg
            1.2.1 ffmpeg读取文件推送
    2 rtsp推流
        2.1 ffmpeg
            2.1.1 ffmpeg读取文件推流
    3 rtmp拉流
        3.1 vlc
        3.2 ffmpeg
            3.2.1 ffmpeg拉流保存文件
        3.3 ffplay
        3.4 mpv
    4 http-flv和https-flv拉流
    5 HLS(m3u8+ts)拉流
        5.1 safari浏览器
        5.2 vlc，ffmpeg，ffplay
    6 http-ts拉流
    7 rtsp拉流
    8 测试文件下载

(后续会补充其他客户端的使用方式，以及其他的流媒体协议格式。)
1 rtmp推流
1.1 obs

上官网（ https://obsproject.com/ ）下载对应操作系统的二进制安装包，并安装。

本文以macos系统的obs 25.0.8版本做演示，其他系统和obs版本都差不多。
1.1.1 obs采集摄像头

    打开obs，点击右下角的设置按钮
    在弹出的窗口中，点击左侧的推流按钮
    右侧出现推流详情页：
        服务下拉框不用管，保持自定义...
        服务器输入框中填写 rtmp://127.0.0.1:1935/live
        串流密钥输入框中填写 test110
            tips，串流密钥默认是不明文显示输入的字符，如果怕输出，可以点击右侧的显示按钮
    点击右下角的确认按钮，完成设置
    回到主界面，点击右下角开始推流按钮
    底部的状态栏显示LIVE，fps，kb/s等信息，分别表示推流时长，帧率，码率，说明推流成功

obs除了能采集摄像头作为输入流，还提供采集桌面，音视频文件等功能。而且提供了很多参数都可以在图形化界面上配置。
1.2 ffmpeg
1.2.1 ffmpeg读取文件推送

（如何安装ffmpeg见文末）

这里分两种情况，一种是flv、mp4文件中的音频本身就是AAC格式，视频本身就是H264或H265格式，流媒体服务器本身就支持，那么ffmpeg不需要对音视频重新编码：

$ffmpeg -re -stream_loop -1 -i demo.flv -c:a copy -c:v copy -f flv rtmp://127.0.0.1:1935/live/test110

对其中的参数做些说明：

    -re表示按文件中音视频流的码率推送，如果不加，就是不控制发送速度，一次性发送给服务端了，不符合直播的特点
    -stream_loop -1表示文件结束后，继续从文件头部循环推送的次数，-1表示无限循环
    -i表示输入文件
    -c:a copy表示音频编码格式不变
    -c:v copy表示视频编码格式不变
    -f flv推送rtmp流需要指定格式为flv
    最后是rtmp推流地址

如果是mp4文件，将demo.flv换成mp4文件名即可，比如demo.mp4

以上是比较常见的一种情况。

另一种情况，是文件中的音视频编码格式，流媒体服务器不支持，那么ffmpeg需要重新编码：

$ffmpeg -re -i demo.flv -c:a aac -c:v h264 -f flv rtmp://127.0.0.1:1935/live/test110

其中，-c:a aac表示音频用aac编码，-c:v h264表示视频用h264编码。
2 rtsp推流
2.2 ffmpeg
2.2.1 ffmpeg读取文件推送

ffmpeg -re -stream_loop -1 -i demo.flv -acodec copy -vcodec copy -f rtsp rtsp://localhost:5544/live/test110

另外，rtsp还支持rtp over tcp的方式推流，对应的ffmpeg命令如下：

ffmpeg -re -stream_loop -1 -i demo.flv -acodec copy -vcodec copy -rtsp_transport tcp -f rtsp rtsp://localhost:5544/live/test110

参数含义参见 1.2.1
3 rtmp拉流
3.1 vlc

上官网（ https://www.videolan.org/vlc/ ）下载对应操作系统的二进制安装包，并安装。

本文以macos系统的vlc 3.0.8版本做演示，其他系统和obs版本都差不多。

    打开vlc
    点击底部菜单栏中的File，再点击Open Network...
    在弹出的窗口中的URL输入框中填入 rtmp://127.0.0.1:1935/live/test110
    点击右下角的确认按钮，完成设置
    开始播放直播流

3.2 ffmpeg
3.2.1 ffmpeg拉流保存文件

如何安装ffmpeg见文末

$ffmpeg -i rtmp://127.0.0.1:1935/live/test110 -c copy test110.flv

3.3 ffplay

ffplay是ffmpeg项目中带的一个播放器

$ffplay rtmp://127.0.0.1:1935/live/test110

3.4 mpv

上官下载二进制安装包，安装，然后输入拉流地址播放，基本和vlc差不多，参考 2.1 vlc
4 http-flv和https-flv拉流

http-flv拉流，对于vlc，ffmpeg，ffplay，mpv来说，和拉rtmp流是一样的，把拉流URL从
rtmp://127.0.0.1:1935/live/test110
改成
http://127.0.0.1:8080/live/test110.flv
即可。

https-flv拉流方式和http-flv相同。
5 HLS(m3u8+ts)拉流
5.1 safari浏览器

打开safari浏览器，在地址行输入HLS拉流地址
http://127.0.0.1:8080/hls/test110/playlist.m3u8
即可。
5.2 vlc，ffmpeg，ffplay

HLS(m3u8+ts)拉流，对于vlc，ffmpeg，ffplay来说，和拉rtmp流是一样的，把拉流URL从
rtmp://127.0.0.1:1935/live/test110
改成
http://127.0.0.1:8080/hls/test110/playlist.m3u8
即可。
6 http-ts拉流

http-ts拉流，对于vlc，ffmpeg，ffplay，mpv来说，和拉rtmp流是一样的，把拉流URL从
rtmp://127.0.0.1:1935/live/test110
改成
http://127.0.0.1:8080/live/test110.ts
即可。
7 rtsp拉流

$ffplay rtsp://localhost:5544/live/test110
$ffmpeg -i rtsp://localhost:5544/live/test110 -vcodec copy -acodec copy -f flv /tmp/test110.flv
$ffmpeg -rtsp_transport tcp -i rtsp://localhost:5544/live/test110 -vcodec copy -acodec copy -f flv /tmp/test110.flv

8 测试文件

测试文件下载地址：

https://github.com/q191201771/doc

测试文件转换生成命令：

$ffmpeg -i wontcry.mp4 -acodec aac -vcodec h264 -r 15 -g 30 -keyint_min 30 -bf 0 -f flv wontcry.flv
# -r 15                 fps帧率，表示1秒编码15帧
# -g 30                 GOP大小，也即I帧间隔，每30帧1个I帧，也即2秒1个I帧
# -keyint_min 30        最小GOP大小
# -bf 0                 不使用B帧编码
# -acodec aac           输出文件音频编码使用aac
# -vcodec h264          输出文件视频编码使用h264
# -i wontcry.mp4        输入文件
# -f flv wontcryflv.flv 输出flv文件


$ffmpeg -i wontcry.flv -acodec copy -vcodec copy -t 30 -f flv wontcry30s.flv
# -t 30 截取前30秒内容

$ffmpeg -i wontcry30s.flv -acodec aac -vcodec hevc -r 15 -g 30 -keyint_min 30 -bf 0 -preset ultrafast  -x265-params "bframes=0" -f flv hevc.flv
# 转码成不包含B帧的265

ffmpeg安装

    linux安装ffmpeg
    macOS编译金山云ksvc ffmpeg，在rtmp flv之上支持hevc h265
    macOS编译ffmpeg中的播放器ffplay
    macOS Catalina 10.15.1环境下编译的ffmpeg无法运行，启动就崩溃

原创不易，转载请注明文章出自开源流媒体服务器lal，Github：https://github.com/q191201771/lal 官方文档：https://pengrl.com/lal

yoko, 20210206