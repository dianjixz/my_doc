hostapd 是用于将建立 wifi 热点的工具
hostapd 创建5G热点
``` bash
#hostapd.conf
interface=wlan0
driver=nl80211
ssid=ingenic
# 36, 44, 52, 60, 100, 108, 116, 124, 132, 140
# 149, 157, 184, 192
channel=44
hw_mode=a
ieee80211n=1
ieee80211ac=1
macaddr_acl=0
 
wmm_enabled=1         # QoS support
 
 
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=12345678
wpa_key_mgmt=WPA-PSK
wpa_pairwise=CCMP
rsn_pairwise=CCMP
 
#add by huohongpeng
ctrl_interface=/var/run/hostapd






```


2.4g 热点
```bash
#/etc/hostapd_ori.conf 
interface=wlan1
ssid=TonyOpenWRT
channel=9
hw_mode=g
ignore_broadcast_ssid=0
auth_algs=1
wpa=3
wpa_passphrase=TonyHo123456
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP

```

即我们创建一个SSID为TonyOpenWRT的热点， wpa_passphrase为密码， 后面的为加密方式， 然后还选择了channel， 这个channel可以通过前面的iw list来查看， 例如其中一个为： 

``` bash
hostapd -B -d /etc/hostapd_ori.conf 
ifconfig wlan1 192.168.2.1  
```

转载自：老丁的Linux：http://laoding.blog.51cto.com/980622/1697015


