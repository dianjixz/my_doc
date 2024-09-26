# weston
Weston 是一个开源的显示服务器（display server），它是 Wayland 协议的一个参考实现。Wayland 是一种现代的显示服务器协议，旨在替代传统的 X Window System（X11）。Weston 作为 Wayland 的参考实现，提供了一个轻量级、高性能的显示服务器，适用于各种设备，包括桌面电脑、嵌入式系统和移动设备。

以下是 Weston 的一些主要特点：

1. **轻量级和高性能**：Weston 设计为轻量级和高性能，适合在资源受限的设备上运行。
2. **模块化设计**：Weston 采用模块化设计，可以轻松添加或移除功能模块。
3. **支持多种后端**：Weston 支持多种后端，包括 DRM/KMS、RPI、FBDEV、WAYLAND 和 X11 等，使其可以在不同的硬件和平台上运行。
4. **现代化的图形栈**：Wayland 和 Weston 提供了一个现代化的图形栈，支持硬件加速和高级图形功能。
5. **易于集成**：Weston 易于与现有的应用程序和工具集成，同时也支持新的开发工具和库。

Weston 和 Wayland 的目标是提供一个更简单、更高效、更安全的图形栈，以满足现代计算环境的需求。随着 Wayland 和 Weston 的不断发展，它们在桌面和嵌入式系统中的应用越来越广泛。

在Buildroot中配置Weston非常简单。通过执行 make menuconfig 命令，我们可以进入配置菜单，并选择Weston和相关的依赖包。以下是一些常见的Weston配置选项：

    BR2_PACKAGE_WESTON：启用Weston软件包
    BR2_PACKAGE_WESTON_DRM：启用DRM后端支持
    BR2_PACKAGE_WESTON_FBDEV：启用Framebuffer后端支持
    BR2_PACKAGE_WESTON_X11：启用X11后端支持

通过选择适当的配置选项，可以根据需要启用或禁用Weston的不同功能。

这一步我只打开了第一个。

二、设置自启动以及环境变量

相关文件，自己添加
```
/etc/init.d/S31weston             //weston自启动程序
/etc/xdg/weston/weston.ini        //weston配置文件
/etc/profile.d/env.sh            //环境变量配置
```
1、自启动文件

/etc/init.d/S31weston 
```bash
#!/bin/sh
#
# Start linux launcher...
#
 
case "$1" in
  start)
                printf "Starting weston"
                source /etc/profile.d/env.sh
                weston -c /etc/xdg/weston/weston.ini --tty=2 --idle-time=0 --log=/var/log/weston.log &
        ;;
  stop)
                killall weston
                printf "stop finished"
        ;;
  *)
        echo "Usage: $0 {start|stop}"
        exit 1
        ;;
esac
exit 0
```
2、/etc/xdg/weston/weston.ini
```bash
[shell]
# top(default)|bottom|left|right|none, none to disable panel
# panel-position=none
 
 
[core]
# Boards could have not any input device, and only use
# removable input device like usb(keyboard, mouse).
require-input=false
```
3、/etc/profile.d/env.sh 
```bash
#!/bin/sh 
 
export LC_ALL='zh_CN.utf8'
export QT_QPA_PLATFORM=wayland
#export WESTON_DRM_MIRROR=1
export WESTON_DRM_KEEP_RATIO=1
export QT_GSTREAMER_WINDOW_VIDEOSINK=waylandsink
mkdir -p /tmp/.xdg &&  chmod 0700 /tmp/.xdg
export XDG_RUNTIME_DIR=/tmp/.xdg
export PATH=$PATH:/usr/bin/cmd
export QT_QPA_GENERIC_PLUGINS=evdevkeyboard
```
————————————————


没有亲自测试过，还不知道这样是否可行。
原文链接：https://blog.csdn.net/wwddgod/article/details/138571214