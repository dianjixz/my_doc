# buileroot增加自己的应用程序

一共需要三步：
第一步：在主目录下创建一个world文件夹，在文件夹中创建一个自己的软件文件夹。在文件夹中放上自己的源代码和Makefile文件  
第二步：在package目录下创建自己的软件文件夹。在文件夹中创建一个*.mk 和 Config.in文件  
第三步：在package/Config.in中添加软件菜单  







buildroot package 目录下已经有大量很完善的应用程序，例如用于二维码扫描的Zbar、zxing等。
## 本节描述，构建自己的hello world程序

顶层目录创建work/helloworld目录，用于放置helloworld的源码
helloworld目录下，两个文件helloworld.c Makefile  
~~~
eric@eric-PC:~/Documents/work/linux-c/buildroot-2020.02.1/work/helloworld$ ls -l
总用量 8
-rw-r--r-- 1 eric eric  72 4月  23 11:22 helloworld.c
-rw-r--r-- 1 eric eric 177 4月  23 15:26 Makefile
~~~  
helloworld.c  
~~~  
#include <stdio.h>

int main()
{
	printf("hello world\r\n");
	return 0;
}
~~~  
Makefile  
~~~  
all: helloworld

helloworld: helloworld.o
	$(CC) -o helloworld helloworld.o

clean:
	rm -rf *.o
	rm -rf helloworld

install:
	$(INSTALL) -D -m 0755 helloworld $(TARGET_DIR)/bin
~~~
+ package目录下，新建app-helloworld目录里面两个文件app-helloworld.mk Config.in  
~~~  
eric@eric-PC:~/Documents/work/linux-c/buildroot-2020.02.1/package/app-helloworld$ ls -l
总用量 8
-rw-rw-r-- 1 eric eric 636 4月  23 14:05 app-helloworld.mk
-rw-rw-r-- 1 eric eric  75 4月  23 11:02 Config.in

~~~  
app-helloworld.mk  
~~~  
################################################################################
#
# helloworld
#
################################################################################

APP_HELLOWORLD_VERSION = 1.0.0
APP_HELLOWORLD_SITE_METHOD:=local
APP_HELLOWORLD_SITE = $(CURDIR)/work/helloworld
APP_HELLOWORLD_INSTALL_TARGET:=YES



define APP_HELLOWORLD_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) all
endef

define APP_HELLOWORLD_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/helloworld $(TARGET_DIR)/bin
endef

define APP_HELLOWORLD_PERMISSIONS
    /bin/helloworld f 4755 0 0 - - - - -
endef

$(eval $(generic-package))
~~~  
Config.in  
~~~  
config BR2_PACKAGE_APP_HELLOWORLD
	bool "helloworld"
	help
	  eric test demo

~~~  
以上，自己的app包已经做好了  
+ 将自己的包，加入到buildroot框架中在package/Config.in文件最后，加入刚才创建的package helloworld  
~~~  
menu "app user"
	source "package/app-helloworld/Config.in"
endmenu
~~~  
+ 查看menuconfig->Target packages，可以看到刚才添加的app user选项  
~~~  
 /home/eric/Documents/work/linux-c/buildroot-2020.02.1/.config - Buildroot 2020.02.1 Configuration
 → Target packages ────────────────────────────────────────────────────────────────────────────────
  ┌────────────────────────────────────── Target packages ──────────────────────────────────────┐
  │  Arrow keys navigate the menu.  <Enter> selects submenus ---> (or empty submenus ----).     │  
  │  Highlighted letters are hotkeys.  Pressing <Y> selects a feature, while <N> excludes a     │  
  │  feature.  Press <Esc><Esc> to exit, <?> for Help, </> for Search.  Legend: [*] feature is  │  
  │  selected  [ ] feature is excluded                                                          │  
  │ ┌─────────↑(-)────────────────────────────────────────────────────────────────────────────┐ │  
  │ │             Compressors and decompressors  --->                                         │ │  
  │ │             Debugging, profiling and benchmark  --->                                    │ │  
  │ │             Development tools  --->                                                     │ │  
  │ │             Filesystem and flash utilities  --->                                        │ │  
  │ │             Fonts, cursors, icons, sounds and themes  --->                              │ │  
  │ │             Games  --->                                                                 │ │  
  │ │             Graphic libraries and applications (graphic/text)  --->                     │ │  
  │ │             Hardware handling  --->                                                     │ │  
  │ │             Interpreter languages and scripting  --->                                   │ │  
  │ │             Libraries  --->                                                             │ │  
  │ │             Mail  --->                                                                  │ │  
  │ │             Miscellaneous  --->                                                         │ │  
  │ │             Networking applications  --->                                               │ │  
  │ │             Package managers  --->                                                      │ │  
  │ │             Real-Time  --->                                                             │ │  
  │ │             Security  --->                                                              │ │  
  │ │             Shell and utilities  --->                                                   │ │  
  │ │             System tools  --->                                                          │ │  
  │ │             Text editors and viewers  --->                                              │ │  
  │ │             app user  --->                                                              │ │  
  │ └─────────────────────────────────────────────────────────────────────────────────────────┘ │  
  ├─────────────────────────────────────────────────────────────────────────────────────────────┤  
  │                  <Select>    < Exit >    < Help >    < Save >    < Load >                   │  
  └─────────────────────────────────────────────────────────────────────────────────────────────┘  