hostapd 的配置文件位于 /etc/hostapd/hostapd.conf 。
``` bash
interface=wlp2s0
#bridge=br0
driver=nl80211
logger_syslog=-1
logger_syslog_level=2
logger_stdout=-1
logger_stdout_level=2
#ctrl_interface=/var/run/hostapd
#ctrl_interface_group=wheel
#ctrl_interface_group=0
##### IEEE 802.11 related configuration #######################################
ssid=test
#ssid2=mytest
#ssid2=P"hello\nthere"
utf8_ssid=1
# ISO/IEC 3166-1 标准的国家代码，用于设置监管域。根据需要设置，按照哪个国家来操作设备。
# 这可以限制可用的频道和发射功率。
#country_code=CN
# 是否启用 IEEE 802.11d，0 禁用（默认），1 启用。
# 需要正确设置 country_code 才能使 IEEE 802.11d 正常工作。
#ieee80211d=1
# 是否启用 IEEE 802.11h，0 禁用（默认），1 启用。
# 这将启用雷达探测和 DFS 支持，如果可用话。
#ieee80211h=1
# 添加 Power Constraint element 到 Beacon 和 Probe Response frames 中去。
#local_pwr_constraint=5
# 该选项仅在 ieee80211h=1 选项并且已经配置 local_pwr_constraint 选项时可用。
#spectrum_mgmt_required=1
# 设置无线模式需要硬件支持，a = IEEE 802.11a, b = IEEE 802.11b, g = IEEE 802.11g, ad = IEEE 802.11ad (60 GHz)
# hw_mode=a 或 hw_mode=g 选项也与 IEEE 802.11n 一起使用来设置波段。
# 当使用 ACS 时(看下文 channel parameter )，可以使用特殊的选项值 hw_mode=any ，说明可以使用任何可用的波段，该值当前仅在驱动使用了 offloaded ACS 的情况下有效。
# 默认: hw_mode=b
hw_mode=g
# 注意有些设备不使用这个选项，而是需要在 iwconfig 中另外配置。
# 如果编译的时候启用了 CONFIG_ACS 选项。通过设置 channel=acs_survey 或 channel=0，这样 hostapd 可以在运行时基于 ACS survey 算法自动选择合适的信道。
# 有些设备仅支持最大为11频道，有些支持最大为13频道，可以通过iw list 查看。
# 另外启用 ht_capab 将影响可用的频道数
channel=6
# ACS tuning - 信道自动选择
# 参考：http://wireless.kernel.org/en/users/Documentation/acs
# 信道自动选择，有助于选择更好的信道，但是会增加启动时间，一般不需要修改。
# 可以通过设置下面选项自定义 ACS survey 算法。
# 默认：acs_num_scans=5  acs_chan_bias=1:0.8 6:0.8 11:0.8
#acs_num_scans=40
#acs_chan_bias=1:0.8 6:0.8 11:0.8
# 默认允许所有信道
#chanlist=100 104 108 112 116
#chanlist=1 6 11-13
# beacon 间隔  （默认: 100; 范围 15到65535）
#beacon_int=100
#dtim_period=1
# 默认且最大值为2007个列表
#max_num_sta=255
#rts_threshold=2347
#fragm_threshold=2346
#supported_rates=10 20 55 110 60 90 120 180 240 360 480 540
#basic_rates=10 20
#basic_rates=10 20 55 110
#basic_rates=60 120 240
# 用命令：iw list 查看无线网卡是否有“short preamble supported”。
# 0 禁用，1 启用，无线网卡支持的话可以启用以改善网络性能。
#preamble=1
# 无线MAC地址过滤
# 此选项要求 driver=hostap 或 driver=nl80211
# 0 = 接受所有连接，仅拒绝 deny 列表中的连接
# 1 = 拒绝所有连接，仅接受 accept 列表中的连接
# 2 = 使用外部 RADIUS 服务器 (还是会优先查找 accept/deny 列表)
# 简单一点的话，可以将两个列表设置为同一个文件，仅由 macaddr_acl 来控制允许或拒绝
macaddr_acl=0
accept_mac_file=/etc/hostapd/hostapd.accept
deny_mac_file=/etc/hostapd/hostapd.deny
# 身份验证算法，1 开放系统认证，2 共享密钥认证(需要 WEP)，3 两者
auth_algs=3
# 是否广播，0 广播SSID，1 不广播SSID（发送空SSID，即长度为0的SSID），
# 2 忽略广播 SSID 的探测请求，跟1类似，但是兼容某些不支持空SSID的设备
#ignore_broadcast_ssid=0
#vendor_elements=dd0411223301
# 启用WMM
wmm_enabled=1
#
# WMM-PS Unscheduled Automatic Power Save Delivery [U-APSD]
# Enable this flag if U-APSD supported outside hostapd (eg., Firmware/driver)
#uapsd_advertisement_enabled=1
#
# Low priority / AC_BK = background
#wmm_ac_bk_cwmin=4
#wmm_ac_bk_cwmax=10
#wmm_ac_bk_aifs=7
#wmm_ac_bk_txop_limit=0
#wmm_ac_bk_acm=0
# Note: for IEEE 802.11b mode: cWmin=5 cWmax=10
#
# Normal priority / AC_BE = best effort
#wmm_ac_be_aifs=3
#wmm_ac_be_cwmin=4
#wmm_ac_be_cwmax=10
#wmm_ac_be_txop_limit=0
#wmm_ac_be_acm=0
# Note: for IEEE 802.11b mode: cWmin=5 cWmax=7
#
# High priority / AC_VI = video
#wmm_ac_vi_aifs=2
#wmm_ac_vi_cwmin=3
#wmm_ac_vi_cwmax=4
#wmm_ac_vi_txop_limit=94
#wmm_ac_vi_acm=0
# Note: for IEEE 802.11b mode: cWmin=4 cWmax=5 txop_limit=188
#
# Highest priority / AC_VO = voice
#wmm_ac_vo_aifs=2
#wmm_ac_vo_cwmin=2
#wmm_ac_vo_cwmax=3
#wmm_ac_vo_txop_limit=47
#wmm_ac_vo_acm=0
# Note: for IEEE 802.11b mode: cWmin=3 cWmax=4 burst=102
# 检测客户端不活动超时,是否在线间隔时间,默认每5分钟检测客户端是否在线
# 站点不活动限制，默认5分钟
#ap_max_inactivity=300
# 默认禁用，设置为1,说明只要客户在5分钟内没有活动就断线,即使客户端仍然在AP信号的范围内，如果客户端过多可以启用来提高带机量。
#skip_inactivity_poll=0
# 对于过度的传输失败或其他连接丢失的情况下，断开连接，这并不适合所有设备
#disassoc_low_ack=1
# 最大允许侦听间隔，默认65535,即不限制 (how many Beacon periods STAs are allowed to remain asleep)
#max_listen_interval=100
# WDS(4-address frame)模式，仅在 driver=nl80211 的情况下支持
#wds_sta=1
# 仅在设置了bridge= 时有效，默认情况下，例如当设置了bridge=br0，那么wds_bridge=br0。
# 当然也可以手动指定为wds_bridge=XXXXX。
#wds_bridge=wds-br0
# Start the AP with beaconing disabled by default.没明白是什么意思。
#start_disabled=0
# AP隔离，1启用，0禁用。默认不隔离。
#ap_isolate=1
#bss_load_update_period=50
# 用于测试的
#bss_load_test=12:80:20000
##### IEEE 802.11n related configuration ######################################
# 启用80211n，你也将需要启用WMM来使用全部HT功能。
ieee80211n=1
# [HT40-]或[HT40+]没有设置的话将只工作在20MHz，
# [HT40-]对应可使用的频道为5-13
# [HT40+]对应可使用的频道为1-7（欧洲标准为1-9）
# 当多径效应不是很严重时，用户可以将该间隔配置为[SHORT-GI-20][SHORT-GI-40]
# 个人理解，如果不上天馈系统的话可以启用[SHORT-GI-20][SHORT-GI-40]，前提是硬件支持。
#ht_capab=[LDPC][HT40-][HT40+][SHORT-GI-20][SHORT-GI-40][SMPS-DYNAMIC][GF][TX-STBC][RX-STBC123][DELAYED-BA][MAX-AMSDU-7935][DSSS_CCK-40][40-INTOLERANT][LSIG-TXOP-PROT]
# intel Wireless-N 105 (rev c4) 网卡支持的选项
ht_capab=[HT40-][SHORT-GI-20][SHORT-GI-40][DSSS_CCK-40][40-INTOLERANT][GF]
# Atheros AR9271 网卡支持的选项
#ht_capab=[HT40-][SHORT-GI-20][SHORT-GI-40][DSSS_CCK-40][40-INTOLERANT][RX-STBC1]
#ht_capab=[HT40-][SHORT-GI-20][SHORT-GI-40][DSSS_CCK-40]
#ht_capab=[HT40-][SHORT-GI-20][SHORT-GI-40]
#ht_capab=[HT40+][DSSS_CCK-40][40-INTOLERANT]
# 强制要求客户端使用HT PHY，不使用就拒绝。
#require_ht=0
# 间隔时间（秒）
#obss_interval=5
##### WPA/IEEE 802.11i configuration ##########################################
# 1 wpa，2 wpa2, 3 两者。 新的设备可以设置为2，仅使用wpa2版本。
wpa=2
# 指定密码，ASCII码或密码字符。
#wpa_psk=0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef
#wpa_passphrase=12345678
# 使用密码文件，可以跟客户端MAC地址绑定设置不同的密码，比较实用。
wpa_psk_file=/etc/hostapd/hostapd.wpa_psk
# 是否使用验证服务器，0 禁用（默认），1 可用，2 强制使用（其他方式将拒绝）
#wpa_psk_radius=0
# 设置密钥管理算法WPA-PSK WPA-EAP或两者
wpa_key_mgmt=WPA-PSK
# 设置加密算法，CCMP就是AES，不推荐使用TKIP，除非设备不支持。
wpa_pairwise=CCMP
# 默认情况下rsn_pairwise自动设置为wpa_pairwise的值，当然也可以手动指定。
#rsn_pairwise=CCMP
#wpa_group_rekey=600
#wpa_strict_rekey=1
#wpa_gmk_rekey=86400
#wpa_ptk_rekey=600
# 预认证
#rsn_preauth=1
#rsn_preauth_interfaces=wlp2s0 br0 enp3s0
#peerkey=1
# 80211w支持，0 禁用（默认），1 可选，2 强制使用（其他方式将拒绝） 有些网卡不支持
#ieee80211w=1
# AES-128-CMAC（默认） BIP-GMAC-128 BIP-GMAC-256 BIP-CMAC-256 四种
# 一般通用设备都支持AES-128-CMAC，其他的不一定
#group_mgmt_cipher=AES-128-CMAC
#assoc_sa_query_max_timeout=1000
#assoc_sa_query_retry_timeout=201
#disable_pmksa_caching=0
#okc=1
#sae_anti_clogging_threshold=5
#sae_groups=19 20 21 25 26
##### IEEE 802.11r configuration ##############################################
# Wi-Fi无线漫游标准
#mobility_domain=a1b2
#r0_key_lifetime=10000
#r1_key_holder=000102030405
#reassociation_deadline=1000
#pmk_r1_push=1
##### Neighbor table ##########################################################
#ap_table_max_size=255
#ap_table_expiration_time=3600
##### IEEE 802.11v-2011 #######################################################
# Wi-Fi节能版 
#time_advertisement=2
#time_zone=CST8
# 下面三项有些网卡不支持
#wnm_sleep_mode=1
#bss_transition=1
#proxy_arp=1
 
#bss=wifi0
#bssid=00:13:10:95:fe:0d
#interface=wifi0
#ssid=test2
#bridge=br0
#driver=nl80211
#auth_algs=1
#ignore_broadcast_ssid=0
#wpa=2
#wpa_passphrase=12345678
```
实际一个高通dongle ap配置：
``` bash
interface=wlan0
driver=nl80211
ctrl_interface_group=0
hw_mode=g
max_num_sta=2
ssid=ap_wifi
auth_algs=1
channel=0
#ieee80211n=1
#ht_capab=[HT20][SHORT-GI-20]
#vht_oper_chwidth=1
#vht_oper_centr_freq_seg0_idx=42
country_code=CN
ieee80211d=1
wpa=2
wpa_key_mgmt=WPA-PSK
wpa_passphrase=12345678
rsn_pairwise=CCMP
wpa_pairwise=CCMP
```



``` bash
interface=wlan1
driver=nl80211
ssid=nihao
# 36, 44, 52, 60, 100, 108, 116, 124, 132, 140
# 149, 157, 184, 192
channel=44
hw_mode=a
ieee80211n=1
macaddr_acl=0
wmm_enabled=1         # QoS support
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=12345678
wpa_key_mgmt=WPA-PSK
wpa_pairwise=CCMP
rsn_pairwise=CCMP
#add by huohongpeng
ctrl_interface=/var/run/hostapd


```