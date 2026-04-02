# AX650 SSD 烧录指南

> 使用 SSD 手动烧录 AX650，替代传统方式以显著降低烧录时间。

---

## 概述

本文档记录如何通过 SSD 手动将系统镜像烧录到 AX650 设备的 eMMC 中。相比传统烧录方式，使用 SSD 可大幅提升烧录速度。

---

## 前提条件

- AX650 开发板（M5Stack AI Pyramid）
- 已准备好系统镜像文件（`.axp` 解压后）
- SSD 已挂载并可访问（示例中挂载于 `/mnt`）
- root 权限

---

## eMMC 分区布局

| 分区 | 大小 | 用途 |
|------|------|------|
| `mmcblk0p1` | 1536K | uboot |
| `mmcblk0p2` | 1536K | uboot_bk |
| `mmcblk0p3` | 1M | env |
| `mmcblk0p4` | 20M | param |
| `mmcblk0p5` | 6M | logo |
| `mmcblk0p6` | 1M | dtb |
| `mmcblk0p7` | 64M | kernel |
| `mmcblk0p8` | 1M | atf |
| `mmcblk0p9` | 1M | optee |
| `mmcblk0p10` | 1M | recovery_dtb |
| `mmcblk0p11` | 74M | recovery |
| `mmcblk0p12` | 30380032K (~29GB) | rootfs |

---

## 烧录步骤

### 1. 准备环境

```bash
# 挂载 SSD 到 /mnt
mount /dev/nvme0n1p2 /mnt/
cd /mnt/
```

### 2. 烧录各个分区

按顺序执行以下 `dd` 命令：

```bash
# 1. U-Boot 主分区
dd if=./u-boot_signed.bin of=/dev/mmcblk0p1

# 2. U-Boot 备份分区
dd if=./uboot_bk.bin of=/dev/mmcblk0p2

# 3. 清空 env 分区
dd if=/dev/zero of=/dev/mmcblk0p3

# 4. Param 分区（稀疏镜像需转换）
simg2img param_sparse.ext4 /dev/mmcblk0p4

# 5. Logo 分区
dd if=./axera_logo.bmp of=/dev/mmcblk0p5

# 6. DTB 分区
dd if=./AX650_emmc_signed.dtb of=/dev/mmcblk0p6

# 7. Kernel 分区
dd if=./boot_signed.bin of=/dev/mmcblk0p7

# 8. ATF 分区
dd if=./atf_bl31_signed.bin of=/dev/mmcblk0p8

# 9. OP-TEE 分区
dd if=./optee_signed.bin of=/dev/mmcblk0p9

# 10. Recovery DTB 分区
dd if=./recovery_signed.dtb of=/dev/mmcblk0p10

# 11. Recovery 分区
dd if=./recovery_signed.bin of=/dev/mmcblk0p11

# 12. Rootfs 分区（稀疏镜像需转换，耗时最长）
simg2img rootfs_sparse.ext4 /dev/mmcblk0p12

# 同步写入
sync
```

---

## 修改启动参数

### 临时从 NVMe 启动（用于烧录时）

```bash
# pcie启动
fw_setenv bootargs 'mem=2048M console=ttyS0,115200n8 earlycon=uart8250,mmio32,0x2016000 board_id=0x1,boot_reason=0x8, initcall_debug=0 loglevel=8 usbcore.autosuspend=-1 ax_boot_delay=10 rootfstype=ext4 rw rootwait root=/dev/nvme0n1p5 blkdevparts=mmcblk0:1536K(uboot),1536K(uboot_bk),1M(env),20M(param),6M(logo),1M(dtb),64M(kernel),1M(atf),1M(optee),1M(recovery_dtb),74M(recovery),30380032K(rootfs)'


# nfs 网络启动
fw_setenv bootargs 'mem=2048M console=ttyS0,115200n8 earlycon=uart8250,mmio32,0x2016000 board_id=0x1,boot_reason=0x8, initcall_debug=0 loglevel=8 usbcore.autosuspend=-1 ax_boot_delay=10   root=/dev/nfs rw nfsroot=192.168.28.109:/nfsexport/ax650_rootfs,v4.2,tcp ip=dhcp       blkdevparts=mmcblk0:1536K(uboot),1536K(uboot_bk),1M(env),20M(param),6M(logo),1M(dtb),64M(kernel),1M(atf),1M(optee),1M(recovery_dtb),74M(recovery),30380032K(rootfs)'


```

### 烧录完成后改回 eMMC 启动

