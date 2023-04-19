我们有一台linux服务器，它有一个无线网卡连接了互联网，还有一个有线网卡，有线网卡网口通过网线和另一台linux服务器B连接, 服务器B没有互联网．所以我们想把服务器A的互联网共享给服务器B使用．

经过看了几篇其他老师分享的文章，整理后的思路就是：先把服务器A的无线网共享给服务器Ａ的有限网卡，然后打开服务器A的内核ＩＰ转发功能；再就是把服务器Ｂ的网卡ｉｐ设置成和服务器的有线网卡ｉｐ在一个网段，网关设置成服务器Ａ的有限网卡ｉｐ．接下来记录一下具体步骤：
步骤一：设置服务器A的无线网共享给服务器Ａ的有限网卡

打开ｗｉｆｉ网络设置

nm-connection-editor

然后点击左下角的＂＋＂，设置连接名称，以太网选项页中设备选择网卡编号

然后在ipv4设置页面中的方法设置为与其它计算机共享，然后点击保存．到这步，完成了无线网共享给有线网卡．

步骤二：打开服务器A的内核IP转发功能

    #  echo 1 > /proc/sys/net/ipv4/ip_forward      // 打开ip转发
     
    #  iptables -F
     
    #  iptables -P INPUT ACCEPT
     
    #  iptables -P FORWARD ACCEPT
     
    # iptables -t nat -A POSTROUTING -o wlp2s0 -j MASQUERADE     （wlp2s0为A接外网的无线网卡）

步骤三：设置服务器B的网卡的ｉp和网关

设置ｉp和网管可以分为永久和暂时：因为ubuntu和centos的用就设置网卡ｉｐ的配置文件路径不一致：，就说一下临死的设置方案：

    ifconfig 网卡名　ip/24   # 设置网卡ｉp和服务器A的有限网卡ｉｐ在一个网段
    route add -net 0.0.0.0/0 gw 服务器A的有限网卡ＩＰ　　＃　设置服务器Ａ的有限网卡ｉｐ为服务器Ｂ的网关

设置完成后，可以ping www.baidu.com , 看一下网络是否可用了
————————————————
版权声明：本文为CSDN博主「dogheader」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/dogheader/article/details/128394561