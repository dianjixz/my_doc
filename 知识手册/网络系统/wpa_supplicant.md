 

# wpa_supplicant及wpa_cli使用方法

###### [Linux](https://www.ucloud.cn/yun/linux/)



**wpa_supplicant**是一个连接、配置**WIFI**的工具，它主要包含**wpa_supplicant**与**wpa_cli**两个程序。通常情况下，可以通过**wpa_cli**来进行**WIFI**的配置与连接，如果有特殊的需要，可以编写应用程序直接调用**wpa_supplicant**的接口直接开发。

**启动\**wpa_supplicant\**应用**

```
$ wpa_supplicant -D nl80211 -i wlan0 -c /etc/wpa_supplicant.conf -B
```

**/etc/wpa_supplicant.conf**文件里，添加下面代码:

```
ctrl_interface=/var/run/wpa_supplicant
update_config=1
```

**启动\**wpa_cli\**应用**

```
$ wpa_cli -i wlan0 scan             // 搜索附近wifi网络
$ wpa_cli -i wlan0 scan_result      // 打印搜索wifi网络结果
$ wpa_cli -i wlan0 add_network      // 添加一个网络连接
```

如果要连接加密方式是**[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][ESS]** (**wpa**加密)，**wifi**名称是**name**，**wifi**密码是：**psk**。

```
$ wpa_cli -i wlan0 set_network 0 ssid '"name"'
$ wpa_cli -i wlan0 set_network 0 psk '"psk"'
$ wpa_cli -i wlan0 enable_network 0
```

如果要连接加密方式是**[WEP][ESS]** (**wep**加密)，**wifi**名称是**name**，**wifi**密码是**psk**。

```
$ wpa_cli -i wlan0 set_network 0 ssid '"name"'
$ wpa_cli -i wlan0 set_network 0 key_mgmt NONE
$ wpa_cli -i wlan0 set_network 0 wep_key0 '"psk"'
$ wpa_cli -i wlan0 enable_network 0
```

如果要连接加密方式是**[ESS]** (无加密)，**wifi**名称是**name**。

```
$ wpa_cli -i wlan0 set_network 0 ssid '"name"'
$ wpa_cli -i wlan0 set_network 0 key_mgmt NONE
$ wpa_cli -i wlan0 enable_network 0
```

**分配\**ip/netmask/gateway/dns\****

```
$ udhcpc -i wlan0 -s /etc/udhcpc.script -q
```

执行完毕，就可以连接网络了。

**保存连接**

```
$ wpa_cli -i wlan0 save_config
```

**断开连接**

```
$ wpa_cli -i wlan0 disable_network 0
```

**连接已有的连接**

```
$ wpa_cli -i wlan0 list_network             列举所有保存的连接
$ wpa_cli -i wlan0 select_network 0         连接第1个保存的连接
$ wpa_cli -i wlan0 enable_network 0         使能第1个保存的连接
```

**断开\**wifi\****

```
$ ifconfig wlan0 down
$ killall udhcpc
$ killall wpa_supplicant
```

***\*wpa_wifi_tool\**使用方法**

**wpa_wifi_tool**是基于**wpa_supplicant**及**wpa_cli**的一个用于快速设置**wifi**的工具，方便调试时连接**wifi**使用。使用方法：1、运行**wpa_wifi_tool**;2、输入**help**进行命令查看;3、**s**进行**SSID**扫描;4、**c[n]**进行**wifi**连接，连接时若为新的**SSID**则需输入密码，若为已保存的**SSID**则可以使用保存过的密码或者重新输入密码;5、**e**退出工具。







 **wpa_supplicant作用：**

1、读取配置文件

2、初始化配置参数，驱动函数

3、让驱动scan当前所有的bssid

4、检查扫描的参数是否和用户设置的想否

5、如果相符，通知驱动进行权限 认证操作

6、连上AP

1.运行 wpa supplicant 程序

在init.rc里执行：wpa_supplicant /system/bin/wpa_supplicant-Dwext -ieth0 -c/data/wifi/wpa_supplicant.conf -f /data/wifi/wpa_log.txt

 对于 启动命令wpa_supplicant 带的 参数，用了 两个数据结构 来保存，

