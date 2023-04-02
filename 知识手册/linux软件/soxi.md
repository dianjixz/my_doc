Linux下查看wav文件的头信息 | sox安装 和 基本操作——简记
墨理学AI 发表于 2022/01/13 22:57:33
776 0 0
【摘要】 sox官网，翻译即可 安装sox sudo apt-get install sox 1 有时，直接使用该命令，安装失败。大概率是 apt-get 默认软件下载源的问题，更换apt-get数据源之后再...

sox官网，翻译即可

    安装sox

sudo apt-get install sox

  
 

有时，直接使用该命令，安装失败。大概率是 apt-get 默认软件下载源的问题，更换apt-get数据源之后再安装即可（ubuntu16.04 apt-get 切换阿里云源）。

    查看音频信息

soxi sa1.wav

Input File     : 'sa1.wav'
Channels       : 1
Sample Rate    : 16000
Precision      : 16-bit
Duration       : 00:00:06.46 = 103424 samples ~ 484.8 CDDA sectors
File Size      : 207k
Bit Rate       : 256k
Sample Encoding: 16-bit Signed Integer PCM
  
 

查看当前目录下所有wav文件的头部信息

sox -V *.wav -n

  
 

    看wav时长

sox sa1.wav -n stat

Samples read:            103424
Length (seconds):      6.464000
Scaled by:         2147483647.0
Maximum amplitude:     0.054565
Minimum amplitude:    -0.050568
Midline amplitude:     0.001999
Mean    norm:          0.003897
Mean    amplitude:     0.000174
RMS     amplitude:     0.005530
Maximum delta:         0.054108
Minimum delta:         0.000000
Mean    delta:         0.000981
RMS     delta:         0.002194
Rough   frequency:         1010
Volume adjustment:       18.327
  
 

可见时长为6.464s

    截取3s音频

sox sa1.wav 3seconds.wav trim 1 3 

  
 

截取时间开始点为 1 s处， 截取长度 为 3s

    连接两个wav

sox sa1.wav  sa2.wav connection.wav 

  
 

将sa1.wav , sa2.wav按照次序连接在一起,输出文件是connection.wav

    混合重叠两个wav

sox -m sa1.wav sa2.wav  merge.wav

  
 

sa1.wav 和 sa2.wav 被混合重叠在一起,音轨数目不需要一样，输出文件可能减少音轨,输出文
件是不可逆的。

    混合重叠两个采样率一样的wav

sox -M sa1.wav sa2.wav  merge.wav

  
 

采用sequence或merge来联和文件时,输入文件的样本速率必须一样,否则联合
不起来。例如,采用merge联合两个文件:
实际上,-M参数主要用来将几个声道混合成一个联合声道,例如将两个单声道混合成立体声道

    查看一个wav文件的 采样率和声道

sox  -V  sa1.wav  -n

  
 

输出结果如下：
wav音频头信息

备注：在安装了mp3lame或libmad库支持以后,能将wav格式转为mp3格式
参考链接

文章来源: positive.blog.csdn.net，作者：墨理学AI，版权归原作者所有，如需转载，请联系作者。

原文链接：positive.blog.csdn.net/article/details/84029633
