# BootC 操作记录

突然间接到客户一个新的需求，说是要使用 BootC 来部署新的场景，本来是想使用 CoreMP135 进行部署的。查看BootC的使用文档后，BootC 只支持64位架构，并且



```bash
   1  ifconfig 
    2  touch Containerfile.custom
    6  vi Containerfile.custom 
    7  podman machine init
    8  qemu-img
    9  sudo dnf install qemu-img
   10  podman machine init
   11  podman machine set --rootful
   12  podman machine start
   13  sudo dnf install gvproxy
   14  sudo dnf install podman
   15  podman machine start
   16  sudo podman machine start
   17  podman machine start
   18  ls
   19  wget https://github.com/containers/gvisor-tap-vsock/releases/download/v0.8.7/gvproxy-linux-arm64
   20  ls
   21  chmod +x gvproxy-linux-arm64 
   22  ls
   23  ./gvproxy-linux-arm64 
   24  mv gvproxy-linux-arm64 /usr/lib/podman/gvproxy
   25  sudo chown root:root /usr/lib/podman/gvproxy
   26  ls
   27  ld /usr/local/libexec/podman
   28  ls /usr/local/libexec/podman -lh
   29  ls /usr/local/lib/podman -lh
   30  ls
   31  ls /usr/libexec/podman
   32  sudo cp gvproxy-linux-arm64 /usr/libexec/podman/gvproxy
   33  podman machine start
   34  sudo dnf install qemu-system-aarch64
   35  podman machine start
   36  sudo dnf install virtiofsd
   37  podman machine start
   38  cd /
   39  find -name "virtiofsd"
   40  find -name "virtiofsd" 2>/dev/null
   41  cd -
   42  podman machine start
   43  ls ./usr/libexec/virtiofsd
   44  ./usr/libexec/virtiofsd
   45  ls /usr/libexec/virtiofsd
   46  /usr/libexec/virtiofsd
   47  podman machine start
   48  export PATH=$PATH:/usr/libexec
   49  podman machine start
   50  export DOCKER_HOST='unix:///run/user/1000/podman/podman-machine-default-api.sock'
   51  dock ps
   52  docker ps
   53  podman status
   54  podman stats
   55  podman ps
   56  ps -a
   57  podman ps -a
   58  podman images
   59  ls
   60  podman machine start
   61  podman machine status
   62  podman machine ps
   63  podman ps
   64  podman -c podman-machine-default-root build --cap-add=all --security-opt=label=disable --device /dev/fuse -t localhost/fedora-iot -f Containerfile.custom .
   65  history 
                        


```
第一次运行，报了硬盘空间不足

第二次运行，联网失败
```bash
nihao@MiWiFi-R4A-srv:~$ podman -c podman-machine-default-root build --cap-add=all --security-opt=label=disable --device /dev/fuse -t localhost/fedora-iot -f Containerfile.custom .
ERRO[0823] 1 error occurred:
	* archive/tar: write too long

 
Error: Post "http://d/v5.4.1/libpod/build?addcaps=%5B%22all%22%5D&compatvolumes=0&devices=%5B%22%2Fdev%2Ffuse%22%5D&dockerfile=%5B%22Containerfile.custom%22%5D&forcerm=1&httpproxy=1&identitylabel=1&idmappingoptions=%7B%22HostUIDMapping%22%3Atrue%2C%22HostGIDMapping%22%3Atrue%2C%22UIDMap%22%3A%5B%5D%2C%22GIDMap%22%3A%5B%5D%2C%22AutoUserNs%22%3Afalse%2C%22AutoUserNsOpts%22%3A%7B%22Size%22%3A0%2C%22InitialSize%22%3A0%2C%22PasswdFile%22%3A%22%22%2C%22GroupFile%22%3A%22%22%2C%22AdditionalUIDMappings%22%3Anull%2C%22AdditionalGIDMappings%22%3Anull%7D%7D&isolation=0&jobs=1&labelopts=%5B%22disable%22%5D&layers=1&networkmode=0&nsoptions=%5B%7B%22Name%22%3A%22user%22%2C%22Host%22%3Atrue%2C%22Path%22%3A%22%22%7D%5D&omithistory=0&output=localhost%2Ffedora-iot&outputformat=application%2Fvnd.oci.image.manifest.v1%2Bjson&pullpolicy=missing&retry=3&retry-delay=2s&rm=1&seccomp=%2Fusr%2Fshare%2Fcontainers%2Fseccomp.json&shmsize=67108864&t=localhost%2Ffedora-iot": io: read/write on closed pipe




```



