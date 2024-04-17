使用的 stm32mp1 的一些笔记
=======================





stm32mp1 启动是从特定地址的分区启动的，烧写系统时需要对 sd 卡进行分区。
``` bash
sudo sgdisk --resize-table=128 -a 1 -n 1:34:545 -c 1:fsbl1 -n 2:546:1057 -c 2:fsbl2 -n 3:1058:5153 -c 3:ssbl -n 4:5154:136225 -c 4:bootfs -n 5:136226 -c 5:rootfs -A 4:set:2 -p /dev/sdb -g

```

```bash
sudo sgdisk --resize-table=128 -a 1 \
-n 1:34:545             -c 1:fsbl1 \
-n 2:546:1057           -c 2:fsbl2 \
-n 3:1058:1569          -c 3:fsbl3 \
-n 4:1570:2081          -c 4:fsbl4 \
-n 5:2082:10273         -c 5:fsbl5 \
-n 6:10274:18465        -c 6:fsbl6 \
-n 7:18466:19489        -c 7:fsbl7 \
-n 8:19490:150561       -c 8:boot \
-n 9:150562:183329      -c 9:vendorfs \
-n 10:183330:9253821    -c 10:rootfs \
-g  /dev/sdb






sudo sgdisk -og -a 1 \
-a 1 -n 1:34:545 -c 1:fsbl1 -t 1:8301 \
-a 1 -n 2:546:1057 -c 2:fsbl2 -t 2:8301 \
-a 1 -n 3:1058:1569 -c 3:metadata1 -t 3:8301 \
-a 1 -n 4:1570:2081 -c 4:metadata2 -t 4:8301 \
-a 1 -n 5:2082:10273 -c 5:fip-a -t 5:19d5df83-11b0-457b-be2c-7559c13142a5  -u 5:4fd84c93-54ef-463f-a7ef-ae25ff887087 \
-a 1 -n 6:10274:18465 -c 6:fip-b -t 6:19d5df83-11b0-457b-be2c-7559c13142a5  -u 6:09c54952-d5bf-45af-acee-335303766fb3 \
-a 1 -n 7:18466:19489 -c 7:u-boot-env -t 7:8301 \
-a 1 -n 8:19490:150561 -c 8:bootfs -t 8:8300  -A 8:set:2 \
-a 1 -n 9:150562:183329 -c 9:vendorfs -t 9:8300 \
-a 1 -n 10:183330:  -c 10:rootfs -t 10:8300  -u 10:e91c4e10-16e6-4c0e-bd0e-77becf4a3582 \
/dev/sda
sudo mkfs.ext4 /dev/sda10
sudo mkfs.ext4 /dev/sda8





# test
dd if=/dev/zero of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw bs=1024 count=0 seek=1536K
sgdisk -og -a 1 FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 1:34:545 -c 1:fsbl1 -t 1:8301  FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 2:546:1057 -c 2:fsbl2 -t 2:8301  FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 3:1058:1569 -c 3:metadata1 -t 3:8301  FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 4:1570:2081 -c 4:metadata2 -t 4:8301  FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 5:2082:10273 -c 5:fip-a -t 5:19d5df83-11b0-457b-be2c-7559c13142a5  -u 5:4fd84c93-54ef-463f-a7ef-ae25ff887087 FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 6:10274:18465 -c 6:fip-b -t 6:19d5df83-11b0-457b-be2c-7559c13142a5  -u 6:09c54952-d5bf-45af-acee-335303766fb3 FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 7:18466:19489 -c 7:u-boot-env -t 7:8301  FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 8:19490:150561 -c 8:bootfs -t 8:8300  -A 8:set:2 FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 9:150562:183329 -c 9:vendorfs -t 9:8300  FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -a 1 -n 10:183330:  -c 10:rootfs -t 10:8300  -u 10:e91c4e10-16e6-4c0e-bd0e-77becf4a3582 FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk -p FlashLayout_sdcard_stm32mp135f-dk-extensible.raw
sgdisk FlashLayout_sdcard_stm32mp135f-dk-extensible.raw -i 5
sgdisk FlashLayout_sdcard_stm32mp135f-dk-extensible.raw -i 6
sgdisk FlashLayout_sdcard_stm32mp135f-dk-extensible.raw -i 10
dd if=arm-trusted-firmware/tf-a-stm32mp135f-dk-sdcard.stm32 of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw conv=fdatasync,notrunc seek=1 bs=17408
dd if=arm-trusted-firmware/tf-a-stm32mp135f-dk-sdcard.stm32 of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw conv=fdatasync,notrunc seek=1 bs=279552
dd if=arm-trusted-firmware/metadata.bin of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw conv=fdatasync,notrunc seek=1 bs=541696
dd if=arm-trusted-firmware/metadata.bin of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw conv=fdatasync,notrunc seek=1 bs=803840
dd if=fip/fip-stm32mp135f-dk-optee.bin of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw conv=fdatasync,notrunc seek=1 bs=1065984
dd if=st-image-bootfs-openstlinux-weston-stm32mp1.ext4 of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw conv=fdatasync,notrunc seek=1 bs=9978880
dd if=st-image-vendorfs-openstlinux-weston-stm32mp1.ext4 of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw conv=fdatasync,notrunc seek=1 bs=77087744
dd if=st-image-weston-openstlinux-weston-stm32mp1.ext4 of=FlashLayout_sdcard_stm32mp135f-dk-extensible.raw conv=fdatasync,notrunc seek=1 bs=93864960


sudo e2label  /dev/sda10 rootfs
sudo dosfslabel /dev/sda8 boot




sudo sgdisk -P /dev/sdb --resize-table=128 -a 1

-n 1:34:545 -c 1:fsbl1 
-n 2:546:1057 -c 2:fsbl2 
-n 3:1058:1569 -c 3:ssb3 
-n 4:1570:2081 -c 4:bootfs 
-n 5:136226 -c 5:rootfs 
-A 4:set:2 -p /dev/sdb -g




```
sgdisk：

