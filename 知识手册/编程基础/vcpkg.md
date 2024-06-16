# vcpkg

## 介绍
vcpkg 是一个开源的C++库包管理工具，由Microsoft开发和维护。它旨在简化C++开发人员在Windows、Linux和macOS等操作系统上使用第三方库的过程。通过vcpkg，您可以轻松地安装和管理各种C++库，而无需手动下载和配置。vcpkg提供了一个庞大的库目录，包括常用的开源库，如Boost、OpenCV、OpenGL以及各种C++工具库。您可以使用vcpkg进行库的安装、卸载和更新，并与您的C++项目无缝集成。它使得在C++开发中使用第三方库变得更加方便、快捷和可靠。


## 安装
```bash
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg --help



```

## 使用

```bash
# 下载包，并编译
./vcpkg install opencv --only-downloads

# 对下载好的包继续编译
./vcpkg install opencv

# 列出已经安装的库
./vcpkg list

# 搜索库
./vcpkg search <lib>

# 带版本安装库
./vcpkg install <lib>:<arch>
# ./vcpkg install thrift:x86-windows
# ./vcpkg install thrift:x86-windows-static


# 查看支持的平台
vcpkg help triplet

```

## 注意
vcpkg 默认是安装32位的库，我是需要安装 x64 的库，因此需要添加一个系统变量：
```bash
VCPKG_DEFAULT_TRIPLET=x64-windows
```


## 使用技巧
### 直接手动下载不好下载的包
在 VCPKG 下载包的时候，总是会碰到下载突然卡住，其中一个包下载不下来的情况，直接手动去下载(如下图框出的链接地址)下来，然后放在 C:\src\vcpkg\downloads 下面，重新再次执行下载命令即可。
![](https://pic3.zhimg.com/80/v2-e3d7d3f1fd1a267d314582175624d96e_720w.webp)