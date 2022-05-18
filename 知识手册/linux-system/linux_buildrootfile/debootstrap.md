1. 

## 1. debootstrap

### 1.1 简介

debootstrap是debian/ubuntu下的一个工具，用来构建一套基本的系统(根文件系统)。生成的目录符合Linux文件系统标准(FHS)，即包含了/boot、/etc、/bin、/usr等等目录，但它比发行版本的Linux体积小很多，当然功能也没那么强大，因此只能说是“基本的系统”。

### 1.2 安装

```text
sudo apt-get install debootstrap
```

### 1.3 使用方式

```text
# 可加参数指定源
sudo debootstrap --arch [平台] [发行版本代号] [目录] [源]
```

**如下例说明：**

```text
sudo debootstrap --arch amd64 buster ./ [这里可以方源的http链接,不放则使用默认]
# 即是构建amd64平台debian最新发行版debian10(代号为buster)的基本系统，存放到当前目录下。
```

当前debootstrap支持的发行版本可以在/usr/share/debootstrap/scripts查看，补充两个版本代号信息相关的链接有兴趣可以了解一下，版本代号网上都可以查到。

[形形色色的Linux 发行版代号都在这里](https://link.zhihu.com/?target=https%3A//blog.csdn.net/snow2know/article/details/53090885)

[ubuntu部分版本号的含义](https://link.zhihu.com/?target=https%3A//www.cnblogs.com/sirius-xu/archive/2013/03/05/2943952.html)

输入上述命令后，就会从网络下载相关的文件；

```ini
# 此句收尾代表成功
I: Base system installed successfully.
```

## 2. deb源码包下载和编译

### 2.1 下载

需在source.list里配好deb-src的源

```text
apt-get source [name] # 下载并解包
apt-get source -d [name] # 下载源码包，不解包
dpkg-source -x [xx.dsc] # 解包
```

### 2.2 编译

编译成功会在上层目录生成deb包

```text
apt-get build-dep [name] # 安装指定包编译所需依赖
dpkg-buildpackage # 编译all
dpkg-buildpackage -b -aamd64  # 只编译指定架构

# dpkg-buildpackage部分参数解析
-D 检查依赖性
-d 不检查依赖和冲突
-nc 清理源码树
dpkg-buildpackage -nc 2>&1 | tee ../mk.log # 收集信息输出log
```

## 3. 用debootstrap构建编译环境

### 3.1 amd64架构debian10案例

```text
# 创建环境进入
mkdir buster&cd buster
sudo debootstrap --arch amd64 buster ./
sudo chroot .

# 修改云源
echo deb http://mirrors.ustc.edu.cn/debian buster main > /etc/apt/sources.list
echo deb-src http://mirrors.ustc.edu.cn/debian buster main >> /etc/apt/sources.list
apt-get update

# 部分版本是没有装编译工具包的
apt-get install dpkg-dev
apt-get install debhelper

# 下载编译源码
cd /tmp
apt-get source redis
apt-get build-dep redis
cd redis.x.x
dpkg-buildpackage
find ../*.deb
```



发布于 2021-08-23 13:58