保存分区表：
sudo sgdisk --backup=partition_table_backup.bin /dev/sda

恢复的分区表：
sudo sgdisk --load-backup=partition_table_backup.bin /dev/sda


sfdisk：
保存分区表：
sudo sfdisk -d /dev/sdX > partition_table_backup.txt
or
sudo parted /dev/sdX print > partition_table_backup.txt






恢复的分区表：
sudo sfdisk /dev/sdX < partition_table_backup.txt
or
sudo parted /dev/sdX --script < partition_table_backup.txt




Disk /dev/sdb：14.84 GiB，15931539456 字节，31116288 个扇区
Disk model: Mass-Storage    
单元：扇区 / 1 * 512 = 512 字节
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节
磁盘标签类型：gpt
磁盘标识符：05E429DD-ADD8-425C-8212-E6E5CAE8B5CA

设备          起点    末尾    扇区   大小 类型
/dev/sdb1       34     545     512   256K Linux 保留            fsbl1
/dev/sdb2      546    1057     512   256K Linux 保留            fsbl2
/dev/sdb3     1058    1569     512   256K Linux 保留            metadata1
/dev/sdb4     1570    2081     512   256K Linux 保留            metadata2
/dev/sdb5     2082   10273    8192     4M 未知                  fipa
/dev/sdb6    10274   18465    8192     4M 未知                  fipb
/dev/sdb7    18466   19489    1024   512K Linux 保留            u-boot-env
/dev/sdb8    19490  150561  131072    64M Linux 文件系统         bootfs
/dev/sdb9   150562  183329   32768    16M Linux 文件系统         vendorfs
/dev/sdb10  183330 1690657 1507328   736M Linux 文件系统         rootfs
/dev/sdb11 1690658 3145694 1455037 710.5M Linux 文件系统         userfs








debian 系开机自动扩容：
起用 resize-helper.service 开机服务，该服务会检测磁盘是否完全使用了磁盘，如果没有就扩展根文件系统到整个磁盘。
```bash
systemctl enable resize-helper
```








|   fsbl1  |  fsbl2  |  metadata1  |  metadata2  |  fipa   |  fipb  |  u-boot-env   |  bootfs  |  vendorfs  |   rootfs  |  userfs |
|   -----  |  -----  |  ---------  |  ---------  |  ----   |  ----  |  ----------   |  ------  |  --------  |   ------  |  ------ |
|   256K   |  256K   |  256K       |  256K       |  4M     |  4M    |  512K         |   64M    |    16M     |   736M    |  710.5M |


``` bash
dd if=tf-a-sdcard.stm32 of=/dev/mmcblk0p1 conv=fdatasync
dd if=tf-a-sdcard.stm32 of=/dev/mmcblk0p2 conv=fdatasync
dd if=metadata.bin of=/dev/mmcblk0p3 conv=fdatasync
dd if=metadata.bin of=/dev/mmcblk0p4 conv=fdatasync
dd if=fip.bin of=/dev/mmcblk0p5 conv=fdatasync
dd if=/dev/zero of=/dev/mmcblk0p7 conv=fdatasync

```