2025年 10月 27日 星期一 15:51:40 CST
我在外网找到了一篇红帽人员写的博客，按照这个博客的说明经过了多次尝试成功在CM4Stack上启动了一个 bootc 的系统镜像。博客描述的步骤和内容要比bootc官方文档好的多，但也并不能完全的执行下来。我是添加了一些自己的内容。[Bootable containers on the Raspberry Pi 4
](https://mrguitar.net/?p=2605)
当然这篇文章是成功执行后才写的，所有中间遇到的困难也只能简单的描述一下。

```txt
BootC 是 red Hat 开发的一种新型容器技术，将整个系统打包成容器用于运行，可以远程进行系统级更新。BootC 依赖 red hat系系统（Fedora、CentOS Stream、RHEL），这些系统主要运行在 64 位平台上。RHEL 收费、CentOS Stream主要用于服务器，Fedora 是比较理想的选择。

BootC的主要工作原理是将容器（podman容器）打包成可启动的镜像，然后进行远程分发。能够进行系统级的回溯和更新，适用于分布式的批量部署和运行维护

```
上面是我向我们芯片厂商提供的描述，但完全让 bootc 工作起来还是经历一饭挫折的。


下载测试了Fedora系统，在CM4Stack 上运行正常，raspberry5上无法运行。  
在CM4Stack上尝试构建bootc系统，3次构建失败.  
转在ubuntu X86主机上构建bootc系统，构建系统，相关指令运行错误。  
通过 kvm 创建了一个 fedora 的虚拟机，运行相关指令，这次成功了。  


作为博客执行的开始，我遵循博客内容，创建了一个 Containerfile，
```bash
FROM quay.io/fedora/fedora-bootc:latest

RUN dnf -y install cockpit cockpit-ws cockpit-podman git vim-enhanced tree && dnf clean all && systemctl enable cockpit.socket

COPY etc etc

ARG sshpubkey

RUN if test -z "$sshpubkey"; then echo "must provide sshpubkey"; exit 1; fi; \
    useradd -G wheel core && \
    mkdir -m 0700 -p /home/core/.ssh && \
    echo $sshpubkey > /home/core/.ssh/authorized_keys && \
    chmod 0600 /home/core/.ssh/authorized_keys && \
    chown -R core: /home/core

RUN set -eu; mkdir -p /usr/ssh && \
    echo 'AuthorizedKeysFile /usr/ssh/%u.keys .ssh/authorized_keys .ssh/authorized_keys2' >> /etc/ssh/sshd_config.d/30-auth-system.conf && \
    echo ${sshpubkey} > /usr/ssh/root.keys && chmod 0600 /usr/ssh/root.keys
```
但是这个配置太浪费时间了，所以我修改了这个配置，仅仅使用了如下的内容:
```bash
FROM quay.io/fedora/fedora-bootc:latest
#COPY files/secret /etc

# 创建 nihao 用户并设置密码
RUN useradd -m -s /bin/bash nihao && \
    echo "nihao:nihao" | chpasswd

```
虽然这个配置节省了大量的时间，但这个配置文件也蕴含了一个巨坑，后面会提到这个坑。

刚开始我按照官方文档中的命令行来操作，后来我突然意识到，博客中提到的 Podman Desktop 貌似是一个 podman 的 ui 操作程序。然后开始下载 Podman Desktop ，事实证明，这确实如博客中提到的那样，Podman Desktop makes life easy。命令行操作对于初次执行的人来说，上手难度还是非常大的。

[podman-desktop](https://podman-desktop.io/)

这个应用下载下来时是一个 flatpak 的安装包，所以还需要安装上 flatpak 以及他的运行环境：
```bash
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub io.podman_desktop.PodmanDesktop
sudo flatpak install ./podman-desktop-1.22.1.flatpak
```

想要使用 podman 执行交叉编译的话，还需要安装下面的依赖：
```bash
sudo dnf install qemu-user-static qemu-aarch64
```

安装 podman-desktop 完成后，能够在系统菜单中找到 podman-desktop 的图标，点击执行他，能看到一个podman的操作界面。有ui操作还是比较开心的。

首先你要在 Containers 下使用Create 创建一个 images，这一步主要是为定制系统服务的。


podman-desktop 中有 bootable Containers 插件，可以在 Extensions 中安装，安装完成后可以使用这个插件来构建 bootc 的镜像。

在 bootable Containers  中，点击 images，然后在想要创建镜像后的后面点击 Build disk Image 按钮，设置相关参数进行构建。

bootable Containers 这个插件运行并不是多么完美，使用几次后，总是创建失败，失败后只能通过重启来解决。至少在我的虚拟机里是这个样子的。

如果你足够幸运，那你将会在输出目录中得到一个 disk.raw 的镜像文件。当然，到这个时候，disk.raw 还是不能直接运行。额，是的，就是这么的恶心。


你要为 disk.raw 安装上启动文件。博客中对这一步进行了说明，但是博客中只是描述了大概的步骤，实际中并不能解决，因为它链目录都是错的。


首先你要将 raspberry4 的启动文件下载下来,在 fedora-bootc 中执行如下操作,就能下载:
```bash
RELEASE=43 # The target Fedora Release. Use the same one that current FCOS is based on.
mkdir -p /tmp/RPi4boot/boot/efi/
dnf download --resolve --releasever=$RELEASE --forcearch=aarch64 --destdir=/tmp/RPi4boot/ uboot-images-armv8 bcm283x-firmware bcm283x-overlays
```
然后将文件复制出来:
```bash
for rpm in /tmp/RPi4boot/*rpm; do rpm2cpio $rpm | cpio -idv -D /tmp/RPi4boot/; done
cp /tmp/RPi4boot/usr/share/uboot/rpi_arm64/u-boot.bin /tmp/RPi4boot/boot/efi/rpi-u-boot.bin
cp /tmp/RPi4boot/boot/* /tmp/RPi4boot/efi -r
e
```
上面并不是我的原始命令,使用时要注意路径.  

解压出来后有两个比较中要的文件,第一个时 boot 目录,这里面是启动文件,第二个是u-boot.bin ,主要在 boot目录下是rpi-u-boot.bin,如果不是这个文件就不启动.

挂载 disk.raw 到 loop 设备,然后将loop100p1挂载,然后将 boot 和 rpi-u-boot.bin 复制进去.

这时候的 disk.raw 才是一个完整的镜像文件,可以烧虑到设备上运行了.


我在添加 boot 文件时试错了.主要文档和博客没有说明好以下问题:

1: disk.raw 中没有启动文件.
2: 启动文件包含 boot中的文件和 rpi-u-boot.bin 他们都是有固定名字的.因为这个镜像中有两个boot分区,所以非常容易弄混.
3: fedora-bootc 太过于干净了,需要进行一个手动配置.


当然，其实还有一个还算比较重要的一步吧，就是定义镜像文件的大小，默认大小是10G，可以烧录到磁盘后手动扩展，也可以直接在创建镜像时指定大小。








记录一下收集到的网页吧：

[Bootable containers on the Raspberry Pi 4](https://mrguitar.net/?p=2605) : 非常重要，制作 BootC 的主要参考文档，其他文档描述的都比较模糊。

[Provisioning Fedora CoreOS on the Raspberry Pi 4](https://docs.fedoraproject.org/en-US/fedora-coreos/provisioning-raspberry-pi4/) : 非常重要，这个和上个文档配和完成的 bootc 的验证的。这个文档属于借花献佛了，可恶的是，这么重要的步骤，bootc 主要文档竟然没有说明。

[Branching Fedora IoT](https://docs.fedoraproject.org/zh_Hans/iot/sop-branching-fedora-iot/) : Fedora IoT 的分支文档 

[Architectures/ARM/Raspberry](https://fedoraproject.org/wiki/Architectures/ARM/Raspberry_Pi/zh-cn) ： Fedora 对树莓派的支持wiki ,内容较老，参考价值不大。

[Fedora Iot Bootc](https://docs.fedoraproject.org/zh_Hans/iot/fedora-iot-bootc-pmachine-example/) : 比较重要把，参考使用的，帮助理解 bootc 的启动需要做什么。

[Automating bootc deployments using git](https://mrguitar.net/?p=2587): 验证时没用到，但是后续客户支持时，应该会用到的。bootc升级源的配置。

[Migrating my Fedora Server to fedora-bootc](https://mrguitar.net/?p=2579) : 感觉像是杂谈，对 bootc 的理解有帮助。

[Adventures with bootc: Upgrading to Fedora 42](https://mrguitar.net/?p=2686) : 可以看看，没用到。

[coreos](https://fedoraproject.org/coreos/): coreos 的官网，只是打开的一个连接而以，没用到。

[Image Builder Bootc User Guide  Developer Guide](https://osbuild.org/docs/user-guide/introduction/): 重在参考。

[bootc-image-builder ](https://osbuild.org/docs/bootc/) : 重在参考。

[https://podman-desktop.io/](https://podman-desktop.io/): podman-desktop 的官网，下载 podman-desktop 的地方。

[Podman 4.3 on Artix Linux: Fix initialization issues](https://scqr.net/en/blog/2023/01/27/podman-43-on-artix-linux-fix-initialization-issues/): 修复某个报错的地方，在 Fedora 中好像没有用。`podman machine start` 出错时在这里找的解决办法。

[How to install Fedora IoT on Raspberry Pi 4](https://www.redhat.com/ja/blog/fedora-iot-raspberry-pi): 写的很好，但是我没提取到非常有用的信息。

[Any bootc users here](https://www.reddit.com/r/Fedora/comments/1hqtrjg/any_bootc_users_here/) : 一些 bootc 的讨论，对理解 bootc 有帮助。

[bootc 的文档](https://bootc-dev.github.io/bootc/intro.html#bootc) : bootc的主要文档，东西太多，比较难懂。

[Learning about Bootc](https://news.ycombinator.com/item?id=43456905) : 看标题。

[Trying to work with bootc - initial attempts](https://brianecole.com/blog/2025-03-27_bootc-initial.html) : 可以参考





