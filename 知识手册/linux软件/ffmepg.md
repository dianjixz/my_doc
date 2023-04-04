ffmpeg显示图片到Framebuffer
sudo ffmpeg -c:v png -i ./demo.png -pix_fmt rgb565le -f fbdev /dev/fb1



ffplay -i rtsp://10.254.239.1:8554/axstream0 -fflags nobuffer

ffmpeg命令行拉TCP的RTSP流的方法及使用测试记录——RTP over RTSP(TCP)
https://blog.csdn.net/xuw_xy/article/details/116153413