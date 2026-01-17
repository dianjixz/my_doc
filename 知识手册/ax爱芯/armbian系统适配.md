# 爱芯适配 armbian 系统

为爱芯开发板提供 armbian 系统镜像


Armbian的Build工作流可以理解为其自动化编译和构建镜像的全过程。它为arm架构的开发板（如树莓派、橙派、Banana Pi等）生成定制化的Linux发行版镜像。以下为专业且详细的介绍：

一、核心流程总览

1. 源码准备
   - Armbian Build Framework本身是一个基于Bash脚本的大型自动化工具，源码通常托管在GitHub（https://github.com/armbian/build）。
   - 包含主脚本（build.sh）、定义板卡硬件特性（config/board）、内核配置（config/kernel）、用户自定义脚本、第三方包补丁，以及一系列辅助脚本。

2. 环境初始化
   - 脚本检查宿主环境（推荐Ubuntu/Debian主机），准备必要依赖：如qemu-user-static、debootstrap、gcc-arm等。
   - 可选地支持Docker方式隔离构建环境。

3. 选择目标和配置
   - 用户指定目标开发板、处理器架构（armhf、arm64、armv7等）、系统版本（Debian、Ubuntu），及内核版本。
   - 可通过命令行参数或配置文件调整定制选项（如桌面/服务器，是否包含额外驱动、是否启用特殊功能等）。

4. 根文件系统（RootFS）构建
   - 使用 debootstrap/minbase 拉取纯净的根文件系统，集成目标系统所需的基本包。
   - 应用Armbian默认配置、优化脚本，以及用户定制化脚本。

5. 内核编译和打补丁
   - 下载或拉取对应硬件适配的Linux内核源码（官方或主线）。
   - 应用Armbian自有或第三方补丁，针对特定硬件修复或优化。
   - 交叉编译支持arm体系（可根据板卡特性选择特定内核分支和功能）。

6. 构建U-Boot和引导部分
   - 下载/编译适配开发板的U-Boot（或其他引导程序），应用对应配置和补丁。

7. 安装板卡驱动及定制化功能
   - 集成Wi-Fi、蓝牙、GPIO等外围驱动；调整设备树（dtb）；配置分区、挂载、网络等。

8. 最终打包镜像
   - 组合根文件系统、内核、设备树、U-Boot，生成标准的SD卡镜像（.img）、固件包等发布物。
   - 镜像可挂载烧录至物理开发板用于启动。

9. 自动化测试和验收（可选）
   - 一些高度定制的Armbian Build流程集成了自动化测试脚本，对生成镜像进行启动、基本功能验证。

10. 发布及维护
   - 镜像及BSP可上传至Armbian官方、社区或私有仓库，供用户下载和进一步定制。

二、流程示意图（简略）

源码准备 → 环境检测/准备 → 用户配置/选择 → debootstrap生成根FS → 内核/U-Boot编译 → 驱动/定制 → 生成镜像 → 测试 → 发布

三、相关脚本（重要组成）

- build.sh：主入口脚本，参数解析，执行分步流程。
- config/boards/xxx.conf：板卡特性配置。
- config/kernel/xxx.config：内核配置参数。
- packages/armbian-firmware/：驱动与补丁管理。
- lib/、tools/、userpatches/等：辅助工具与用户自定义内容。

四、实用命令举例

```bash
./build.sh BOARD=orangepizero BRANCH=current RELEASE=bookworm BUILD_DESKTOP=yes
```
上述命令自动为Orange Pi Zero基于Debian Bookworm构建集成桌面并升级至最新内核分支的Armbian镜像。

五、与主流CI/CD融合

近年来，Armbian的build流经常与GitHub Actions、GitLab CI等自动化构建平台融合，以实现自动触发、状态追踪和多目标构建发布。

---

简而言之，Armbian的build工作流是一条高度自动化、可定制、可扩展的Linux系统定制和固件生成流水线，极大简化了arm开发板Linux系统移植、调优以及大量分发的工作。Armbian的build工作流是一个高度自动化的流程，主要用于为ARM架构的单板计算机（如Orange Pi、Banana Pi、NanoPi等）生成Linux（通常是基于Debian或Ubuntu）系统镜像。其核心流程如下：

**1. 环境准备与依赖安装**
- 一般推荐在基于Debian/Ubuntu的主机或Docker容器中进行。
- 自动下载和安装所需的包，如debootstrap、qemu-user-static、交叉编译器等。

