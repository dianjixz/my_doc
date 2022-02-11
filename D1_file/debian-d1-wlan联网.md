 # D1 连接 wifi 流程

第一步:

​	加载 wifi 驱动

~~~ bash
modprobe xr829
~~~

第二步:

​	开启 wlan0

~~~ba
ifconfig wlan0 up
~~~

第三步:

​	编辑 wifi 密码文件: /etc/wpa_supplicant.conf

~~~ bash
root@MaixLinux:~# cat /etc/wpa_supplicant.conf 
ctrl_interface=/var/run/wpa_supplicant GROUP=wheel

network={
    ssid="****"
    psk="*****"
}
~~~



第四步:

​	开启 wpa_supplicant 服务.

~~~ bash
wpa_supplicant -Dwext -iwlan0 -c/etc/wpa_supplicant.conf -dd -B

-D:驱动名称  
  nl80211 = Linux nl80211/cfg80211
  wext = Linux wireless extensions (generic)
  wired = Wired Ethernet driver
-c:配置文件
-i:网口名称
&:后台运行
更多命令执行wpa_supplicant --help 

~~~



第五步:

​	获取 dhcp :

~~~ bash
dhclient -i wlan0
~~~



wpa_cli -i wlan0 scan         　//搜索附件wifi热点
wpa_cli -i wlan0 scan_result 　 //显示搜索wifi热点
wpa_cli -i wlan0 status        //当前WPA/EAPOL/EAP通讯状态
wpa_cli -i wlan0 ping          //pings wpa_supplicant
————————————————
版权声明：本文为CSDN博主「Ruler.」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_24835087/article/details/104783990
