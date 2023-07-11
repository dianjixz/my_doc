 嵌入式Linux热点分享网络(三) 原创

udhcpd /etc/udhcpd.conf 

udhcpd为局域网提供自动获取IP地址和DNS功能

    创建好AP以后，STA终端可以连接到AP。此时STA想要连接到局域网就需要有IP地址，如果想连接到外网就需要网关。如果想连接到域名表示的外网，就需要dns服务器。

    udhcpd是dhcp服务器，动态的提供IP地址，网关和DNS。所以一般AP端通过hostapod建立了无线的物理连接之后还需要提供DHCP服务功能，这样其他STA就可以获取IP地址等配置，只有获取到IP地址，才是真正意义上组成了局域网。
udhcpd(/etc/udhcpd.conf)关键配置项：

在我的系统中AP是通过wlan0模拟出来的。所以udhcp需要为wlan0提供dhcp服务，需要如下配置：

# The interface that udhcpd will use                                         
interface       wlan0

    1.
    2.

根据实际情况，指定udchpd提供IP地址的范围:

# The start and end of the IP lease block                                    
start           192.168.1.2                          
end             192.168.1.254

    1.
    2.
    3.

指定租期文件存储位置，在启动udhcpd程序之前，需要先创建好这个文件(一般用touch创建一个空文件)，否则程序会报错：

# The location of the leases file                                        
lease_file  /var/udhcpd.leases

    1.
    2.

udhcp指定子网掩码和网关

option    subnet    255.255.255.0                                          
opt       router    192.168.1.1

    1.
    2.

udhcp指定dns服务器地址(可以存在多个)，一般是上一级网络的dns地址，比如AP共享的是eth0的网络,则可以把eth0的dns填写到这里，还可以添加常用的公共dns服务器，比如阿里的，或者114的。

opt       dns       123.150.150.150 114.114.114.114

    1.

udhcpd配置模板
``` bash
# Sample udhcpd configuration file (/etc/udhcpd.conf)
# Values shown are defaults

# The start and end of the IP lease block
start   192.168.10.100
end   192.168.10.254

# The interface that udhcpd will use
interface wlan1

# The maximum number of leases (includes addresses reserved
# by OFFER's, DECLINE's, and ARP conflicts). Will be corrected
# if it's bigger than IP lease block, but it ok to make it
# smaller than lease block.
#max_leases 254

# The amount of time that an IP will be reserved (leased to nobody)
# if a DHCP decline message is received (seconds)
#decline_time 3600

# The amount of time that an IP will be reserved
# if an ARP conflict occurs (seconds)
#conflict_time  3600

# How long an offered address is reserved (seconds)
#offer_time 60

# If client asks for lease below this value, it will be rounded up
# to this value (seconds)
#min_lease  60

# The location of the pid file
#pidfile  /var/run/udhcpd.pid

# The location of the leases file
#lease_file /var/lib/misc/udhcpd.leases

# The time period at which udhcpd will write out leases file.
# If this is 0, udhcpd will never automatically write leases file.
# Specified in seconds.
#auto_time  7200

# Every time udhcpd writes a leases file, the below script will be called
#notify_file      # default: no script
#notify_file  dumpleases  # useful for debugging

# The following are BOOTP specific options
# next server to use in bootstrap
#siaddr   192.168.0.22  # default: 0.0.0.0 (none)
# tftp server name
#sname    zorak   # default: none
# tftp file to download (e.g. kernel image)
#boot_file  /var/nfs_root # default: none

# NOTE: "boot_file FILE" and "opt bootfile FILE" are conceptually the same,
# but "boot_file" goes into BOOTP-defined fixed-size field in the packet,
# whereas "opt bootfile" goes into DHCP option 0x43.
# Same for "sname HOST" and "opt tftp HOST".

# Static leases map
#static_lease 00:60:08:11:CE:4E 192.168.0.54
#static_lease 00:60:08:11:CE:3E 192.168.0.44 optional_hostname

# The remainder of options are DHCP options and can be specified with the
# keyword 'opt' or 'option'. If an option can take multiple items, such
# as the dns option, they can be listed on the same line, or multiple
# lines.
# Examples:
opt dns 192.168.10.2 192.168.10.10
option  subnet  255.255.255.0
opt router  192.168.10.2
opt wins  192.168.10.10
option  dns 129.219.13.81 # appended to above DNS servers for a total of 3
option  domain  local
option  lease 864000    # default: 10 days
option  msstaticroutes  10.0.0.0/8 10.127.0.1   # single static route
option  staticroutes  10.0.0.0/8 10.127.0.1, 10.11.12.0/24 10.11.12.1
# Arbitrary option in hex or string form:
option  0x08  01020304  # option 8: "cookie server IP addr: 1.2.3.4"
option  14  "dumpfile"

# Currently supported options [hex option value] (for more info, see options.c):
#opt lease      NUM             # [0x33]
#opt subnet     IP              # [0x01]
#opt broadcast  IP              # [0x1c]
#opt router     IP_LIST         # [0x03]
#opt ipttl      NUM             # [0x17]
#opt mtu        NUM             # [0x1a]
#opt hostname   STRING          # [0x0c] client's hostname
#opt domain     STRING          # [0x0f] client's domain suffix
#opt search     STRING_LIST     # [0x77] search domains
#opt nisdomain  STRING          # [0x28]
#opt timezone   NUM             # [0x02] (localtime - UTC_time) in seconds. signed
#opt tftp       STRING          # [0x42] tftp server name
#opt bootfile   STRING          # [0x43] tftp file to download (e.g. kernel image)
#opt bootsize   NUM             # [0x0d] size of that file
#opt rootpath   STRING          # [0x11] (NFS) path to mount as root fs
#opt wpad       STRING          # [0xfc] Web Proxy Auto Discovery Protocol
#opt serverid   IP              # [0x36] default: server's IP
#opt message    STRING          # [0x38] error message (udhcpd sends it on success too)
#opt vlanid     NUM             # [0x84] 802.1P VLAN ID
#opt vlanpriority NUM           # [0x85] 802.1Q VLAN priority
# RFC 5071: PXELINUX Options
#opt 0xd0       F100747E        # [0xd0] magic
#opt pxeconffile STRING         # [0xd1]
#opt pxepathprefix STRING       # [0xd2]
#opt reboottime NUM             # [0xd3] bootstrap timeout
# Options specifying server(s)
#opt dns        IP_LIST         # [0x06]
#opt wins       IP_LIST         # [0x2c]
#opt nissrv     IP_LIST         # [0x29]
#opt ntpsrv     IP_LIST         # [0x2a]
#opt lprsrv     IP_LIST         # [0x09]
#opt swapsrv    IP              # [0x10]
# Options specifying routes
#opt routes     IP_PAIR_LIST    # [0x21]
#opt staticroutes   STATIC_ROUTES # [0x79] RFC 3442 classless static route option
#opt msstaticroutes STATIC_ROUTES # [0xf9] same, using MS option number
# Obsolete options, no longer supported
#opt logsrv     IP_LIST # [0x07] 704/UDP log server (not syslog!)
#opt namesrv    IP_LIST # [0x05] IEN 116 name server, obsolete (August 1979!!!)
#opt cookiesrv  IP_LIST # [0x08] RFC 865 "quote of the day" server, rarely (never?) used
#opt timesrv    IP_LIST # [0x04] RFC 868 time server, rarely (never?) used
# TODO: in development
#opt userclass  STRING          # [0x4d] RFC 3004. set of LASCII strings. "I am a printer" etc
#opt sipsrv     STRING LIST     # [0x78] RFC 3361. flag byte, then: 0: domain names, 1: IP addrs
#opt ip6rd ....                 # [0xd4] IPv6 rapid deployment
```





