1.getevent
输出所有event设备的基本信息

![img](https://img2018.cnblogs.com/blog/1309603/201906/1309603-20190616003735366-467182845.png)

 


输出格式：
设备名： 事件type 事件code 事件value

注意：这里的数字都是16进制。

getevent -c 10 //输出10条信息后退出
getevent -l //将type、code、value以对应的常量名称显示

使用 getevent -l 命令后输出结果格式为： ![img](https://img2018.cnblogs.com/blog/1309603/201906/1309603-20190616003704705-867509113.png)

 



对应的数字代码转换为容易理解的字符串了，这些常量定义都在input.h文件中。

2.sendevent
发送事件命令，模拟事件操作。
该命令格式如下：

![img](https://img2018.cnblogs.com/blog/1309603/201906/1309603-20190616003651663-1569833255.png)

 

注意：和getevent命令输出的结果不一样，这里的type,code,value都是10进制的。

看这张图
![img](https://img2018.cnblogs.com/blog/1309603/201906/1309603-20190616003633375-867731018.png)

这是在联想A390t上点击音量减按键的时候，getevent的输出。很容易理解，分为四步：按下音量减->发送同步报告->抬起音量减->发送同步报告。只有发送了同步报告以后事件才能生效。
转化成16进制输出：

![img](https://img2018.cnblogs.com/blog/1309603/201906/1309603-20190616003621343-721143466.png)

将其中的16进制数转化成10进制数（0x72->114）,使用sendevent命令模拟按下音量减键的效果试试。
依次输入如下命令：

sendevent /dev/input/event0 0001 0114 00000001
sendevent /dev/input/event0 0000 0000 00000000
sendevent /dev/input/event0 0001 0114 00000000
sendevent /dev/input/event0 0000 0000 00000000

可以发现，在输入了前两天命令之后出现了音量调节窗口，如果不输入后两条命令，音量调节窗口是不会消失的。

最后强调一下：
（1）getevent输出的是16进制，sendevent使用的是10进制，注意转换。
（2）getevent命令的输出结果因设备不同而不同，但都是相似的。

 sudo evtest /dev/input/event0