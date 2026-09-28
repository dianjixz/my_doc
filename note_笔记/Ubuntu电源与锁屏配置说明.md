# Ubuntu 电源与锁屏配置说明

## 配置目的

本机已调整为：

```text
只允许屏幕变黑/锁屏，不允许系统自动挂起、休眠或混合睡眠。
```

也就是说：

- 后台程序会继续运行
- SSH、下载、训练、编译、脚本等任务不会因为系统睡眠而暂停
- 屏幕可以自动关闭或锁屏
- 系统级 suspend / hibernate / hybrid-sleep 已被禁用

---

# 一、已执行的主要配置

## 1. 禁止 GNOME 桌面空闲后自动挂起

已执行：

```bash
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type 'nothing'
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type 'nothing'
```

含义：

| 配置项 | 当前值 | 含义 |
|---|---|---|
| `sleep-inactive-ac-type` | `nothing` | 接电源时，空闲后不挂起 |
| `sleep-inactive-battery-type` | `nothing` | 使用电池时，空闲后不挂起 |

查看当前值：

```bash
gsettings get org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type
gsettings get org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type
```

期望输出：

```text
'nothing'
'nothing'
```

---

## 2. 设置屏幕空闲时间为 3600 秒

已执行：

```bash
gsettings set org.gnome.desktop.session idle-delay 3600
```

含义：

```text
空闲 3600 秒，也就是 1 小时后，系统进入空闲状态。
```

在 GNOME 桌面中，这通常对应：

- 屏幕变暗
- 屏幕关闭
- 触发锁屏逻辑

但不会让系统 suspend，因为前面已经禁用了自动挂起。

查看当前值：

```bash
gsettings get org.gnome.desktop.session idle-delay
```

期望输出：

```text
uint32 3600
```

---

## 3. 查看锁屏状态

你执行过：

```bash
gsettings get org.gnome.desktop.screensaver lock-enabled
gsettings get org.gnome.desktop.screensaver lock-delay
```

这两个命令只是查看，没有修改。

含义：

| 配置项 | 含义 |
|---|---|
| `lock-enabled` | 是否启用自动锁屏 |
| `lock-delay` | 屏幕空闲后多久锁屏 |

查看命令：

```bash
gsettings get org.gnome.desktop.screensaver lock-enabled
gsettings get org.gnome.desktop.screensaver lock-delay
```

如果希望确保自动锁屏开启，可以执行：

```bash
gsettings set org.gnome.desktop.screensaver lock-enabled true
```

如果希望屏幕关闭后立刻锁屏，可以执行：

```bash
gsettings set org.gnome.desktop.screensaver lock-delay 0
```

---

## 4. 系统级禁用 sleep/suspend/hibernate/hybrid-sleep

已执行：

```bash
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target suspend-then-hibernate.target
```

含义：

系统层面禁用了以下目标：

| target | 含义 |
|---|---|
| `sleep.target` | 睡眠总目标 |
| `suspend.target` | 挂起到内存 |
| `hibernate.target` | 休眠到硬盘 |
| `hybrid-sleep.target` | 混合睡眠 |
| `suspend-then-hibernate.target` | 先挂起后休眠 |

执行后，即使运行下面命令，也不会真正进入睡眠：

```bash
systemctl suspend
systemctl hibernate
systemctl hybrid-sleep
```

这是一种比较强制的禁用方式。

查看是否已禁用：

```bash
systemctl status sleep.target suspend.target hibernate.target hybrid-sleep.target suspend-then-hibernate.target
```

或者：

```bash
systemctl list-unit-files | grep -E 'sleep|suspend|hibernate'
```

如果看到 `masked`，说明已被禁用。

---

# 二、当前效果总结

当前配置效果应该是：

```text
1. Ubuntu 不会因为空闲自动 suspend。
2. Ubuntu 不会自动 hibernate。
3. Ubuntu 不会 hybrid-sleep。
4. 屏幕可以在空闲后关闭。
5. 锁屏功能仍然可以使用。
6. 后台进程不会因为系统睡眠而暂停。
```

适合以下场景：

- 长时间下载
- 后台编译
- Python 脚本运行
- 机器学习训练
- SSH 远程连接
- 服务器式使用桌面机
- 长时间运行 Docker、数据库、服务程序

---

# 三、以后如何修改配置

## 1. 修改多久后屏幕变黑/进入空闲

当前是 3600 秒，也就是 1 小时。

### 改成 5 分钟

```bash
gsettings set org.gnome.desktop.session idle-delay 300
```

### 改成 10 分钟

```bash
gsettings set org.gnome.desktop.session idle-delay 600
```

### 改成 30 分钟

```bash
gsettings set org.gnome.desktop.session idle-delay 1800
```

### 改成 1 小时

```bash
gsettings set org.gnome.desktop.session idle-delay 3600
```

