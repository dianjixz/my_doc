文章目录

    小荣的日常记录
    发生了什么
        第一步，查看靶场glibc版本
        第二步，配置all in one环境
        第三步，安装配置patchelf
        下载配置特定版本glibc
        编译

发生了什么

当我兴致勃勃的准备内核提权时，发现glibc版本不对
在这里插入图片描述

真让人生气
第一步，查看靶场glibc版本

ldd --version

    1

在这里插入图片描述
第二步，配置all in one环境

下载glibc-all-in-one

proxychains4 -f /etc/proxychains4.conf git clone https://github.com/matrix1001/glibc-all-in-one.git

    1

在这里插入图片描述
更新列表

cd glibc-all-in-one/
python3 update_list

    1
    2

在这里插入图片描述
看看能安装啥

cat list

    1

只截取了一部分

在这里插入图片描述
第三步，安装配置patchelf

照抄，没有原因

git clone https://github.com/NixOS/patchelf.git
cd patchelf
./bootstrap.sh 
./configure
make
make check
sudo make install

    1
    2
    3
    4
    5
    6
    7

在这里插入图片描述这样的结尾说明一切正常
下载配置特定版本glibc

cat  list
我们这个靶场需要64位2.28
没有2.28
下个低一个版本的
./download 2.27-3ubuntu1_amd64

    1
    2
    3
    4
    5

在这里插入图片描述
编译

核心，精华
编译方式

gcc -Wl,-rpath='/root/glibc-all-in-one/libs/2.27-3ubuntu1_amd64',-dynamic-linker='/root/glibc-all-in-one/libs/2.27-3ubuntu1_amd64/ld-linux-x86-64.so.2' -s 1.c -o haha

    1

注意这一段，要求是绝对路径
/root/glibc-all-in-one/libs/2.27-3ubuntu1_amd64这个路径和下面这个应该不用说了，注意是ld-linux-x86-64.so.2文件，对应下一个参数
/root/glibc-all-in-one/libs/2.27-3ubuntu1_amd64/ld-linux-x86-64.so.2
剩下的参数和正常的一样

-Wl,-rpath='/root/glibc-all-in-one/libs/2.27-3ubuntu1_amd64',-dynamic-linker='/root/glibc-all-in-one/libs/2.27-3ubuntu1_amd64/ld-linux-x86-64.so.2'

    1

如果需要其他版本的glibc也可以
不过我还没弄好，但是已经可以正常执行了，不会报错了
在这里插入图片描述
应该是sh的调用问题
不过环境已经好了
还有一种简单的方法就是
直接安装一个2018年的kali
什么都解决了
不会有任何问题
————————————————
版权声明：本文为CSDN博主「人间体佐菲」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_65527369/article/details/127973141