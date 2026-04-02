
# 使用 NetworkManager 开启 WiFi 热点

## 1. 安装 NetworkManager

```bash
# Debian / Ubuntu
sudo apt update
sudo apt install network-manager

# CentOS / RHEL
sudo yum install NetworkManager

# Fedora
sudo dnf install NetworkManager

# Arch Linux
sudo pacman -S networkmanager
```

---

## 2. 启动并设置开机自启

```bash
sudo systemctl start NetworkManager
sudo systemctl enable NetworkManager

# 查看运行状态
sudo systemctl status NetworkManager
```

---

## 3. 查看网络设备

```bash
# 查看所有网络设备
nmcli device

# 查看 WiFi 设备名称（一般为 wlan0 或 wlp2s0）
nmcli device | grep wifi
```

输出示例：
```
DEVICE   TYPE      STATE      CONNECTION
wlan0    wifi      connected  --
eth0     ethernet  connected  --
```

---

## 4. 创建 WiFi 热点

```bash
# 基本命令格式
sudo nmcli device wifi hotspot \
  ifname wlan0 \
  ssid "MyHotspot" \
  password "12345678"

# 指定频段（band）：bg=2.4GHz  a=5GHz
sudo nmcli device wifi hotspot \
  ifname wlan0 \
  ssid "MyHotspot" \
  password "12345678" \
  band bg

# 指定信道
sudo nmcli device wifi hotspot \
  ifname wlan0 \
  ssid "MyHotspot" \
  password "12345678" \
  band bg \
  channel 6
```

---

## 5. 管理热点连接

```bash
# 查看所有连接
nmcli connection show

# 查看热点详细信息
nmcli connection show Hotspot

# 启动热点（已创建过）
nmcli connection up Hotspot

# 停止热点
nmcli connection down Hotspot

# 删除热点配置
nmcli connection delete Hotspot
```

---

## 6. 修改热点配置

```bash
# 修改 SSID
nmcli connection modify Hotspot 802-11-wireless.ssid "NewSSID"

# 修改密码
nmcli connection modify Hotspot wifi-sec.psk "NewPassword123"

# 修改 IP 地址段
nmcli connection modify Hotspot ipv4.addresses "192.168.10.1/24"

# 修改后重启热点生效
nmcli connection down Hotspot && nmcli connection up Hotspot
```

---

## 7. 设置开机自动启动热点

```bash
nmcli connection modify Hotspot connection.autoconnect yes

# 取消开机自启
nmcli connection modify Hotspot connection.autoconnect no
```

---

## 8. 查看连接设备

```bash
# 查看连接到热点的客户端 IP
sudo arp -n

# 或者查看 DHCP 分配记录
cat /var/lib/NetworkManager/dnsmasq-*.leases
```

---

## 9. 完整一键脚本

```bash
#!/bin/bash
WIFI_IFACE="wlan0"      # 网卡名
SSID="MyHotspot"         # 热点名称
PASSWORD="12345678"      # 密码（最少8位）

sudo nmcli device wifi hotspot \
  ifname $WIFI_IFACE \
  ssid "$SSID" \
  password "$PASSWORD" \
  band bg

echo "热点已开启: SSID=$SSID  密码=$PASSWORD"
```

---

## 常见错误处理

```bash
# 错误：网卡被占用
sudo nmcli device disconnect wlan0   # 先断开当前连接

# 错误：rfkill 屏蔽
sudo rfkill unblock wifi             # 解除屏蔽

# 查看 rfkill 状态
rfkill list

# 错误：无法共享网络，开启 IP 转发
sudo sysctl -w net.ipv4.ip_forward=1
```

---

## 快速参考

| 操作 | 命令 |
|------|------|
| 开启热点 | `nmcli device wifi hotspot ifname wlan0 ssid "名称" password "密码"` |
| 停止热点 | `nmcli connection down Hotspot` |
| 重启热点 | `nmcli connection up Hotspot` |
| 删除热点 | `nmcli connection delete Hotspot` |
| 查看连接 | `nmcli connection show` |