### 永不自动黑屏

```bash
gsettings set org.gnome.desktop.session idle-delay 0
```

查看当前值：

```bash
gsettings get org.gnome.desktop.session idle-delay
```

---

## 2. 修改是否自动锁屏

### 开启自动锁屏

```bash
gsettings set org.gnome.desktop.screensaver lock-enabled true
```

### 关闭自动锁屏

```bash
gsettings set org.gnome.desktop.screensaver lock-enabled false
```

查看当前状态：

```bash
gsettings get org.gnome.desktop.screensaver lock-enabled
```

---

## 3. 修改屏幕关闭后多久锁屏

### 立即锁屏

```bash
gsettings set org.gnome.desktop.screensaver lock-delay 0
```

### 30 秒后锁屏

```bash
gsettings set org.gnome.desktop.screensaver lock-delay 30
```

### 5 分钟后锁屏

```bash
gsettings set org.gnome.desktop.screensaver lock-delay 300
```

查看当前值：

```bash
gsettings get org.gnome.desktop.screensaver lock-delay
```

---

# 四、以后如何恢复系统睡眠功能

你之前执行了：

```bash
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target suspend-then-hibernate.target
```

如果以后想恢复 suspend / hibernate / hybrid-sleep，需要执行：

```bash
sudo systemctl unmask sleep.target suspend.target hibernate.target hybrid-sleep.target suspend-then-hibernate.target
```

然后可以测试：

```bash
systemctl suspend
```

---

# 五、恢复 GNOME 自动挂起

如果你以后想恢复 Ubuntu 默认的自动挂起行为，可以设置回 `suspend`。

## 接电源时空闲后自动挂起

```bash
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type 'suspend'
```

## 使用电池时空闲后自动挂起

```bash
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type 'suspend'
```

查看：

```bash
gsettings get org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type
gsettings get org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type
```

如果输出：

```text
'suspend'
'suspend'
```

说明已经恢复为空闲后自动挂起。

---

# 六、推荐的常用配置组合

## 方案 A：只锁屏，不睡眠，适合长期跑任务

```bash
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type 'nothing'
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type 'nothing'
gsettings set org.gnome.desktop.session idle-delay 3600
gsettings set org.gnome.desktop.screensaver lock-enabled true
gsettings set org.gnome.desktop.screensaver lock-delay 0
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target suspend-then-hibernate.target
```

效果：

```text
1 小时后屏幕空闲/锁屏，但系统不睡眠，后台程序继续运行。
```

---

## 方案 B：恢复普通桌面电脑默认行为

```bash
sudo systemctl unmask sleep.target suspend.target hibernate.target hybrid-sleep.target suspend-then-hibernate.target

gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type 'suspend'
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type 'suspend'
gsettings set org.gnome.desktop.session idle-delay 300
gsettings set org.gnome.desktop.screensaver lock-enabled true
gsettings set org.gnome.desktop.screensaver lock-delay 0
```

效果：

```text
5 分钟后屏幕空闲/锁屏，并允许系统自动挂起。
```

---

# 七、快速检查当前配置

以后可以执行下面这一组命令检查当前状态：

```bash
echo "GNOME AC sleep:"
gsettings get org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type

echo "GNOME Battery sleep:"
gsettings get org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type

echo "Idle delay:"
gsettings get org.gnome.desktop.session idle-delay

echo "Lock enabled:"
gsettings get org.gnome.desktop.screensaver lock-enabled

echo "Lock delay:"
gsettings get org.gnome.desktop.screensaver lock-delay

echo "System sleep targets:"
systemctl list-unit-files | grep -E 'sleep|suspend|hibernate'
```

如果你想要的是“只锁屏不睡眠”，理想状态类似：

```text
GNOME AC sleep:
'nothing'

GNOME Battery sleep:
'nothing'

Idle delay:
uint32 3600

Lock enabled:
true

System sleep targets:
sleep.target masked
suspend.target masked
hibernate.target masked
hybrid-sleep.target masked
suspend-then-hibernate.target masked
```

---

# 八、手动锁屏命令

如果你只是想手动锁屏，但不中断后台程序，可以用：

```bash
loginctl lock-session
```

或者 GNOME 下：

```bash
gnome-screensaver-command -l
```

如果后者不可用，推荐使用：

```bash
loginctl lock-session
```

也可以直接按快捷键：

```text
Super + L
```

通常 `Super` 就是 Windows 键。

---

# 九、重要说明

当前配置中最关键的是这两类设置：

## GNOME 层面

```bash
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type 'nothing'
gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-battery-type 'nothing'
```

它们负责防止桌面环境因为空闲而自动挂起。

## systemd 层面

```bash
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target suspend-then-hibernate.target
```

它们负责从系统层面禁止睡眠、挂起和休眠。

所以当前系统基本不会进入睡眠状态，后台任务会持续运行。