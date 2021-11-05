# 搭建ubuntu20.04编译环境

## 安装编译依赖包

~~~bash
sudo apt-get install build-essential subversion git-core libncurses5-dev zlib1g-dev flex quilt libssl-dev xsltproc libxml-parser-perl mercurial bzr ecj cvs unzip lib32z1 lib32z1-dev lib32stdc++6 libstdc++6 rsync busybox -y
~~~

## 降级gawk

直接使用`sudo apt install gawk`安装的是5.0版本，编译SDK时不能使用5.0版本，所以我们要进行降级。

> 编译gawk时需要用高版本gcc/g++，如果已经进行gcc/g++降级了，可以使用`sudo update-alternatives --config gcc  `配置高版本gcc和g++，编译完成后再降回来。

~~~ bash
#!/bin/bash
wget https://mirrors.ustc.edu.cn/ubuntu/pool/main/g/gawk/gawk_4.1.3%2Bdfsg.orig.tar.xz
#下载源码
tar xvf gawk_4.1.3+dfsg.orig.tar.xz
cd gawk-4.1.3
#进入源码目录
mkdir build
#创建编译目录
cd build
#进入编译目录
../configure
#检查环境
make
#编译,此时编译出错不用担心，查看当前文件夹是否有gawk二进制文件产生。如果有，就可以进行下一步
sudo make install
#安装
gawk -V
#查看版本
~~~


## 降级gcc和g++

编译sdk里面glibc的时候gcc版本过高，会导致glibc的编译报错，我们可以通过降级gcc来解决这个问题。

~~~ bash
#!/bin/bash
sudo echo "deb http://dk.archive.ubuntu.com/ubuntu/ xenial main universe" >> /etc/apt/sources.list
#添加低版本的gcc源
sudo apt update
#更新源
sudo apt-get install -y gcc-5
sudo apt-get install -y g++-5
#安装gcc-5 g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 100
#配置版本
gcc -v
~~~

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 100  
最后的数字为优先级(越大越高),因为只有一个5版本作为alternatives,可以不要纠结数字,这样设就行了
然后可以输入以下命令查看设置结果(非必须)  
sudo update-alternatives --config gcc  

同样的g++也一样。  

当以上设置不需要的时候输入以下命令删除:

sudo update-alternatives --remove gcc /usr/bin/gcc-5

