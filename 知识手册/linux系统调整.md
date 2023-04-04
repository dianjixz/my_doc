ubuntu-创建桌面快捷方式
====================
第一步：/usr/share/applications目录下创建桌面配置文件mytest.desktop

``` bash
[Desktop Entry]
Name=mytest //应用程序名称
GenericName=mytest //应用程序通用名称
Comment=Qt mytest //应用程序简要描述
Exec=/usr/bin/untitled //可执行文件绝对路径
Terminal=flase //是否开启一个终端执行
Type=Application //快捷方式类型为应用
Icon=/home/forlinx/imgs/rtc-icon.png //图标文件绝对路径
Encoding=UTF-8 //编码格式
```

第二步：将可执行文件拷贝到/usr/bin目录下
第三步：将图标文件拷贝到/home/forlinx/imgs目录下
第四步：将/usr/bin/applications目录下图标拷贝到桌面
————————————————
版权声明：本文为CSDN博主「Paper_Love」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_42952079/article/details/125381405