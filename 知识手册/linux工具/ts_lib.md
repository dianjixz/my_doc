参考：

[tslib 下载地址](https://github.com/libts/tslib)

环境：

ubuntu16.04(虚拟机)

tslib 版本：1.21

目标架构：ARM
1：交叉编译

进入tslib 解压目录，执行 ./autogen.sh 来生成 Makefile 。需要在虚拟机中安装 autoconf、 automake、 libtool 工具。

执行 configure 脚本配置编译属性，我写的如下，仅供参考。

      1 #!/bin/bash
      2 ./configure \
      3 -prefix=$(pwd)/arm-tslib \
      4 --host=arm-linux-gnueabihf \
      5 --cache-file=arm-linux.cache \
      6 ac_cv_func_malloc_0_nonnull=yes \

configure 配置完成之后，执行 make, make install。
2：移植到根文件系统测试

        我的根文件系统使用 buildroot 生成，当然， buildroot 也可以生成 tslib。将上述交叉编译后得到的 arm-tslib 复制到 rootfs 的 /usr/lib 目录下。编辑 /etc/profile ,加入如下内容：

        export TSLIB_ROOT=/usr/lib/arm-tslib
        export TSLIB_CONSOLEDEVICE=none
        export TSLIB_FBDEVICE=/dev/fb0
        export TSLIB_TSDEVICE=/dev/input/event1
        export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf
        export TSLIB_PLUGINDIR=$TSLIB_ROOT/lib/ts
        export LD_PRELOAD=$TSLIB_ROOT/lib/libts.so
     
        #export TSLIB_CALIBFILE=/etc/pointercal
        #此项为电阻屏专用，若使用电容屏幕，可不用此项

        source /etc/profile 命令刷新环境 /etc/profile。运行如下测试程序，点击界面 Draw 按钮即可测试画线操作，路径如下：

/usr/lib/arm-tslib/bin/ts_test
————————————————
版权声明：本文为CSDN博主「苏轼圈」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_41867145/article/details/122654497