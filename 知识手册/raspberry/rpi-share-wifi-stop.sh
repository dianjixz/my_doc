#!/bin/bash

echo "##### 开始停止 树莓派共享 WiFi #####"

set -e
set -x

# stop hostapd
# ====================================
systemctl stop hostapd
systemctl disable hostapd


# revert dhcpcd
# ====================================
systemctl stop dhcpcd

cat << EOF > /etc/dhcpcd.conf
# Inform the DHCP server of our hostname for DDNS.
hostname

# Use the hardware address of the interface for the Client ID.
clientid

# Persist interface configuration when dhcpcd exits.
persistent

# Rapid commit support.
# Safe to enable by default because it requires the equivalent option set
# on the server to actually work.
option rapid_commit

# A list of options to request from the DHCP server.
option domain_name_servers, domain_name, domain_search, host_name
option classless_static_routes

# Respect the network MTU. This is applied to DHCP routes.
option interface_mtu

# A ServerID is required by RFC2131.
require dhcp_server_identifier

# Generate SLAAC address using the Hardware Address of the interface
#slaac hwaddr
# OR generate Stable Private IPv6 Addresses based from the DUID
slaac private
EOF

systemctl start dhcpcd


# stop dnsmasq
# ====================================
systemctl stop dnsmasq


# stop iptable
# ====================================
systemctl stop rpi-share-wifi-iptables
systemctl disable rpi-share-wifi-iptables


set +x

echo "[电气罐头] 树莓派共享 WiFi 已停止！建议重启以便完全生效！"
echo "@see  https://tech.biko.pub/#/tool/rpi-share-wifi"
