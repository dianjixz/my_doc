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















背景

基于weston使用qt或者gtk等UI框架显示应用程序的时候，使用wayland平台。

指令差异：

weston --backend=drm-backend.so --tty=7 --use-pixman（使用软件渲染）

weston --backend=drm-backend.so --tty=7（使用硬件GPU渲染）
维度	命令 1（软件渲染）weston --backend=drm-backend.so --tty=7 --use-pixman	命令 2（硬件 GPU 渲染）weston --backend=drm-backend.so --tty=7
渲染方式	基于 pixman 库的软件渲染（CPU 计算图形数据）	基于 GPU 的硬件渲染（依赖显卡驱动加速图形处理）
核心依赖	仅依赖 CPU 和 pixman 库（无需 GPU 驱动支持）	依赖 DRM 兼容的 GPU 硬件 + 对应的开源驱动（如 Intel i915、AMD radeon）
性能表现	性能较低（CPU 负载高），适合简单界面（如终端、轻量 GUI）	性能高（GPU 分担计算），支持复杂界面、动画、高分辨率显示
适用场景	1. GPU 硬件不支持 DRM 渲染；2. GPU 驱动未正确安装 / 加载；3. 调试图形驱动问题	1. 有 DRM 兼容 GPU（如 Intel/AMD 集成显卡、部分 ARM GPU）；2. 需高性能图形显示（如桌面、多媒体）
关键参数解析

两条命令共享部分核心参数，需先理解其作用：

    --backend=drm-backend.so

指定 Weston 使用 DRM 后端（Direct Rendering Manager，直接渲染管理器）—— 这是 Linux 下访问显卡硬件、控制显示输出的核心接口，也是硬件渲染的基础。（Weston 还支持其他后端，如 x11-backend.so（在 X11 上运行）、fbdev-backend.so（帧缓冲后端），但 DRM 后端是硬件加速的主流选择。）

  2.--tty=7指定 Weston 在 第 7 个虚拟终端（TTY） 启动。Linux 系统默认有多个 TTY（通常 1-6 为纯文本终端，7 及以上可分配给图形界面），需确保该 TTY 未被其他进程占用。

   3.--use-pixman（仅命令 1）

强制 Weston 使用 pixman 库进行软件渲染：

    pixman 是一个跨平台的 2D 图形库，负责像素级计算（如绘制、缩放、合成），完全依赖 CPU 完成，不调用 GPU 资源；
    该参数会覆盖硬件渲染，即使 GPU 可用，也会强制走软件路径，主要用于调试或硬件不兼容场景。

使用前提与注意事项
1. 通用前提（两条命令均需满足）

    权限：Weston 访问 DRM 设备（如 /dev/dri/card0）需要 root 权限，需通过 sudo 执行命令（或给当前用户添加 video 用户组：sudo usermod -aG video $USER，注销后生效）。
    DRM 设备存在：需确保系统识别 GPU 并生成 DRM 设备文件（ls /dev/dri/ 应看到 card0 等设备），若不存在，可能是 GPU 驱动未安装（如 Intel/AMD 需开源 mesa 驱动，NVIDIA 需 nvidia-drm 模块）。
    TTY 切换：执行命令后，需通过 Ctrl+Alt+F7 切换到第 7 个 TTY 查看 Weston 界面；退出 Weston 可按 Ctrl+Alt+Backspace（需配置启用），或在 Weston 终端中执行 weston-terminal 后输入 exit。

2. 命令 1（软件渲染）的特殊注意

    性能限制：软件渲染依赖 CPU，若 CPU 性能较弱或界面复杂（如高分辨率、动画），会出现明显卡顿，仅适合简单测试。
    调试用途：若硬件渲染失败（如 Weston 启动崩溃、黑屏），可尝试此命令排查问题 —— 若软件渲染能正常启动，说明问题出在 GPU 驱动或 DRM 硬件兼容；若仍失败，则可能是 Weston 本身或系统环境问题（如依赖库缺失）。

3. 命令 2（硬件渲染）的特殊注意

    GPU 与驱动兼容性：
        支持硬件渲染的 GPU 需满足 DRM KMS（Kernel Mode Setting） 支持（主流 Intel/AMD 集成显卡、ARM Mali/Tegra 显卡均支持，部分老旧 GPU 或冷门型号可能不支持）；
        NVIDIA 显卡需特别注意：开源 nouveau 驱动支持 DRM 渲染，但闭源 nvidia 驱动需开启 nvidia-drm.modeset=1 内核参数（在 grub 配置中添加，重启生效），否则可能无法启用硬件渲染。
    验证硬件渲染是否生效：启动 Weston 后，在 weston-terminal 中执行 weston-info，查看输出中是否有 renderer: "GL" 或 EGL API version: 1.x（EGL 是 GPU 渲染的接口）—— 若显示 renderer: "pixman"，则说明硬件渲染未生效，需排查驱动或参数。

常见问题与排查

    Weston 启动失败，提示 “could not open DRM device”
        原因：权限不足或 DRM 设备不存在；
        解决：用 sudo 执行命令，或检查 GPU 驱动是否安装（glxinfo | grep "OpenGL renderer" 可验证驱动是否加载）。

    命令 2（硬件渲染）启动后黑屏，但命令 1（软件渲染）正常
        原因：GPU 驱动不兼容、DRM KMS 未启用（如 NVIDIA 闭源驱动未开 modeset），或显示器分辨率不支持；
        解决：
            检查内核参数（NVIDIA 需 nvidia-drm.modeset=1）；
            尝试指定分辨率启动：weston --backend=drm-backend.so --tty=7 --mode=1920x1080（需替换为显示器支持的分辨率，可通过 weston-info 查看可用模式）。

    软件渲染卡顿严重
        原因：CPU 负载过高，或 pixman 未启用优化；
        解决：若需临时使用，可降低界面分辨率；长期使用需优先解决硬件渲染问题（安装正确驱动、更换支持 DRM 的 GPU）

    某些情况下在weston下运行demo时候会报错：Could not connect to display，通常网上的分析和采用的方法如下所示：

    该错误是由于在远程或者串口终端环境下没有正确设置DISPLAY导致的。

    在Linux/Unix类操作系统上, DISPLAY用来设置将图形显示到何处。直接登陆图形界面或者登陆命令行界面后使用startx启动图形, DISPLAY环境变量将自动设置为:0.0, 此时可以打开终端, 输出GUI程序的名称来启动程序, 图形将显示在本地窗口上, 在终端上输入printenv查看当前环境变量, 输出结果中有如下内容:

    DISPLAY=:0.0       解决办法 ：   export DISPLAY=':0.0'
————————————————
版权声明：本文为CSDN博主「乌鸦乌鸦你的小虎牙」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/yigedadabing/article/details/152116671

参考链接：

https://blog.csdn.net/yikunbai5708/article/details/104783572/





综合一键示例脚本
```bash
sudo apt update
sudo apt install weston mesa-utils
export LIBGL_ALWAYS_SOFTWARE=1
sudo weston --backend=fbdev-backend.so --device=/dev/fb0
```





