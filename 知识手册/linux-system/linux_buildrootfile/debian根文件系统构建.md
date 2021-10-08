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