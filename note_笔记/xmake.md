关于 xmake 的安装都已经写的很好了，在此只说一下 xmake 常用生成文件中没有的东西。

比如我们期望编译 opencv 工程，我们需要在项目描述中添加 add_packages
``` launch

target("find_line1")
    set_kind("binary")
    add_files("find_line.cpp")
    add_packages("opencv")

```