stm32mp135 sdk 的 linux包编译时出错解决:
``` bash
cc1: error: cannot load plugin ./scripts/gcc-plugins/arm_ssp_per_task_plugin.so: ./scripts/gcc-plugins/arm_ssp_per_task_plugin.so: undefined symbol: _ZN8opt_pass14set_pass_paramEjb


Okay. I've got a dirty solution. Open .config (kernel configuration), find
CONFIG_HAVE_GCC_PLUGINS=y
CONFIG_GCC_PLUGINS=y
and change value to "n"
then try to make again
```





``` bash
#rarch=armautoload=nobaudrate=115200board=stm32mp1board_id=0x1635board_name=stm32mp135f-dkboard_rev=0x000bboot_a_script=load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script}; 
source ${scriptaddr}boot_device=mmcboot_efi_binary=load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} efi/boot/bootarm.efi; 
if fdt addr ${fdt_addr_r}; 
then bootefi ${kernel_addr_r} ${fdt_addr_r};
else bootefi ${kernel_addr_r} ${fdtcontroladdr};
fiboot_efi_bootmgr=if fdt addr ${fdt_addr_r}; 
then bootefi bootmgr ${fdt_addr_r};else bootefi bootmgr;fiboot_extlinux=sysboot ${devtype} ${devnum}:${distro_bootpart} any ${scriptaddr} ${prefix}${boot_syslinux_conf}boot_instance=0boot_net_usb_start=trueboot_prefixes=/ /boot/boot_script_dhcp=boot.scr.uimgboot_scripts=boot.scr.uimg boot.scrboot_syslinux_conf=extlinux/extlinux.confboot_targets=mmc1 ubifs0 mmc0 usb0 pxe bootcmd=run bootcmd_stm32mpbootcmd_mmc0=devnum=0; run mmc_bootbootcmd_mmc1=devnum=1; run mmc_bootbootcmd_pxe=run boot_net_usb_start; dhcp; if pxe get; then pxe boot; fibootcmd_stm32mp=echo "Boot over ${boot_device}${boot_instance}!";if test ${boot_device} = serial || test ${boot_device} = usb;then stm32prog ${boot_device} ${boot_instance}; else run env_check;if test ${boot_device} = mmc;then env set boot_targets "mmc${boot_instance}"; fi;if test ${boot_device} = nand || test ${boot_device} = spi-nand ;then env set boot_targets ubifs0; fi;if test ${boot_device} = nor;then env set boot_targets mmc0; fi;run distro_bootcmd;fi;bootcmd_ubifs0=devnum=0; run ubifs_bootbootcmd_usb0=devnum=0; run usb_bootbootdelay=1
console=ttySTM0
cpu=armv7distro_bootcmd=for target in ${boot_targets}; do run bootcmd_${target}; doneefi_dtb_prefixes=/ /dtb/ /dtb/current/env_check=if env info -p -d -q; then env save; fieth1addr=10:e7:7a:e3:40:14ethaddr=10:e7:7a:e3:40:13fdt_addr_r=0xc4000000fdtcontroladdr=dabf0e10fdtfile=stm32mp135f-dk.dtb
fdtoverlay_addr_r=0xc4300000
kernel_addr_r=0xc2000000
load_efi_dtb=load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${prefix}${efi_fdtfile}loadaddr=0xc2000000mmc_boot=if mmc dev ${devnum}; then devtype=mmc; run scan_dev_for_boot_part; fipxefile_addr_r=0xc4200000ramdisk_addr_r=0xc4400000scan_dev_for_boot=echo Scanning ${devtype} ${devnum}:${distro_bootpart}...; for prefix in ${boot_prefixes}; do run scan_dev_for_extlinux; run scan_dev_for_scripts; done;run scan_dev_for_efi;scan_dev_for_boot_part=part list ${devtype} ${devnum} -bootable devplist; env exists devplist || setenv devplist 1; for distro_bootpart in ${devplist}; do if fstype ${devtype} ${devnum}:${distro_bootpart} bootfstype; then run scan_dev_for_boot; fi; done; setenv devplistscan_dev_for_efi=setenv efi_fdtfile ${fdtfile}; if test -z "${fdtfile}" -a -n "${soc}"; then setenv efi_fdtfile ${soc}-${board}${boardver}.dtb; fi; for prefix in ${efi_dtb_prefixes}; do if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${efi_fdtfile}; then run load_efi_dtb; fi;done;run boot_efi_bootmgr;if test -e ${devtype} ${devnum}:${distro_bootpart} efi/boot/bootarm.efi; then echo Found EFI removable media binary efi/boot/bootarm.efi; run boot_efi_binary; echo EFI LOAD FAILED: continuing...; fi; setenv efi_fdtfilescan_dev_for_extlinux=if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${boot_syslinux_conf}; then echo Found ${prefix}${boot_syslinux_conf}; run boot_extlinux; echo SCRIPT FAILED: continuing...; fiscan_dev_for_scripts=for script in ${boot_scripts}; do if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${script}; then echo Found U-Boot script ${prefix}${script}; run boot_a_script; echo SCRIPT FAILED: continuing...; fi; donescriptaddr=0xc4100000serial#=801D000B3031510A30333432serverip=192.168.1.1soc=stm32mpubifs_boot=env exists bootubipart || env set bootubipart UBI; env exists bootubivol || env set bootubivol boot; if ubi part ${bootubipart} && ubifsmount ubi${devnum}:${bootubivol}; then devtype=ubi; run scan_dev_for_boot; fiusb_boot=usb start; if usb dev ${devnum}; then devtype=usb; run scan_dev_for_boot_part; fiusb_pgood_delay=1000vendor=st\arch=armautoload=no
baudrate=115200
board=stm32mp1board_id=0x1635board_name=stm32mp135f-dkboard_rev=0x000bboot_a_script=load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script}; source ${scriptaddr}boot_device=mmcboot_efi_binary=load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} efi/boot/bootarm.efi; if fdt addr ${fdt_addr_r}; then bootefi ${kernel_addr_r} ${fdt_addr_r};else bootefi ${kernel_addr_r} ${fdtcontroladdr};fiboot_efi_bootmgr=if fdt addr ${fdt_addr_r}; then bootefi bootmgr ${fdt_addr_r};else bootefi bootmgr;fiboot_extlinux=run scan_m4fw;run scan_overlays; sysboot ${devtype} ${devnum}:${distro_bootpart} any ${scriptaddr} ${prefix}${boot_syslinux_conf}boot_instance=0boot_m4fw=rproc init; rproc load 0 ${m4fw_addr} ${filesize}; rproc start 0boot_net_usb_start=trueboot_prefixes=/mmc0_boot_script_dhcp=boot.scr.uimgboot_scripts=boot.scr.uimg boot.scrboot_syslinux_conf=extlinux/stm32mp135f-dk_extlinux.confboot_targets=mmc0bootcmd=run bootcmd_stm32mpbootcmd_mmc0=devnum=0; run mmc_bootbootcmd_mmc1=devnum=1; run mmc_bootbootcmd_pxe=run boot_net_usb_start; dhcp; if pxe get; then pxe boot; fibootcmd_stm32mp=echo "Boot over ${boot_device}${boot_instance}!";if test ${boot_device} = serial || test ${boot_device} = usb;then stm32prog ${boot_device} ${boot_instance}; else run env_check;if test ${boot_device} = mmc;then env set boot_targets "mmc${boot_instance}"; fi;if test ${boot_device} = nand || test ${boot_device} = spi-nand ;then env set boot_targets ubifs0; fi;if test ${boot_device} = nor;then env set boot_targets mmc0; fi;run distro_bootcmd;fi;bootcmd_ubifs0=devnum=0; run ubifs_bootbootcmd_usb0=devnum=0; run usb_bootbootdelay=1bootfstype=ext4console=ttySTM0cpu=armv7devplist=8distro_bootcmd=for target in ${boot_targets}; do run bootcmd_${target}; doneefi_dtb_prefixes=/ /dtb/ /dtb/current/env_check=if env info -p -d -q; then env save; fieth1addr=10:e7:7a:e3:40:14ethaddr=10:e7:7a:e3:40:13fdt_addr_r=0xc4000000fdtcontroladdr=dabf0e10fdtfile=stm32mp135f-dk.dtbfdtoverlay_addr_r=0xc4300000fileaddr=c4100000filesize=efdkernel_addr_r=0xc2000000load_efi_dtb=load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${prefix}${efi_fdtfile}loadaddr=0xc2000000m4fw_addr=0xc2000000m4fw_name=rproc-m4-fw.elfmmc_boot=if mmc dev ${devnum}; then devtype=mmc; run scan_dev_for_boot_part; fiov_apply=test -n ${fdtovaddr} && test -n ${overlay} && for ov in ${overlay}; do echo overlaying ${ov}...; load ${devtype} ${devnum}:${distro_bootpart} ${fdtovaddr} /overlays/${ov}.dtbo && fdt resize ${filesize} && fdt apply ${fdtovaddr}; doneov_init=load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${fdtfile} && env set fdt_addr ${fdt_addr_r} && fdt addr ${fdt_addr} && setexpr fdtovaddr ${fdt_addr} + C0000pxefile_addr_r=0xc4200000ramdisk_addr_r=0xc4400000scan_dev_for_boot=echo Scanning ${devtype} ${devnum}:${distro_bootpart}...; for prefix in ${boot_prefixes}; do run scan_dev_for_extlinux; run scan_dev_for_scripts; done;run scan_dev_for_efi;scan_dev_for_boot_part=part list ${devtype} ${devnum} -bootable devplist; env exists devplist || setenv devplist 1; for distro_bootpart in ${devplist}; do if fstype ${devtype} ${devnum}:${distro_bootpart} bootfstype; then run scan_dev_for_boot; fi; done; setenv devplistscan_dev_for_efi=setenv efi_fdtfile ${fdtfile}; if test -z "${fdtfile}" -a -n "${soc}"; then setenv efi_fdtfile ${soc}-${board}${boardver}.dtb; fi; for prefix in ${efi_dtb_prefixes}; do if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${efi_fdtfile}; then run load_efi_dtb; fi;done;run boot_efi_bootmgr;if test -e ${devtype} ${devnum}:${distro_bootpart} efi/boot/bootarm.efi; then echo Found EFI removable media binary efi/boot/bootarm.efi; run boot_efi_binary; echo EFI LOAD FAILED: continuing...; fi; setenv efi_fdtfilescan_dev_for_extlinux=if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${boot_syslinux_conf}; then echo Found ${prefix}${boot_syslinux_conf}; run boot_extlinux; echo SCRIPT FAILED: continuing...; fiscan_dev_for_scripts=for script in ${boot_scripts}; do if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${script}; then echo Found U-Boot script ${prefix}${script}; run boot_a_script; echo SCRIPT FAILED: continuing...; fi; donescan_m4fw=if test -e ${devtype} ${devnum}:${distro_bootpart} ${m4fw_name};then echo Found M4 FW $m4fw_name; if load ${devtype} ${devnum}:${distro_bootpart} ${m4fw_addr} ${m4fw_name}; then run boot_m4fw; fi; fi;scan_overlays=if test -e ${devtype} ${devnum}:${distro_bootpart} /overlays/overlays.txt && load ${devtype} ${devnum}:${distro_bootpart} ${loadaddr} /overlays/overlays.txt && env import -t ${loadaddr} ${filesize}; then echo loaded overlay.txt: ${overlay}; run ov_init; run ov_apply; fiscriptaddr=0xc4100000serial#=801D000B3031510A30333432serverip=192.168.1.1soc=stm32mpubifs_boot=env exists bootubipart || env set bootubipart UBI; env exists bootubivol || env set bootubivol boot; if ubi part ${bootubipart} && ubifsmount ubi${devnum}:${bootubivol}; then devtype=ubi; run scan_dev_for_boot; fiusb_boot=usb start; if usb dev ${devnum}; then devtype=usb; run scan_dev_for_boot_part; fiusb_pgood_delay=1000vendor=st
```