~~~  
进入app user 选项，选择helloworld  
~~~  
 /home/eric/Documents/work/linux-c/buildroot-2020.02.1/.config - Buildroot 2020.02.1 Configuration
 → Target packages → app user ─────────────────────────────────────────────────────────────────────
  ┌───────────────────────────────────────── app user ──────────────────────────────────────────┐
  │  Arrow keys navigate the menu.  <Enter> selects submenus ---> (or empty submenus ----).     │  
  │  Highlighted letters are hotkeys.  Pressing <Y> selects a feature, while <N> excludes a     │  
  │  feature.  Press <Esc><Esc> to exit, <?> for Help, </> for Search.  Legend: [*] feature is  │  
  │  selected  [ ] feature is excluded                                                          │  
  │ ┌─────────────────────────────────────────────────────────────────────────────────────────┐ │  
  │ │         [*] helloworld                                                                  │ │  
  │ │                                                                                         │ │  
  │ └─────────────────────────────────────────────────────────────────────────────────────────┘ │  
  ├─────────────────────────────────────────────────────────────────────────────────────────────┤  
  │                  <Select>    < Exit >    < Help >    < Save >    < Load >                   │  
  └─────────────────────────────────────────────────────────────────────────────────────────────┘  
~~~  
+ 编译 make
可以在目录/output/target/bin，看到自己的代码helloworld已经被编译，并纳入系统中。  
~~~  
eric@eric-PC:~/Documents/work/linux-c/buildroot-2020.02.1/output/target/bin$ ls hello* -l
-rwxr-xr-x 1 eric eric 5180 4月  23 15:41 helloworld

~~~  
+ 重新编译模块，查看详细过程  
~~~  
eric@eric-PC:~/Documents/work/linux-c/buildroot-2020.02.1$ make app-helloworld-rebuild
rm -f /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0/.stamp_rsynced
rm -f /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0/.stamp_staging_installed
rm -f /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0/.stamp_target_installed
rm -f /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0/.stamp_images_installed
rm -f /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0/.stamp_host_installed
rm -f /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0/.stamp_built

>>> app-helloworld 1.0.0 Syncing from source dir /home/eric/Documents/work/linux-c/buildroot-2020.02.1/work/helloworld
rsync -au --chmod=u=rwX,go=rX  --exclude .svn --exclude .git --exclude .hg --exclude .bzr --exclude CVS /home/eric/Documents/work/linux-c/buildroot-2020.02.1/work/helloworld/ /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0
>>> app-helloworld 1.0.0 Building
/usr/bin/make -j5 CC="/home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-gcc" LD="/home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-ld" -C /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0 all
make[1]: 进入目录“/home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0”
make[1]: 对“all”无需做任何事。
make[1]: 离开目录“/home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0”
>>> app-helloworld 1.0.0 Installing to target
/usr/bin/install -D -m 0755 /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/build/app-helloworld-1.0.0/helloworld /home/eric/Documents/work/linux-c/buildroot-2020.02.1/output/target/bin
eric@eric-PC:~/Documents/work/linux-c/buildroot-2020.02.1$
~~~  
+ 可以看到如下过程：
1、>>> app-helloworld 1.0.0 Syncing 复制源码
2、>>> app-helloworld 1.0.0 Building 编译源码
3、>>> app-helloworld 1.0.0 Installing to target 安装应用  

> 注意：buildroot 有一套自己的命名规则，增加自己的package的时候，要严格按照buildroot的规则 修改，可以参考> package目录下的已有的例子





