ALSA中aplay -Dhw:0,0 和 aplay -Dplughw:0,0 的区别



aplay -Dhw:0,0 -c 2 -r 48000 -f S16_LE  /usr/share/sounds/alsa/Front_Left.wav


aplay -Dplughw:0,0 -c 2 -r 48000 -f S16_LE  /usr/share/sounds/alsa/Front_Left.wav  可以正常播放.

其中的区别就是hw 和 plughw , 参考 Difference between “hwplug” and “hw”得出结论

hw:   accesses the hardware device directly.  // 直接访问硬件

plughw:  inserts sample rate and format conversion plugins, if needed. //经过采样率和格式转换插件。

可以通过mediainfo查看文件信息, 发现为1通道格式，而Codec driver 是需要播放的2 channel的,所以需要plug转换为2 channel才可以播放.

特意找了一个2 channel / 48kHz , 验证了猜想结果． 不加plug可以正常播放


在 arch 中没有 aplay 程序，可以使用 `sudo pacman -S alsa-utils` 进行安装。

