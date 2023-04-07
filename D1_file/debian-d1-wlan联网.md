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


``` bash
wpa_cli -i wlan0 scan         　//搜索附件wifi热点
wpa_cli -i wlan0 scan_result 　 //显示搜索wifi热点
wpa_cli -i wlan0 status        //当前WPA/EAPOL/EAP通讯状态
wpa_cli -i wlan0 ping          //pings wpa_supplicant




#启动脚本
wpa_supplicant -iwlan0 -Dnl80211 -c/etc/ambaipcam/IPC_Q313/config/wlan/wpa_supplicant.conf -B
#查询网卡状态
wpa_cli -p/var/run/wpa_supplicant -iwlan0 status
#搜索附近网络功能  no/ok
wpa_cli -i wlan0 scan
#搜索附近网络,并列出结果
wpa_cli -i wlan0 scan_result
#查看当前连接的是哪个网络
wpa_cli -i wlan0 list_network    
#获取一个存储wifi结构的id,假设为1
wpa_cli -i wlan0 add_network
#设置ID为1的热点SSID
wpa_cli -i wlan0 set_network 1 ssid '"HO4428"'  
#设置ID为1的热点的密码
wpa_cli -i wlan0 set_network 1 psk '"442701102"'
#设置加密方式,可以不加
wpa_cli -i wlan0 set_network 1 key_mgmt WPA2-PSK-CCMP
#设置当前网络优先级,默认为2,可以不加
wpa_cli -i wlan0 set_network 1 priority 2   
#设置ssid默认为1即可  ,可以不加
wpa_cli -i wlan0 set_network 1 scan_ssid 1    
#启动连接ID为1的网络
wpa_cli -i wlan0 enable_network 1
#选择网络0(这样做的好处在于,可以取消先前的其它网络连接)
wpa_cli -i wlan0 select_network 1
#保存刚刚填写的wifi帐号,写入配置文件
wpa_cli -i wlan0 save_config
#请求自动分配IP地址,-b:后台(back) –i:指定接口 –q:获得续约后退出,如果想尽在前台运行,则去掉-b,加上-f
#不用不加,会自动分配
udhcpc -b -i wlan0 -q 
```
————————————————
版权声明：本文为CSDN博主「Ruler.」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_24835087/article/details/104783990