https://blog.csdn.net/qq_36769014/article/details/128126435
stm32mp1系列使用了 drm 显示框架，

# 测试屏幕
modetest -M stm -s 32:480x272


modetest -M stm -s 32:#0




stm32mp1 的系统制作。  
系统制作需要调整的不多，主要是 uboot ， linux 内核，和 rootfs 。
stm32mp1 系列官方提供了 gcc 工具链，直接加载编译环境就能有很方便的使用了。简单记录一下 uboot 和 linux 的编译过程。

## uboot

``` bash
tar xf u-boot-stm32mp-v2021.10-stm32mp1-r1-r0.tar.xz
cd u-boot-stm32mp-v2021.10-stm32mp1-r1
for p in `ls -1 ../*.patch`; do patch -p1 < $p; done

make stm32mp13_defconfig
make DEVICE_TREE=stm32mp135f-dk all

```

## linux

``` bash
tar xfJ linux-5.15.24.tar.xz
cd linux-5.15.24
for p in `ls -1 ../*.patch`; do patch -p1 < $p; done

make multi_v7_defconfig fragment*.config
make uImage vmlinux dtbs LOADADDR=0xC2000040


make INSTALL_MOD_PATH="../build/install_artifact" modules_install

# arch/arm/boot/uImage
# cp $PWD/../build/arch/arm/boot/uImage $PWD/../build/install_artifact/boot/
# cp $PWD/../build/arch/arm/boot/dts/st*.dtb $PWD/../build/install_artifact/boot/
```




stm32mp135 的 boot 文件是合成的：
/dev/sdb1    tf-a
/dev/sdb2    tf-a
/dev/sdb3    tf-a-metadata
/dev/sdb4    tf-a-metadata
/dev/sdb5    optee-os + u-boot
/dev/sdb6    optee-os + u-boot
/dev/sdb7    u-boot-env
/dev/sdb8    bootfs
/dev/sdb9    vendorfs
/dev/sdb10   rootfs
/dev/sdb11   userfs



root_dir = /home/nihao/work/stm32mp/nihso/STM32MP1Dev/stm32mp1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sources/arm-ostl-linux-gnueabi/optee-os-stm32mp-3.16.0-stm32mp1-r1-r0/optee-os-stm32mp-3.16.0-stm32mp1-r1



fiptool create \
--fw-config ${root_dir}/../../FIP_artifacts/arm-trusted-firmware/fwconfig/stm32mp135f-dk-fw-config-optee.dtb \
--hw-config ${root_dir}/../../FIP_artifacts/u-boot/u-boot-stm32mp135f-dk-trusted.dtb \

--nt-fw ${root_dir}/../../FIP_artifacts/u-boot/u-boot-nodtb-stm32mp13.bin \

--tos-fw ${root_dir}/../deploy/tee-header_v2-stm32mp135f-dk.bin \
--tos-fw-extra1 ${root_dir}/../deploy/tee-pager_v2-stm32mp135f-dk.bin \
--tos-fw-extra2 ${root_dir}/../deploy/tee-pageable_v2-stm32mp135f-dk.bin \

${root_dir}/../../FIP_artifacts/fip/fip-stm32mp135f-dk-optee.bin



stm32mp135的rtc功能会影响到 GPIO PI1 引脚上，会导致改引脚有异常的 32khz 的时钟信号







stm32mp135 的 UID 地址 0x5C005234 - 0x5C00523F
0x5C005234 + 12






监控网口热插拔：ifplugd，



systemd-networkd

https://blog.csdn.net/xuesong10210/article/details/113858728






监控磁盘设备添加和删除
inotifywait

inotify-tools

```bash
#!/bin/bash

while true; do
    inotifywait -e create,delete -r /dev
    # 在这里执行处理磁盘事件的操作
    # 你可以使用 $REPLY 变量来获取触发事件的文件
    DEVNAME=$(basename $REPLY)
    if [ -e "/dev/$DEVNAME" ]; then
        echo "Disk $DEVNAME inserted."
        # 在这里执行插入磁盘时的操作
    else
        echo "Disk $DEVNAME removed."
        # 在这里执行拔出磁盘时的操作
    fi
done

```








5g RM500U 调试记录
RM500U 是移远公司开发的 5G 应用模块，底板选用的是墨子号的开发底板。
调试的主要主要问题在网卡模式和AT指令上。
第一部分：AT指令

- AT+QNETDEVSTATUS=1 : 查寻网络链接状态
- AT+QNETDEVCTL=1,3,1 ： 设置开机自动链接网络，这个是非常重要的，
- AT+QCFG="usbnet",1 ： 设置网卡模式为 cdc eth
- AT+QCFG="usbnet",3 ： 设置网卡模式为 rndis，可能是驱动的原因，在135上并不怎么兼容。

关于usb自带的5个串口，这个每个都试了，发现并没有什么反应。手册中也没有查到关于这几个串口是否支持AT指令

https://www.cnblogs.com/zhijun1996/p/16484126.html




5g FM650-CN 调试记录
FM650-CN 模块是采用国产展锐平台的 5G 模组。
这个平台网上几乎没有资料，微雪官网也只有一个 AT 英文指令手册。
在使用 AT 指令过程中，非常不稳定，不过一般配置完成后，几乎不会再动AT指令模块了，所以这个问题还好。如果每次都要配置的话，他们的AT指令配置显然是非常糟心的。
注意，发送AT指令的时候一定要注意间隔和回复时间，该模块的AT指令响应非常令人糟心，而且AT口经常会挂掉。
主要用的AT指令：
- AT AT指令测试，测试模块是否准备完成
- AT+GTUSBMODE 设置5G模块 usb 网卡的模式，可选的 cdc 网卡，rndis 网卡等
- AT+GTAUTOCONNECT 设置自动链接网络，这个是配置连网的。
- AT+GTRNDIS 查寻网络ip，可用来判断是否连网
- AT+CGPADDR 查看接口的ip地址
- AT+CGCONTRDP 网上的说法是开启设备dhcp分配，这个设计有点糟心。


链接流程
AT
AT+GTAUTOCONNECT=1
AT+CGPADDR
AT+CGCONTRDP
AT+CGPADDR

注意，5g 模块工作是耗电比较大，135 的单个 usb 口都不足以支撑设备的运行功率。

OpenWrt实现4G/5G网络共享+公网IPv6地址透传分配
https://zhuanlan.zhihu.com/p/624187071?utm_id=0


2c7c:0316
echo "2c7c 0316" > /sys/bus/usb-serial/drivers/option1/new_id




echo -n 'file spi-stm32.c +p' > /sys/kernel/debug/dynamic_debug/control
开启内核特定模块的 log 打印。
echo -n 'file qmi_wwan_q.c +p' > /sys/kernel/debug/dynamic_debug/control

alias arm-ostl-linux-gnueabi-gcc="arm-ostl-linux-gnueabi-gcc  -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi"

alias arm-ostl-linux-gnueabi-g++="arm-ostl-linux-gnueabi-g++  -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi"      
alias arm-ostl-linux-gnueabi-gcc="arm-ostl-linux-gnueabi-gcc  -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi"



make CROSS_COMPILE=arm-ostl-linux-gnueabi- clean install CONFIG_PREFIX=`pwd`/install






CPP=arm-ostl-linux-gnueabi-gcc -E  -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi

CXX=arm-ostl-linux-gnueabi-g++  -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi


OE_CMAKE_TOOLCHAIN_FILE=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/share/cmake/OEToolchainConfig.cmake
M4=m4
OECORE_TUNE_CCARGS= -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7

OECORE_SDK_VERSION=4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15

PKG_CONFIG_PATH=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi/usr/lib/pkgconfig:/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi/usr/share/pkgconfig

HOSTNAME=027c22b8f6f1
GDB=arm-ostl-linux-gnueabi-gdb
SDKTARGETSYSROOT=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
OECORE_BASELIB=lib

TARGET_PREFIX=arm-ostl-linux-gnueabi-
OE_CMAKE_FIND_LIBRARY_CUSTOM_LIB_SUFFIX=
PWD=/home/nihao/workspace
LOGNAME=nihao
OECORE_TARGET_OS=linux-gnueabi

CXXFLAGS= -O2 -pipe -g -feliminate-unused-debug-types 
TEEC_EXPORT=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi/usr
OECORE_NATIVE_SYSROOT=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux

LDFLAGS=-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed 

HOME=/home/nihao
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
OPENSSL_CONF=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/lib/ssl-3/openssl.cnf
KCFLAGS=--sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
OECORE_TARGET_SYSROOT=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
CPPFLAGS=
OPENSSL_MODULES=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/lib/ossl-modules/
LD=arm-ostl-linux-gnueabi-ld  --sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
READELF=arm-ostl-linux-gnueabi-readelf
TA_DEV_KIT_DIR=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi/usr/include/optee/export-user_ta
CORSS_COMPILE=arm-linux-gnueabihf-
LESSCLOSE=/usr/bin/lesspipe %s %s
LIBGCC_LOCATE_CFLAGS=--sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
TERM=xterm
LESSOPEN=| /usr/bin/lesspipe %s
USER=nihao
AR=arm-ostl-linux-gnueabi-ar
AS=arm-ostl-linux-gnueabi-as 
ARCH=arm
SHLVL=2
NM=arm-ostl-linux-gnueabi-nm
OECORE_TARGET_ARCH=arm
OECORE_DISTRO_VERSION=4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15
PKG_CONFIG_SYSROOT_DIR=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
OECORE_ACLOCAL_OPTS=-I /opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/share/aclocal
OBJCOPY=arm-ostl-linux-gnueabi-objcopy
STRIP=arm-ostl-linux-gnueabi-strip
OBJDUMP=arm-ostl-linux-gnueabi-objdump
CONFIG_SITE=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/site-config-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
PATH=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/bin:/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/sbin:/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/bin:/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/sbin:/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/bin/../x86_64-ostl_sdk-linux/bin:/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi:/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-musl:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/home/nihao/workspace/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin
CC=arm-ostl-linux-gnueabi-gcc  -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
CFLAGS= -O2 -pipe -g -feliminate-unused-debug-types 
CROSS_COMPILE=arm-ostl-linux-gnueabi-
MAIL=/var/mail/nihao
CONFIGURE_FLAGS=--target=arm-ostl-linux-gnueabi --host=arm-ostl-linux-gnueabi --build=x86_64-linux --with-libtool-sysroot=/opt/st/stm32mp1/4.0.1-openstlinux-5.15-yocto-kirkstone-mp1-v22.06.15/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
RANLIB=arm-ostl-linux-gnueabi-ranlib
OLDPWD=/home/nihao/workspace/busybox-1.36.0
_=/usr/bin/env


stm32mp1 时钟树
https://pic2.zhimg.com/v2-572f2c5cb10c29efa22325f51efaa515_r.jpg



core135对rndis支持有点缺陷，暂时不知道是什么缺陷，但是可以使用cdc_ether进行支持
也就是将rm500u模块的网卡设置为cdc_ether模式。







loral：


https://www.waveshare.net/wiki/SX1302_LoRaWAN_Gateway_HAT


mkdir lora
cd lora
proxychains git clone https://github.com/Lora-net/lora_gateway.git
# LoRa Gateway drivers
proxychains git clone https://github.com/Lora-net/packet_forwarder.git
# packet forwarding software
proxychains git clone https://github.com/HelTecAutomation/lorasdk.git
# This package will create a "lrgateway" service in Raspberry Pi
cd /home/pi/lora/lora_gateway
make clean all -j
cd /home/pi/lora/packet_forwarder
make clean all -j
cd /home/pi/lora/lorasdk
chmod +x install.sh 
./install.sh
#Run the script. After the script is run, it will create a service named "lrgateway". The purpose is to make the lora driver and data forwarding program run automatically at startup.
sudo cp -f /home/pi/lora/lorasdk/global_conf_EU868.json /home/pi/lora/packet_forwarder/lora_pkt_fwd/global_conf.json
#the "global_conf_EU868.json" may need change to your need.

https://www.waveshare.net/wiki/SX1302_LoRaWAN_Gateway_HAT













sudo sgdisk -og -a 1 Core135_sd.raw
sudo sgdisk -a 1 -n 1:34:545 -c 1:fsbl1 -t 1:8301  Core135_sd.raw
sudo sgdisk -a 1 -n 2:546:1057 -c 2:fsbl2 -t 2:8301  Core135_sd.raw
sudo sgdisk -a 1 -n 3:1058:1569 -c 3:metadata1 -t 3:8301  Core135_sd.raw
sudo sgdisk -a 1 -n 4:1570:2081 -c 4:metadata2 -t 4:8301  Core135_sd.raw
sudo sgdisk -a 1 -n 5:2082:10273 -c 5:fip-a -t 5:19d5df83-11b0-457b-be2c-7559c13142a5  -u 5:4fd84c93-54ef-463f-a7ef-ae25ff887087 Core135_sd.raw
sudo sgdisk -a 1 -n 6:10274:18465 -c 6:fip-b -t 6:19d5df83-11b0-457b-be2c-7559c13142a5  -u 6:09c54952-d5bf-45af-acee-335303766fb3 Core135_sd.raw
sudo sgdisk -a 1 -n 7:18466:19489 -c 7:u-boot-env -t 7:8301  Core135_sd.raw
sudo sgdisk -a 1 -n 8:19490:150561 -c 8:bootfs -t 8:8300  -A 8:set:2 Core135_sd.raw
sudo sgdisk -a 1 -n 9:150562:183329 -c 9:vendorfs -t 9:8300  Core135_sd.raw
sudo sgdisk -a 1 -n 10:183330:  -c 10:rootfs -t 10:8300  -u 10:e91c4e10-16e6-4c0e-bd0e-77becf4a3582 Core135_sd.raw

# 打印出分区表
sudo sgdisk -p Core135_sd.raw




dd_down /dev/sda1 /dev/loop100p1
dd_down /dev/sda2 /dev/loop100p2
dd_down /dev/sda3 /dev/loop100p3
dd_down /dev/sda4 /dev/loop100p4
dd_down /dev/sda5 /dev/loop100p5
dd_down /dev/sda6 /dev/loop100p6
dd_down /dev/sda7 /dev/loop100p7
dd_down /dev/sda8 /dev/loop100p8
dd_down /dev/sda9 /dev/loop100p9
dd_down /dev/sda10 /dev/loop100p10











