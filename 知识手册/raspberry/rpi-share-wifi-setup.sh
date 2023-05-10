#!/bin/bash

# ENV list:
# R_SSID
# R_PWD
# R_HOST_IP
# R_MASK
# R_CLIENT_IP_RANGE

echo "##### 开始配置 树莓派共享 WiFi #####"

set -e
set -x

# prepare
apt update

# setup hostapd
# ====================================
apt install hostapd -y
systemctl stop hostapd

cat << EOF > /etc/hostapd/hostapd.conf
interface=wlan0
driver=nl80211
ssid=$R_SSID
hw_mode=g
channel=7
wmm_enabled=0
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=$R_PWD
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
EOF

echo 'DAEMON_CONF="/etc/hostapd/hostapd.conf"' > /etc/default/hostapd

systemctl unmask hostapd
systemctl enable hostapd
systemctl start hostapd

# setup dhcpcd
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

interface wlan0
  static ip_address=$R_HOST_IP/24
  nohook wpa_supplicant
EOF

systemctl start dhcpcd


# setup dnsmasq
# ====================================
apt install dnsmasq -y
systemctl stop dnsmasq

cat << EOF > /etc/dnsmasq.conf
interface=wlan0
  dhcp-range=$R_CLIENT_IP_RANGE,$R_MASK,24h
EOF

systemctl start dnsmasq


# setup iptable
# ====================================
echo 'net.ipv4.ip_forward=1' > /etc/sysctl.d/routed-ap.conf

cat << EOF > /usr/local/bin/flush-rpi-share-wifi-iptables.sh
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
EOF

chmod +x /usr/local/bin/flush-rpi-share-wifi-iptables.sh

cat << EOF > /usr/lib/systemd/rpi-share-wifi-iptables.service
[Unit]
Description=rpi-share-wifi-iptables

[Service]
Type=oneshot
ExecStart=/usr/local/bin/flush-rpi-share-wifi-iptables.sh

[Install]
WantedBy=multi-user.target
EOF

systemctl enable /usr/lib/systemd/rpi-share-wifi-iptables.service
systemctl start rpi-share-wifi-iptables

set +x

echo "[电气罐头] 树莓派共享 WiFi 配置完成！建议重启以便完全生效！"
echo "@see  https://tech.biko.pub/#/tool/rpi-share-wifi"
