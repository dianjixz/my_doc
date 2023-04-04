 

# Linux设备树详解(一) 基础知识

https://blog.csdn.net/u012489236/article/details/97137007



# 深入浅出DDR系列(一)--DDR原理篇

https://blog.csdn.net/u012489236/article/details/107730731



# 深入浅出DDR系列(二)--DDR工作原理

https://blog.csdn.net/u012489236/article/details/107750942



# Linux设备树详解(三)u-boot设备树的传递

https://blog.csdn.net/u012489236/article/details/97256928



# Linux设备树详解(四)kernel的解析

https://blog.csdn.net/u012489236/article/details/97271797


https://www.celinuxforum.org/                                           #CE Linux Forum


eLinux.org 是 Linux 基金会下属 Consumer Electronics Linux Forum 维护的一个 Embedded Linux Wiki。该 Wiki 全面系统地梳理了嵌入式 Linux 方方面面的知识。

ChinaUnix博客   http://blog.chinaunix.net/uid-31087949-id-5760669.html         描述linux wifi sdio设备驱动



https://www.kernel.org/doc/html/v4.16/process/howto.html


安装linux-headers

CentOS/RedHat 系统

sudo yum -y install kernel-headers  //安装kernel-headers

sudo yum -y install kernel-devel    //安装kernel-devel

    1
    2
    3

Ubuntu/Debian系统

sudo apt-get install build-essential  //install build-essential(optional)

sudo apt-get update                  //install linux-headers
sudo apt-get install linux-headers-$(uname -r)

sudo apt-get update && sudo apt-get install build-essential linux-headers-$(uname -r)
————————————————
版权声明：本文为CSDN博主「miaoyanmm」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/miaoyanmm/article/details/79853297