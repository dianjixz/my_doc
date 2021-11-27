 

# [linux loop device介绍](https://www.linuxprobe.com/linux-loop-device-introduction.html)
示例
第一步：使用dd命令创建文件
dd if=/dev/zero of=FS_on_file bs=1k count=10000
第二步：使用losetup命令创建一个loop device
losetup /dev/loop0 FS_on_file
第三步：创建一个文件系统
mkfs -t ext3 /dev/loop0
第四步：挂载这个文件系统
(the mount point '/mnt/FS_file0' has been created before by 'mkdir'):
mount /dev/loop0 /mnt/FS_file0
第五步：若要删除刚才创建的这些对象，依次执行如下步骤：
$ umount /dev/loop0
$ losetup -d /dev/loop0
$ rm FS_on_file
说明
1. 详细的losetup命令如下

[root@linuxprobe ~]# losetup
usage:
losetup loop_device # give info
losetup -d loop_device # delete
losetup [ -e encryption ] [ -o offset ] loop_device file # setup
其中加密选项有如下几种方式：

NONE use no encryption (default).
XOR use a simple XOR encryption.
DES use DES encryption.

DES encryption is only available if the optional DES package has been added to the kernel.
DES encryption uses an additional start value that is used to protect passwords against dictionary attacks.
2. 默认情况下系统支持的loop device是8个

[root@linuxprobe ~]# ls -ltr /dev/loop*
brw-r----- 1 root disk 7, 0 Jul 19 2009 /dev/loop0
brw-r----- 1 root disk 7, 1 Jul 19 2009 /dev/loop1
brw-r----- 1 root disk 7, 2 Jul 19 2009 /dev/loop2
brw-r----- 1 root disk 7, 3 Jul 19 2009 /dev/loop3
brw-r----- 1 root disk 7, 4 Jul 19 2009 /dev/loop4
brw-r----- 1 root disk 7, 5 Jul 19 2009 /dev/loop5
brw-r----- 1 root disk 7, 6 Jul 19 2009 /dev/loop6
brw-r----- 1 root disk 7, 7 Jul 19 2009 /dev/loop7
如果需要超过8个loop device，那么使用losetup命令的时候可能会遇到类似的错误 'no such device',这是因为超过了可用loop device设备的最大限制，依据你的Linux系统，可以通过修改 /etc/modprobe.conf 配置文件，增加如下参数的方式进行扩展

options loop max_loop=20 --比如我增加到20个
如果要马上生效的话，可以执行以下命令立即加载该模块。

modprobe -v loop
[root@linuxprobe ~]# cat /etc/modprobe.conf|grep loop
options loop max_loop=20

[root@linuxprobe ~]# modprobe -v loop
insmod /lib/modules/2.6.9-42.0.0.0.1.ELsmp/kernel/drivers/block/loop.ko max_loop=20
[root@linuxprobe ~]# ls -ltr /dev/loop*
brw-rw---- 1 root disk 7, 8 Jul 19 07:44 /dev/loop8
brw-rw---- 1 root disk 7, 9 Jul 19 07:44 /dev/loop9
brw-rw---- 1 root disk 7, 10 Jul 19 07:44 /dev/loop10
brw-rw---- 1 root disk 7, 11 Jul 19 07:44 /dev/loop11
brw-rw---- 1 root disk 7, 12 Jul 19 07:44 /dev/loop12
brw-rw---- 1 root disk 7, 13 Jul 19 07:44 /dev/loop13
brw-rw---- 1 root disk 7, 14 Jul 19 07:44 /dev/loop14
brw-rw---- 1 root disk 7, 15 Jul 19 07:44 /dev/loop15
brw-rw---- 1 root disk 7, 16 Jul 19 07:44 /dev/loop16
brw-rw---- 1 root disk 7, 17 Jul 19 07:44 /dev/loop17
brw-rw---- 1 root disk 7, 18 Jul 19 07:44 /dev/loop18
brw-rw---- 1 root disk 7, 19 Jul 19 07:44 /dev/loop19
brw-rw---- 1 root disk 7, 0 Jul 19 2009 /dev/loop0
brw-rw---- 1 root disk 7, 1 Jul 19 2009 /dev/loop1
brw-rw---- 1 root disk 7, 2 Jul 19 2009 /dev/loop2
brw-rw---- 1 root disk 7, 3 Jul 19 2009 /dev/loop3
brw-rw---- 1 root disk 7, 4 Jul 19 2009 /dev/loop4
brw-rw---- 1 root disk 7, 5 Jul 19 2009 /dev/loop5
brw-rw---- 1 root disk 7, 6 Jul 19 2009 /dev/loop6
brw-rw---- 1 root disk 7, 7 Jul 19 2009 /dev/loop7
有了这个东西,在Linux下就可以借助file来测试学习ASM了。





losetup -P /dev/loop404 test.img

挂载img文件设备

losetup -d /dev/loop404  

卸载设备