**2. 用户配置阶段**
- 用户通过命令行参数或配置文件（如`build.sh`参数）选择目标开发板、发行版分支、内核版本等。
- 可以定制系统角色（如桌面版/服务器版）、第三方驱动、预装软件。

**3. 源码和补丁管理**
- 自动拉取适配目标开发板的内核源码、U-Boot（引导程序）源码及Armbian相关补丁。
- 依据板卡支持情况分别应用官方主线或特定分支的补丁。

**4. 根文件系统制作**
- 利用debootstrap或mmdebstrap构建基础的Debian/Ubuntu根文件系统。
- 安装Armbian定制包、固件、初始脚本、优化建议等。

**5. 交叉编译内核与U-Boot**
- 针对不同板卡，应用特定的配置、设备树，然后交叉编译内核和U-Boot。
- 集成驱动和必要的硬件补丁，解决兼容性问题。

**6. 组装镜像与自动化配置**
- 把根文件系统、内核、设备树、引导程序等组合成完整镜像（.img文件），适用于SD卡/EMMC烧录。
- 镜像内预置网络、分区布局、初始用户、SSH等。

**7.（可选）自动化测试**
- 某些开发流程会集成QEMU或真实设备的测试环节，验证镜像启动与基础功能。

**8. 打包、发布与清理**
- 最终镜像打包，生成校验信息。
- 支持CI/CD自动发布（如GitHub Actions集成）。

**典型命令举例：**
```bash
git clone https://github.com/armbian/build.git
cd build
./compile.sh BOARD=orangepizero RELEASE=bookworm BRANCH=current BUILD_DESKTOP=yes
```
这样会自动触发上述整个工作流。

---

**总结：**
Armbian build的工作流本质上是一条由源代码管理、编译、打补丁、定制用户空间、生成系统镜像到自动化发布的完整流水线。其核心脚本为`build.sh`/`compile.sh`。这个流程极大简化了ARM开发板Linux系统定制与量产的难度，并有良好的社区维护和自动化升级能力。Armbian的build工作流，即它的构建（Build）流程，大致可以分为下面几个主要阶段：