一个是 wpa_params, 另一个是wpa_interface.
这主要是考虑到wpa_supplicant是可以 同时支持 多个网络接口的。
wpa_params数据结构 主要记录 与网络接口无关 的一些参数设置。
而每一个网络接口就用一个wpa_interface数据结构来记录。
在启动命令行中，可以用-N来指定将要描述一个新的网络接口，对于一个新的网络接口，可以用下面几个参数描述：
-i : 网络接口名称

-c: 配置文件名称
-C: 控制接口名称
-D: 驱动类型名称
-p: 驱动参数
-b: 桥接口名称

-d: 增加调试信息

/system/bin/wpa_supplicant ：是 wpa_supplicant可执行程序的 path；









# systemctl开机启动wpa_supplicant



各种开机启动搜索，最后才发现系统自带的systemctl也很好使。

Archlinux安装完wpa_supplicant，然后到/etc/wpa_supplicant目录下填写配置文件，随便自定义一个，例如test.conf

~~~ bash
ctrl_interface=/var/run/wpa_supplicant
# update_config=1
network={
ssid="XXXX"
psk="XXX"
}
~~~





该文件配置详见arch官方文档。

接下来，配置service。进入到/usr/lib/systemd/system目录，可以看到下面四个文件：

wpa_supplicant-nl80211@.service
wpa_supplicant.service
wpa_supplicant@.service
wpa_supplicant-wired@.service

编辑wpa_supplicant.service文件，如下：

[Unit]
Description=WPA supplicant


[Service]
Type=dbus
BusName=fi.epitest.hostap.WPASupplicant
ExecStart=/usr/bin/wpa_supplicant -c/etc/wpa_supplicant/test.conf -i wlp3s0


[Install]
WantedBy=multi-user.target
Alias=dbus-fi.epitest.hostap.WPASupplicant.service

其中，只需修改ExecStart=/usr/bin/wpa_supplicant -c/etc/wpa_supplicant/test.conf -i wlp3s0即可。

最后，启动该service，启动命令如下:

\#systemctl  enable wpa_supplicant

重启电脑，测试服务：

$systemctl status wpa_supplicant

如果服务没有问题，那么这个时候应该可以无线上网。





一、wpa_supplicant的使用

运行wpa_supplicant，wpa_supplicant程序作为wpa_cli的服务端，必须先启动后，才能给wpa_cli访问使用。

wpa_supplicant的手册说明：

wpa_supplicant

usage:

wpa_supplicant [-BddhKLqqtvW] [-P<pid file>] [-g<global ctrl>] \

[-G<group>] \

-i<ifname> -c<config file> [-C<ctrl>] [-D<driver>] [-p<driver_param>] \

[-b<br_ifname>] [-e<entropy file>] \

[-o<override driver>] [-O<override ctrl>] \

[-N -i<ifname> -c<conf> [-C<ctrl>] [-D<driver>] \

[-p<driver_param>] [-b<br_ifname>] [-I<config file>] ...]

drivers:

nl80211 = Linux nl80211/cfg80211

wext = Linux wireless extensions (generic)

wired = Wired Ethernet driver

options:

-b = optional bridge interface name

-B = run daemon in the background

-c = Configuration file

-C = ctrl_interface parameter (only used if -c is not)

-d = increase debugging verbosity (-dd even more)

-D = driver name (can be multiple drivers: nl80211,wext)

-e = entropy file

-g = global ctrl_interface

-G = global ctrl_interface group

-h = show this help text

-i = interface name

-I = additional configuration file

-K = include keys (passwords, etc.) in debug output

-L = show license (BSD)

-N = start describing new interface

-o = override driver parameter for new interfaces

-O = override ctrl_interface parameter for new interfaces

-p = driver parameters

-P = PID file

-q = decrease debugging verbosity (-qq even less)

-t = include timestamp in debug messages

-v = show version

-W = wait for a control interface monitor before starting

例：

wpa_supplicant -D nl80211 -i wlan0 -c /etc/wpa_supplicant.conf -B

-D nl80211：表示驱动类型名称为nl80211

-i wlan0：表示网络接口名称为wlan0

-c /etc/wpa_supplicant.conf：表示配置文件为wpa_supplicant.conf

-B：表示将守护进程wpa_supplicant 挂到后台运行

