 

https://www.cnblogs.com/zongfanstudy/articles/13920114.html

一：modetest

是由libdrm提供的测试程序，可以查询显示设备的支持状况，进行基本的显示测试，以及设置显示的模式。

源码下载：libdrm的下载链接：https://dri.freedesktop.org/libdrm/libdrm-2.4.100.tar.bz2

```
1 # ./configure
2 # Make -j4
```

编译完成后会在目录libdrm-2.4.100/tests/modetest下生成 modetest 可执行文件。

modetest示例：

modetest的运行需要root权限。
modetest -M stm -s 32@39:#0
 

首先这里给出一组显示示例，命令参数如下：

```
1 ./modetest -M vc4 -D 0 -a -s 32@140:1920x1080  -P 173@140:1920x1080 -Ftiles
```

程序运行效果是，通过HDM连接的显示器整屏的显示了渐变的斜条纹：

![img](https://img2020.cnblogs.com/blog/2010724/202011/2010724-20201103142528681-1330029754.png)

通过--help参数可以查看modetest支持的全部选项，如下：

```
1 ./modetest --help
```

下面的命令可以查看目前所在平台支持的分辨率

```
1 modetest -M  所在平台的du的名字
```

 

二：kmscube

是由mesa3d提供和维护，这是一个基于 KMS/GBM/EGL/OPENGL ES2.0 测试用例。

kmscude不依赖X11、wayland等窗口系统的简单图像测试程序。

GBM、EGL和OpenGL ES产生图形数据，再经由DRM/KMS显示。

GBM、EGL和OpenGL ES由mesa3D提供相关的库，DRM/KMS通过libdrm库进行访问。

所以kmscude同时包含OpenGL、EGL、GBM和DRM/KMS的基本用法。

 

下载地址：https://gitlab.freedesktop.org/mesa/kmscube

 

编译

```
1 meson . build && ninja -C build
```

编译成功后，build路径下会生成可执行文件：

```
1 ls build/
2 build.ninja             kernel.trace   kmscube-l.profile   kmscube.profile  'kmscube@exe'/   meson-logs/      oprofile_data/  'texturator@exe'/
3 compile_commands.json   kmscube*       kmscube.1.trace     kmscube.trace     meson-info/     meson-private/   texturator*
```

通过如下命令查看kmscude的帮助信息：

```
1 ./kmscube --help
```

基本用法

```
1 sudo ./kmscube -A -D /dev/dri/card0 -M smooth
```

说明：

  -A：DRM/KMS使用atomic模式

  -D：指定设备节点，DRM/KMS和GPU是同一个设备节点

  -M：指定OpenGL的绘图模式，可选的值包括：smooth、rgba、nv12-2img、nv12-1img

 

# Linux DRM图显系统概述

https://zhuanlan.zhihu.com/p/409427408


DRM_test
https://github.com/zizimumu/DRM_test.git