etc/udhcpd.conf
``` bash
# Sample udhcpd configuration file (/etc/udhcpd.conf)

# The start and end of the IP lease block

start		192.168.0.20	#default: 192.168.0.20
end		192.168.0.254	#default: 192.168.0.254


# The interface that udhcpd will use

interface	eth0		#default: eth0


# The maximim number of leases (includes addressesd reserved
# by OFFER's, DECLINE's, and ARP conficts

#max_leases	254		#default: 254


# If remaining is true (default), udhcpd will store the time
# remaining for each lease in the udhcpd leases file. This is
# for embedded systems that cannot keep time between reboots.
# If you set remaining to no, the absolute time that the lease
# expires at will be stored in the dhcpd.leases file.

#remaining	yes		#default: yes


# The time period at which udhcpd will write out a dhcpd.leases
# file. If this is 0, udhcpd will never automatically write a
# lease file. (specified in seconds)

#auto_time	7200		#default: 7200 (2 hours)


# The amount of time that an IP will be reserved (leased) for if a
# DHCP decline message is received (seconds).

#decline_time	3600		#default: 3600 (1 hour)


# The amount of time that an IP will be reserved (leased) for if an
# ARP conflct occurs. (seconds

#conflict_time	3600		#default: 3600 (1 hour)


# How long an offered address is reserved (leased) in seconds

#offer_time	60		#default: 60 (1 minute)

# If a lease to be given is below this value, the full lease time is
# instead used (seconds).

#min_lease	60		#defult: 60


# The location of the leases file

#lease_file	/var/lib/misc/udhcpd.leases	#defualt: /var/lib/misc/udhcpd.leases

# The location of the pid file
#pidfile	/var/run/udhcpd.pid	#default: /var/run/udhcpd.pid

# Everytime udhcpd writes a leases file, the below script will be called.
# Useful for writing the lease file to flash every few hours.

#notify_file				#default: (no script)

#notify_file	dumpleases	# <--- useful for debugging

# The following are bootp specific options, setable by udhcpd.

#siaddr		192.168.0.22		#default: 0.0.0.0

#sname		zorak			#default: (none)

#boot_file	/var/nfs_root		#default: (none)

# The remainer of options are DHCP options and can be specifed with the
# keyword 'opt' or 'option'. If an option can take multiple items, such
# as the dns option, they can be listed on the same line, or multiple
# lines. The only option with a default is 'lease'.

#Examles
opt	dns	192.168.10.2 192.168.10.10
option	subnet	255.255.255.0
opt	router	192.168.10.2
opt	wins	192.168.10.10
option	dns	129.219.13.81	# appened to above DNS servers for a total of 3
option	domain	local
option	lease	864000		# 10 days of seconds


# Currently supported options, for more info, see options.c
#opt subnet
#opt timezone
#opt router
#opt timesrv
#opt namesrv
#opt dns
#opt logsrv
#opt cookiesrv
#opt lprsrv
#opt bootsize
#opt domain
#opt swapsrv
#opt rootpath
#opt ipttl
#opt mtu
#opt broadcast
#opt wins
#opt lease
#opt ntpsrv
#opt tftp
#opt bootfile
#opt wpad

# Static leases map
#static_lease 00:60:08:11:CE:4E 192.168.0.54
#static_lease 00:60:08:11:CE:3E 192.168.0.44


```