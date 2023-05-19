下载源码, stress-ng版本有好几个版本，不同开源仓库差别并不太大

https://github.com/ColinIanKing/stress-ng


解压并进入目录

tar xzvf stress-ng-0.15.05.tar.gz


设置环境变量

export CC=aarch64-linux-gnu-gcc

本次 aarch64-linux-gnu-gcc 进行举例，实际设置成你自己的交叉编译工具即可。

交叉编译

make

如果提示没有aarch64-linux-gnu-gcc的话，如下

使用find命令查找下交叉编译工具

find / -name "aarch64-linux-gnu-gcc"

加入当前路径，即可进行编译工作

 export PATH=$PATH:/root/.iso_compiler/v2/gcc/aarch64-9.3/bin/

 

将编译得到的程序 stress-ng 放入目标板中，即可。

 
缺少 xxx 库，错误如下：
stress-ng: error while loading shared libraries: libatomic.so.1: cannot open shared object file: No such file or directory

解决方法：从交叉编译工具的目录中，找到对应的库，copy到相应路径export出来即可。

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
————————————————
版权声明：本文为CSDN博主「arvin_xiaoting」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/xiaoting451292510/article/details/129584892