1. **环境准备**
   - 首先克隆 [armbian/build](https://github.com/armbian/build) 仓库到本地。
   - 脚本自动检测和安装所需依赖（比如交叉编译工具链、debootstrap、qemu-user-static等）。

2. **参数配置**
   - 通过运行 `./build.sh` 或 `./compile.sh` 并指定相关参数（如BOARD=目标板型号、RELEASE=系统发行版、BRANCH=内核分支、BUILD_MINIMAL/Desktop=精简/桌面），决定要构建什么镜像。
   - 你也可以用 `userpatches/customize-image.sh` 插入自定义脚本。

3. **源码与补丁处理**
   - 自动下载或拉取内核源码、U-Boot源码以及固件包。
   - 对不同板卡及目标版本应用官方和Armbian社区的相关补丁。

4. **根文件系统制作**
   - 使用 debootstrap 生成适用于目标架构的Debian/Ubuntu基础文件系统。
   - 根据配置集成Armbian优化和自定义脚本、驱动、应用程序。

5. **内核及U-Boot交叉编译**
   - 针对目标平台构建主线或特定厂商分支的 Linux 内核，同比例生成配套的驱动和设备树（dtb）。
   - 构建U-Boot引导加载器。

6. **镜像组合与打包**
   - 将内核、U-Boot、根文件系统组装成标准镜像（img或其他格式）。
   - 镜像内嵌有 Armbian 的First Run脚本，支持自动化初始化/配置。

7. **可选自动测试**
   - 可以通过集成QEMU或真实开发板实现自动化自测，验证镜像基础功能。

8. **输出/清理**
   - 生成最终的SD/EMMC镜像文件，并输出于 `/output/images` 目录。
   - 清理构建过程中产生的临时文件和中间产物。

**简而言之：**  
Armbian Build 工作流本质上是一个高度自动化的、脚本驱动的交叉编译与系统定制流水线。其核心目标是：让用户快速、方便地为不同ARM开发板生成定制化的Linux操作系统镜像，高度集成补丁、驱动和优化，适合开发、测试和量产部署。

**更进一步，可以集成GitHub Actions、GitLab CI等CI/CD工具链，实现分布式自动构建和定制化发布。**

如果你想要了解更细节的每一步自动化脚本、相关配置文件和代码流转，也可以参考 [官方文档](https://docs.armbian.com/Developer-Guide_Build-Preparation/)。Armbian的build工作流是一个自动化的固件编译、定制与打包流程，核心目标是为各种ARM开发板（如Orange Pi、Banana Pi等）快速生成高度定制化的Linux系统镜像。其工作流大致如下：

1. **环境准备**
   - 建议在Debian/Ubuntu主机或官方支持的Docker容器中运行build脚本。
   - 自动检测并安装所需依赖（交叉编译器、qemu-user-static、debootstrap等）。

2. **参数与配置**
   - 用户通过命令行参数（如`BOARD=xxx RELEASE=jammy BRANCH=current BUILD_DESKTOP=yes`）或配置文件，指定板卡型号、系统版本、内核分支、是否带桌面环境等。

3. **源码同步与补丁**
   - 自动拉取或下载官方及主线的Linux内核、U-Boot引导器源码及相应补丁。
   - 应用针对具体板子的补丁与定制化配置，确保硬件支持和稳定性。

4. **根文件系统生成**
   - 通过debootstrap等工具为目标架构生成最小化的Debian/Ubuntu根文件系统。
   - 安装基本包、固件、Armbian优化脚本等（如网络、扩展驱动支持等）。

5. **内核与引导加载器编译**
   - 依据目标平台和配置，自动交叉编译Linux内核并嵌入自定义设备树（dtb）、驱动。
   - 编译U-Boot并配置好启动参数。

6. **镜像组装与打包**
   - 按照预设分区方案，将内核、U-Boot、设备树及根文件系统整合为标准img镜像，适合烧录到SD/EMMC。
   - 镜像自动关联自定义FirstRun脚本（如自动扩展分区、初始化用户等）。

7. **（可选）自动化测试与校验**
   - 支持集成QEMU仿真测试，也可以结合CI/CD平台实现自动化构建和回归测试。

8. **产物发布**
   - 构建好的镜像、校验文件、构建日志一并输出到output目录，并可自动上传到指定仓库或镜像服务器。

**核心命令示例**：

```bash
./build.sh BOARD=orangepi5 RELEASE=bookworm BRANCH=current BUILD_DESKTOP=yes
```

**工作流自动化与扩展**：

- 整套流程为Bash脚本链条驱动，有完善的包、补丁、脚本、驱动管理。
- 支持高级定制，比如userpatches目录添加自定义补丁或应用。
- 现已支持与GitHub Actions、GitLab CI等主流CI/CD体系无缝集成，可实现云端批量自动化构建与测试。

**参考：**
- [Armbian官方build文档](https://docs.armbian.com/Developer-Guide_Build-Preparation/)

简言之，Armbian Build工作流就是“参数配置 → 自动源码拉取与补丁应用 → 根文件系统和内核交叉编译 → 镜像组装与打包 → 自动化测试/输出”的流水线。适合批量和高度定制的开发板Linux固件开发。



构建环境说明

    适用硬件：Hinlink-H88k-V3 | Armbian Build | Linux-Rockchip
    编译系统：Ubuntu 22.04 LTS On WSL2
    参考借鉴：官方构建手册 | 自定义编译Armbian

# WSL2上由于网络隔离了，所以需要先宿主机防火墙放行再设置虚拟机的代理
# PowerShell中运行来放开对wsl2的局域网访问权限, 设置 -> 防火墙 -> 高级设置 -> 入站规则中可看到
New-NetFirewallRule -DisplayName "WSL" -Direction Inbound -InterfaceAlias "vEthernet (WSL)" -Action Allow

# 开启代理，需要在国外下载相应文件
proxy_port=7890
proxy_host=$(ip route | awk '/default/ {print $3}')
export https_proxy="http://${proxy_host}:${proxy_port}"
export http_proxy="http://${proxy_host}:${proxy_port}"
export all_proxy="socks5://${proxy_host}:${proxy_port}"

开始构建系统

# 获取Armbian Build项目，项目运行时会自行安装缺少的依赖
git clone --depth=1 https://github.com/armbian/build
cd build

# 因为H88K设备无线网卡是MT7916，所以需要backport内核并使用我们自定义的内核配置
wget https://github.com/leux0/armbian-h88k-images/raw/main/backport-v6.1.39.patch -O patch/kernel/rk35xx-legacy/055-backport-v6.1.39.patch
wget https://github.com/leux0/armbian-h88k-images/raw/main/linux-rk35xx-legacy.config -O config/kernel/linux-rk35xx-legacy.config

# 构建不带桌面的最小系统镜像，计划用来当无线路由器
./compile.sh build BOARD=hinlink-h88k BRANCH=legacy RELEASE=bookworm GITHUB_MIRROR=cnpmjs \
BUILD_DESKTOP=no BUILD_MINIMAL=yes KERNEL_CONFIGURE=no KERNEL_GIT=shallow REGIONAL_MIRROR=china \
PACKAGE_LIST_BOARD="vim-tiny dnsmasq hostapd bridge-utils ifupdown iptables wireless-regdb"

# 构建带GNOME桌面的系统镜像
./compile.sh build BOARD=hinlink-h88k BRANCH=legacy RELEASE=bookworm \
BUILD_DESKTOP=yes BUILD_MINIMAL=no KERNEL_CONFIGURE=no KERNEL_GIT=shallow \
DESKTOP_ENVIRONMENT=gnome DESKTOP_ENVIRONMENT_CONFIG_NAME=config_base \
DESKTOP_APPGROUPS_SELECTED='3dsupport browsers desktop_tools internet' \
BOARD_FIRMWARE_INSTALL="-full" \
COMPRESS_OUTPUTIMAGE=sha,gpg,xz DEB_COMPRESS=xz SHARE_LOG=yes DEBUG=yes \
MAINLINE_MIRROR=tuna UBOOT_MIRROR=gitee GITHUB_MIRROR=cnpmjs DOWNLOAD_MIRROR=china

# 构建完成后可在 build/output/images 下看到img镜像，build/output/debs 中可找到内核安装包

构建参数解析

# 设备相关参数
BOARD				在 build/config/boards/ 下可找到支持的设备代号
BRANCH				在 build/config/kernel/ 下可找到支持的内核分支。legacy，vendor，edge
RELEASE				在 build/config/distributions/ 下可找到支持的发行版
BUILD_MINIMAL			(yes | no) 如为yes则默认网络管理为：systemd-networkd，否则为：NetworkManager
BUILD_DESKTOP			是否安装桌面，与 BUILD_MINIMAL 相冲突
DESKTOP_ENVIRONMENT		在 build/config/desktop/common/environments 下可找到支持的桌面

# 内核相关参数。注意：由于rk3588的legacy内核已被Armbian官方抛弃，所以legacy内核都需要自行编译
INSTALL_HEADERS			(yes | no) 是否安装内核头文件
BUILD_KSRC			(yes | no) 是否在构建时创建内核源码包
INSTALL_KSRC			(yes | no) 是否在预安装内核源码到镜像
KERNEL_GIT			- shallow：仅含特定的分支。- full：包含所有支持的分支，不仅下载量非常大，也需要大量磁盘空间
KERNEL_CONFIGURE		- prebuilt：使用预编译的内核包（仅适用于在维护的硬件）。- yes：通过内核配置界面配置（增减模块或功能）。
				- no：使用Armbian提供的内核配置编译。如果留空则弹出内核配置界面。
# 镜像相关参数
BOOTSIZE			设置 /boot 分区大小（以MB为单位），如果 ROOTFS_TYPE=ext4 则失效
FIXED_IMAGE_SIZE		创建指定大小的镜像文件（以MB为单位），而不是按 rootfs 大小计算
ROOTFS_TYPE			(ext4 | f2fs | btrfs | nilfs2 | xfs | nfs) 创建根文件系统时使用哪种格式
TRFS_COMPRESSION		(lzo | none | zlib | zstd) 在 ROOTFS_TYPE=btrfs 时文件系统的压缩级别，默认为：zlib
IMAGE_XZ_COMPRESSION_RATIO	(1-9) 使用xz压缩镜像时压缩级别。调高时要注意内存消耗
COMPRESS_OUTPUTIMAGE		压缩镜像文件和GPG签名等以便重新分发。 - sha：为镜像生成SHA256哈希。- gpg：使用GPG对映像进行签名
				- 7z：将镜像、哈希和签名压缩到7Z存档。- gz：仅使用GZ格式压缩镜像。 - xz：仅使用XZ格式压缩镜像
# 下载来源相关
DOWNLOAD_MIRROR			(china | bfsu) 选择 toolchain、debian、ubuntu、packages 等的下载源，留空为使用官方下载源
ARMBIAN_MIRROR			(auto) 默认为自动选择Armbian的源，更改举例：ARMBIAN_MIRROR="https://yourlocalmirror.com"
MAINLINE_MIRROR			(google | tuna | bfsu) 选择主线内核源码下载源，留空为使用官方地址：git.kernel.org
USE_MAINLINE_GOOGLE_MIRROR	(yes | no) 如为yes则与MAINLINE_MIRROR=google的作用相同，即使用谷歌的主线内核源
UBOOT_MIRROR			(github | gitee) 选择主线UBOOT源码的下载源，留空为使用官方地址：source.denx.de
USE_GITHUB_UBOOT_MIRROR		(yes | no) 如为yes则与UBOOT_MIRROR=gtihub的作用相同，即使用GITHUB的UBOOT源
GITHUB_MIRROR			(fastgit | gitclone | cnpmjs) 选择GitHub托管存储库的下载镜像。留空为直连到GitHub，但对于中国大陆用户来说可能很慢
REGIONAL_MIRROR			(china) 根据选择区域设置apt源，不会覆盖已指定下载源选项的值。如果留空则使用默认设置
				该值为china会设置后面变量的值为：MAINLINE_MIRROR=tuna, UBOOT_MIRROR=gitee, GITHUB_MIRROR=fastgit, DOWNLOAD_MIRROR=china
				debootstrap也会从中国的tuna源获取rootfs，后续sources.list和armbian.list里用的也是 mirrors.tuna.tsinghua.edu.cn
# 其他相关参数
EXPERT				(yes | no) 是否以交互模式显示正在进行的项
NAMESERVER			( IPv4 ) 在chroot中使用的DNS解析器，它不会影响最终镜像。默认为：1.0.0.1
CONSOLE_AUTOLOGIN		(yes | no) 是否自动以root身份登录本地控制台（可能不安全，建议为no禁用）
SKIP_BOOTSPLASH			(yes | no) 跳过内核启动画面

其他构建技巧

    使用自定义的内核配置文件

# UBOOT和内核源码地址及分支都在该文件中定义：build/config/sources/families/rk35xx.conf
# 如果该内核配置文件存在：userpatches/linux-$LINUXFAMILY-$BRANCH.config
# 则将使用它们而不是默认的配置文件：build/config/kernel/linux-rk35xx-legacy.config
userpatches/linux-rk35xx-legacy.config			# 例子1
userpatches/linux-rockchip64-current.config		# 例子2

# 内核补丁存放位置，根据数字字母排序先后应用补丁
build/patch/kernel/rk35xx-legacy/0000.patching_config.yaml
build/userpatches/kernel/rk35xx-legacy/0000.patching_config.yaml

    查看桌面集合包内含软件包

# DESKTOP_APPGROUPS_SELECTED 中的包集合可在 build/config/desktop/common/appgroups/xxx/packages 中查看其包含的具体包
# DESKTOP_APPGROUPS_SELECTED="browsers chat desktop_tools editors email internet multimedia office programming remote_desktop"
build/config/desktop/common/appgroups/			# 通用包集合，对应 DESKTOP_APPGROUPS_SELECTED
build/config/desktop/common/environments/		# 桌面包集合，对应 DESKTOP_ENVIRONMENT

    安装特定的软件包

# 既可以在构建时使用 PACKAGE_LIST_BOARD="vim-tiny" 来安装，但需注意 build/config/boards/hinlink-h88k.csc 中是否已定义
# 也可在 build/userpatches/customize-image.sh 中使用 apt install vim-tiny 来安装自定义的软件包

    扩展系统功能

# ENABLE_EXTENSIONS=mesa-vpu,nvidia 参数可在 build/extensions/ 中查看可用的选项
build/extensions/mesa-vpu.sh		# 为RK3588开启GPU支持
build/extensions/nvidia.sh		# 安装nvidia-dkms驱动

# 在 https://github.com/armbian/build/blob/main/extensions/mesa-vpu.sh 中第100行可知以下包将被安装
# pkgs+=("rockchip-multimedia-config" "chromium" "libv4l-rkmpp" "gstreamer1.0-rockchip")

    克隆的源码在本地的路径

# 源码在本地的路径
build/cache/sources/rkbin-tools/							# rkbin克隆到本地的位置
build/cache/sources/u-boot-worktree/u-boot-rockchip64/next-dev-v2024.03/		# u-boot源码克隆到本地的位置
build/cache/sources/linux-kernel-worktree/5.10__rk35xx__arm64/				# 内核源码克隆到本地的位置

# Armbian预编译的软件包版本查看
https://github.com/armbian/os/pkgs/container/os%2Fuboot-hinlink-h88k-legacy		# uboot-hinlink-h88k-legacy
https://github.com/armbian/os/pkgs/container/os%2Farmbian-firmware-full			# armbian-firmware-full

    怎么跳过首次登录脚本（创建用户、时区语言、网络连接等）

    如果存在文件 /root/.not_logged_in_yet 那么启动就会运行Armbian的首次登录脚本 /usr/lib/armbian/armbian-firstlogin 来配置系统环境
    删除它后可跳过执行，后续再想执行首次登录脚本来配置系统环境，可先 touch /root/.not_logged_in_yet，然后执行 /usr/lib/armbian/armbian-firstlogin

    怎么在 chroot 环境中运行用户的脚本来自定义系统镜像

# 脚本 build/userpatches/customize-image.sh 会被复制到 chroot 环境中的 /tmp 目录并在 chroot 环境中执行
# 在 git clone armbian build 项目后创建的 userpatches/customize-image.sh 脚本不会在编译时被覆盖
# 构建主机上的 build/userpatches/overlay 目录在 chroot 环境中会绑定到 /tmp/overlay
# 将文件放在 build/userpatches/overlay 下，即可在 customize-image.sh 中通过 /tmp/overlay 路径进行访问
# 在 customize-image.sh 中可通过 $1 $2 $3 $4 来获取 RELEASE LINUXFAMILY BOARD BUILD_DESKTOP 等变量

leux@B650:~$ cat build/userpatches/customize-image.sh
#!/bin/bash

RELEASE=$1
LINUXFAMILY=$2
BOARD=$3
BUILD_DESKTOP=$4

# 输出的内容为：bookworm rk35xx hinlink-h88k no
echo "$RELEASE $LINUXFAMILY $BOARD $BUILD_DESKTOP"

# 跳过首次脚本（默认用户密码：root:1234）
rm /root/.not_logged_in_yet

# 禁网卡重命名
echo "extraboardargs=net.ifnames=0" >> /boot/armbianEnv.txt

# 开启内核转发
sed -i "s/#net.ipv4.ip_forward=1/net.ipv4.ip_forward=1/" /etc/sysctl.conf
sed -i "s/#net.ipv6.conf.all.forwarding=1/net.ipv6.conf.all.forwarding=1/" /etc/sysctl.conf

# 拷贝编译好的程序
cp -p /tmp/overlay/quectel-CM /usr/local/bin/
chmod 755 /usr/local/bin/quectel-CM

# 配置脚本来开机自启
cat << EOF > /etc/systemd/system/quectel-cm.service
[Unit]
Description=Quectel-CM Service
After=network.target
Wants=network.target

[Service]
ExecStop=/bin/kill -s TERM \$MAINPID
ExecStart=/usr/local/bin/quectel-CM -s ctnet -4 -6

[Install]
WantedBy=multi-user.target
EOF

# 设置软件开机自启
systemctl enable quectel-cm

构建问题修复

    需要使用支持UTF-8的终端例如Windows Terminal，或者出现该问题时输入<Enter>跳过，否则10秒后将退出

Please use a terminal that supports UTF-8. For example: [ Windows Terminal ]
[💲|🚸] Get it at the Microsoft Store [ https://apps.microsoft.com/store/detail/windows-terminal/9N0DX20HK701 ]
[💲|💥] Problem detected [ WSL2 Terminal does not support UTF-8 ]
[💲|💥] Exiting in 10 seconds [ Press <Ctrl-C> to abort, <Enter> to ignore and continue ]




```bash
# 模板命令
# ./compile.sh build BOARD=nlnet-xiguapi BRANCH=edge BUILD_DESKTOP=no BUILD_MINIMAL=yes KERNEL_CONFIGURE=no RELEASE=trixie
# 适配的指令
./compile.sh build BOARD=m5stack-ax630c-kit BRANCH=edge BUILD_DESKTOP=no BUILD_MINIMAL=yes KERNEL_CONFIGURE=no RELEASE=trixie




./compile.sh build BOARD=pinecube BRANCH=edge BUILD_DESKTOP=no BUILD_MINIMAL=yes KERNEL_CONFIGURE=no RELEASE=trixie








git clone --depth=1 https://github.com/armbian/build
./compile.sh build BOARD=m5stack-ax630c-kit BRANCH=edge BUILD_DESKTOP=no BUILD_MINIMAL=yes KERNEL_CONFIGURE=no RELEASE=trixie







```