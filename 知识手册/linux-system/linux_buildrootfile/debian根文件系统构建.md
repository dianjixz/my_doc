首先你先新建一个用于mount的目录

mkdir /debian9

其次你需要保证主机上有 debootstrap 以及 qemu-user-static

apt-get -y install debootstrap qemu-user-static

现在你需要dd命令去生成rootfs.img文件

dd if=/dev/zero of=rootfs.img bs=1M count=1024

dd命令的话。bs代表一次读入写入大小。所以rootfs.img的总大小是1M*1024=1G

用mkfs.ext4把img变成ext4文件格式,并且保证页面大小是4k也就是4096

mkfs.ext4 -b 4096 rootfs.img

然后把 rootfs.img mount 到 刚刚新建的文件夹下

mount rootfs.img /debian9 -o loop

cd /debian9

在文件夹里下载debian9

debootstrap --foreign --arch armhf stretch . http://ftp.cn.debian.org/debian

#debootstrap --foreign --arch amd64 xenial .  https://mirrors.tuna.tsinghua.edu.cn/ubuntu/

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

======================

做完以后推出chroot

exit

umount 一下img

cd

umount /debian9

对img进行校验

e2fsck -p -f linuxroot.img

对img大小进行重新设定

resize2fs  -M linuxroot.img

搞定了
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

