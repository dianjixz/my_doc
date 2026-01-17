首先你先新建一个用于mount的目录

mkdir debian9

其次你需要保证主机上有 debootstrap 以及 qemu-user-static

apt-get -y install debootstrap qemu-user-static

现在你需要dd命令去生成rootfs.img文件

dd if=/dev/zero of=rootfs.img bs=1M count=1024

dd命令的话。bs代表一次读入写入大小。所以rootfs.img的总大小是1M*1024=1G

用mkfs.ext4把img变成ext4文件格式,并且保证页面大小是4k也就是4096

mkfs.ext4 -b 4096 rootfs.img

然后把 rootfs.img mount 到 刚刚新建的文件夹下

sudo mount rootfs.img ./debian9 -o loop

cd ./debian9

在文件夹里下载debian9

debootstrap --foreign --arch armhf stretch . http://ftp.cn.debian.org/debian

#debootstrap --foreign --arch amd64 xenial .  https://mirrors.tuna.tsinghua.edu.cn/ubuntu/

#debootstrap --foreign --arch armhf bookworm .  https://mirrors.ustc.edu.cn/debian/


cp qemu到usr/bin

cp /usr/bin/qemu-arm-static usr/bin/

LC_ALL=C LANGUAGE=C LANG=C chroot . /debootstrap/debootstrap --second-stage

LC_ALL=C LANGUAGE=C LANG=C chroot . dpkg --configure -a

设置root密码

chroot . 

passwd

=====================

此外你可以下载一些需要的软件

apt install vim git ....(根据自己需求添加)

=====================

添加用户

useradd -s '/bin/bash' -m -G adm,sudo maze

passwd maze




`useradd` 是一个用于在 Unix-like 操作系统中创建新用户的命令。您提供的命令 `useradd -s '/bin/bash' -m -G adm,sudo maze` 具有以下功能：

1. `-s '/bin/bash'`: 该选项指定用户的默认登录 shell 为 `/bin/bash`，也就是 Bash shell。Bash 是一种常见的命令行 shell，允许用户与系统进行交互，运行命令等。

2. `-m`: 该选项表示在创建用户时，同时创建用户的家目录。用户的家目录通常位于 `/home` 目录下，这样用户可以在家目录中存储其文件和配置。

3. `-G adm,sudo`: 该选项将新用户添加到两个用户组：`adm` 和 `sudo`。用户组是一种组织用户的方式，用户可以共享组中的权限和资源。`adm` 组通常与系统日志相关，而 `sudo` 组允许用户执行超级用户（root）权限的操作。

4. `maze`: 这是新用户的用户名。

综合起来，该命令的功能是创建一个名为 `maze` 的新用户，将其登录 shell 设置为 Bash，创建其家目录，并将其添加到 `adm` 和 `sudo` 用户组，以便该用户在系统中具有管理员权限和访问日志等功能。请注意，执行这样的操作可能需要超级用户权限（root 权限）。








======================

做完以后推出chroot

exit

umount 一下img

cd

umount ./debian9

对img进行校验

e2fsck -p -f rootfs.img

对img大小进行重新设定

resize2fs  -M rootfs.img

搞定了

~~~ bash
#!/bin/bash
if [ -d ./debian9 ] ; then
echo "find dir!"
else
mkdir ./debian9
fi
dd if=/dev/zero of=rootfs.img bs=1M count=2048
mkfs.ext4 -b 4096 rootfs.img
sudo mount rootfs.img ./debian9 -o loop
cd ./debian9


sudo debootstrap --foreign --arch riscv64 sid . file:///home/nihao/wd1T/riscv-base/mirror/ftp.ports.debian.org/debian-ports
sudo cp /usr/bin/qemu-arm-static usr/bin/
sudo LC_ALL=C LANGUAGE=C LANG=C chroot . /debootstrap/debootstrap --second-stage
sudo LC_ALL=C LANGUAGE=C LANG=C chroot . dpkg --configure -a

sudo mount /home/nihao/wd1T/riscv-base /home/nihao/work/debian9/mnt -o bind
sudo mount /tmp /home/nihao/work/debian9/tmp -o bind

sudo chroot . 





sudo umount /home/nihao/work/debian9/tmp
sudo umount /home/nihao/work/debian9/mnt
cd /home/nihao/work
sudo umount ./debian9
e2fsck -p -f rootfs.img
resize2fs  -M rootfs.img


~~~

~~~ bash
#!/bin/bash
echo "deb file:///mnt/mirror/ftp.ports.debian.org/debian-ports sid main" >> /etc/apt/sources.list

sudo apt -o Acquire::AllowInsecureRepositories=true -o Acquire::AllowDowngradeToInsecureRepositories=true update

apt -o APT::Get::AllowUnauthenticated=true install gnupg -y

cat /mnt/mirror/archive_2022.key | apt-key add -
apt update

apt remove vim-common
apt install vim

~~~





————————————————
版权声明：本文为CSDN博主「maze_linux」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u012839187/article/details/78092998



Linux e2fsck命令用于检查使用 Linux ext2 档案系统的 partition 是否正常工作。

```bash
e2fsck [-pacnydfvFV] [-b superblock] [-B blocksize] [-l|-L bad_blocks_file] [-C fd] device
```

**参数说明**：

- device ： 预备检查的硬盘 partition，例如：/dev/sda1
- -a : 对 partition 做检查，若有问题便自动修复，等同 -p 的功能
- -b : 设定存放 superblock 的位置
- -B : 设定单位 block 的大小
- -c : 检查该partition 是否有坏轨
- -C file : 将检查的结果存到 file 中以便查看
- -d : 列印 e2fsck 的 debug 结果
- -f : 强制检查
- -F : 在开始检查前，将device 的 buffer cache 清空，避免有错误发生
- -l bad_blocks_file : 将有坏轨的block资料加到 bad_blocks_file 里面
- -L bad_blocks_file : 设定坏轨的block资料存到 bad_blocks_file 里面，若无该档则自动产生
- -n : 将档案系统以[唯读]方式开启
- -p : 对 partition 做检查，若有问题便自动修复
- -v : 详细显示模式
- -V : 显示出目前 e2fsck 的版本
- -y : 预先设定所有检查时的问题均回答[是]



需要安装的软件包:

~~~ bash
network-manager wpasupplicant openssh-server vim tree net-tools 
~~~







# riscv64系统rootfs制作说明

https://wiki.debian.org/RISC-V#OS_.2F_filesystem_images







nohub /home/xiao/nihao.sh &

lib/ext2fs/ismounted.c:11:**#include<sys/sysmacros.h>** 
lib/blkid/devname.c:15:**#include<sys/sysmacros.h>**



在系统的终端处修改打印级别，例如，屏蔽所有的内核printk打印，那么只需要将第一个数值调到最小值1或0即可
~~~ bash
# echo 1 4 1 7 > /proc/sys/kernel/printk
or

# echo 0 4 0 7 > /proc/sys/kernel/printk
~~~

Load the module by running:

depmod -a && modprobe awusb

depmod -a -b pwd 5.15.84

```shell
sudo nmcli dev wifi connect XXX password XXX wep-key-type key ifname wlan0
```


debian和ubuntu的基本底包下载链接：
https://rcn-ee.com/rootfs/eewiki/minfs/
http://cdimage.ubuntu.com/ubuntu-base/releases
直接下载底包，免于重新从头构建。

https://wiki.t-firefly.com/zh_CN/Firefly-Linux-Guide/first_use.html#ubuntu-minimal-xi-tong