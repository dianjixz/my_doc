本文同步自我自己的公众号（Chrithon的杂货铺），有改动

近期在捣鼓韦东山大佬的Buildroot，在配置Buildroot的时候发现没有RTL8723DS驱动，怎么办呢？

于是我分析了下Buildroot的文件结构，发现package下面有软件包选项相关的配置文件：
package文件夹

PS：请先提前准备好韦东山大佬的的Buildroot（见下图），如果没有，请自行git clone

git clone https://gitee.com/weidongshan/neza-d1-buildroot/

首先我们找一个非firmware的网卡驱动包，复制下来（我复制的是rtl8189fs），然后重命名为rtl8723ds。

进入该文件夹，然后文件名上rtl8189fs的全部替换成rtl8723ds，然后把不相关的文件删除，如图所示：
rtl8723ds文件夹

接下来就是修改配置文件，把Config.in和rtl8723ds.mk里的rtl8189fs全部替换成rtl8723ds(请注意大小写)。如图所示：
rtl8723ds.mk
Config.in

请注意：我修改的rtl8723ds驱动包已经上传到Gitee上，如果你使用的是Github上的版本，需按照阅读原文中的修改“os_intfs.c”和“rtw_cfgvendor.c”。

返回上一级，找到Config.in，找到Hardware handling，然后在这下面找到如下内容：

source "package/rtl8723bu/Config.in"

添加如下内容：

source "package/rtl8723ds/Config.in"

保存，然后运行以下命令：

make neza-d1_defconfig

make make menuconfig

找到Hardware handling那一栏，然后勾上rtl8723ds：
menuconfig->Target Package->Hardware handling

（至于其他包的话，可根据需要自行选择，我这里就勾选了网络相关的软件包）

保存退出，执行：

make all

等固件被编译出来后就可以把该固件烧录到SD卡或者板载存储器里（在此教程中，我就直接用编译好的固件来演示）。
编译成功
烧录到SD卡上
烧录成功

最终效果：
最终效果

 作者：老鸽子Chrithon https://www.bilibili.com/read/cv15053865 出处：bilibili