```bash

fw_setenv bootargs 'mem=2048M console=ttyS0,115200n8 earlycon=uart8250,mmio32,0x2016000 board_id=0x1,boot_reason=0x8, initcall_debug=0 loglevel=8 usbcore.autosuspend=-1 ax_boot_delay=10 rootfstype=ext4 rw rootwait root=/dev/mmcblk0p12 blkdevparts=mmcblk0:1536K(uboot),1536K(uboot_bk),1M(env),20M(param),6M(logo),1M(dtb),64M(kernel),1M(atf),1M(optee),1M(recovery_dtb),74M(recovery),30380032K(rootfs)'
```

---

## 完整一键脚本

```bash
#!/bin/bash
set -e

MNT="/mnt"
echo "=== AX650 SSD 烧录脚本 ==="

# 挂载 SSD
mount /dev/nvme0n1p2 "$MNT" 2>/dev/null || true
cd "$MNT"

echo "[1/12] 烧录 u-boot..."
dd if=./u-boot_signed.bin of=/dev/mmcblk0p1 bs=1M

echo "[2/12] 烧录 u-boot_bk..."
dd if=./uboot_bk.bin of=/dev/mmcblk0p2 bs=1M

echo "[3/12] 清空 env 分区..."
dd if=/dev/zero of=/dev/mmcblk0p3 bs=1M || true

echo "[4/12] 烧录 param..."
simg2img param_sparse.ext4 /dev/mmcblk0p4

echo "[5/12] 烧录 logo..."
dd if=./axera_logo.bmp of=/dev/mmcblk0p5 bs=1M

echo "[6/12] 烧录 dtb..."
dd if=./AX650_emmc_signed.dtb of=/dev/mmcblk0p6 bs=1M

echo "[7/12] 烧录 kernel..."
dd if=./boot_signed.bin of=/dev/mmcblk0p7 bs=1M

echo "[8/12] 烧录 atf..."
dd if=./atf_bl31_signed.bin of=/dev/mmcblk0p8 bs=1M

echo "[9/12] 烧录 optee..."
dd if=./optee_signed.bin of=/dev/mmcblk0p9 bs=1M

echo "[10/12] 烧录 recovery_dtb..."
dd if=./recovery_signed.dtb of=/dev/mmcblk0p10 bs=1M

echo "[11/12] 烧录 recovery..."
dd if=./recovery_signed.bin of=/dev/mmcblk0p11 bs=1M

echo "[12/12] 烧录 rootfs（耗时较长）..."
simg2img rootfs_sparse.ext4 /dev/mmcblk0p12

echo "同步写入..."
sync

echo "设置启动参数为 eMMC..."
fw_setenv bootargs 'mem=2048M console=ttyS0,115200n8 earlycon=uart8250,mmio32,0x2016000 board_id=0x1,boot_reason=0x8, initcall_debug=0 loglevel=8 usbcore.autosuspend=-1 ax_boot_delay=10 rootfstype=ext4 rw rootwait root=/dev/mmcblk0p12 blkdevparts=mmcblk0:1536K(uboot),1536K(uboot_bk),1M(env),20M(param),6M(logo),1M(dtb),64M(kernel),1M(atf),1M(optee),1M(recovery_dtb),74M(recovery),30380032K(rootfs)'

echo "=== 烧录完成，准备重启 ==="
reboot
```

---

## 验证烧录结果

重启后检查当前启动参数：

```bash
cat /proc/cmdline
```

预期输出（从 eMMC 启动）：
```
root=/dev/mmcblk0p12
```

---

## 注意事项

1. **分区顺序**：必须按照分区号顺序烧录，特别是 `rootfs` 在 `p12`
2. **稀疏镜像**：`param` 和 `rootfs` 是稀疏镜像，需使用 `simg2img` 转换
3. **env 分区**：清空即可，U-Boot 会自动生成默认环境变量
4. **启动参数**：烧录完成后务必改回 `root=/dev/mmcblk0p12`，否则下次启动仍会从 NVMe 启动
5. **sync 命令**：烧录完成后必须执行 `sync` 确保数据写入完成

---

## 镜像文件清单

烧录所需文件（通常从 `.axp` 包解压获得）：

```
u-boot_signed.bin      → mmcblk0p1
uboot_bk.bin           → mmcblk0p2
param_sparse.ext4      → mmcblk0p4 (需 simg2img)
axera_logo.bmp         → mmcblk0p5
AX650_emmc_signed.dtb  → mmcblk0p6
boot_signed.bin        → mmcblk0p7
atf_bl31_signed.bin    → mmcblk0p8
optee_signed.bin       → mmcblk0p9
recovery_signed.dtb    → mmcblk0p10
recovery_signed.bin    → mmcblk0p11
rootfs_sparse.ext4     → mmcblk0p12 (需 simg2img)
```

---

## 参考

- 设备：M5Stack AI Pyramid (AX650)
- 镜像版本：`AI_Pyramid_emmc_ubuntu_rootfs_desktop_V3.6.4`
- 原始 U-Boot 环境变量：`fw_printenv`