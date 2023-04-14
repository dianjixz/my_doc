Linux内核模块常见问题
内核模块签名的问题

使用 insmod 命令加载内核模块时出现报错：

    [root@localhost ~]# insmod mbcache.ko
    ksign: module signed with unknown public key
    - signature keyid: 0fb015c8f72fe172 ver=4
    insmod: error inserting 'mbcache.ko': -1 Unknown error 126

此时可以使用 objcopy 命令移除内核模块中的签名：

    [root@localhost ~]# objcopy -R .note.module.sig mbcache.ko

移除之后如果内核版本一致没有其它错误就可以加载了。
内核模块安装路径

编译内核时可以使用 INSTALL_MOD_PATH 参数指定安装路径：

    [root@localhost ~]# make modules_install INSTALL_MOD_PATH=/media

__DATE__ 和 __TIME__ 编译报错

新版本 gcc 编译包含 __DATE__ 或者 __TIME__ 宏的源程序时，会报下面的错误：

    error: macro "__DATE__" might prevent reproducible builds
    error: macro "__TIME__" might prevent reproducible builds

解决办法为增加 EXTRA_CFLAGS 参数为：-Wno-error=date-time（报警但不报错） 或者 -Wno-date-time（不报警）。
inline 函数编译报错

程序里包含 inline 函数编译时可能遇到这种错误：

    warning: inline function ‘xxx’ declared but never defined

解决办法为增加 EXTRA_CFLAGS 参数 -fgnu89-inline。
acs_map 编译报错

使用 devtoolset 编译 kernel 在 make menuconfig 时可能报错：

    ld: scripts/kconfig/lxdialog/checklist.o: undefined reference to symbol 'acs_map'

为命令增加参数：

    make HOST_LOADLIBES="-lcurses -ltinfo" menuconfig

内核版本号附带加号

内核版本号自动附加加号一般由于修改了 git 版本库中的内核源代码，可以通过在内核源代码根目录增加 .scmversion 隐藏文件，或者通过 make 命令避免：

    make LOCALVERSION=

禁用内核自带模块

以 igb 模块为例，首先找到模块的初始化函数：

    [root@localhost ~]# grep -r module_init ~/linux/drivers/net/ethernet/intel/igb
    /root/linux/drivers/net/ethernet/intel/igb/igb_main.c:module_init(igb_init_module);

然后增加内核启动参数禁止加载模块：

    initcall_blacklist=igb_init_module

不过启动之后也是无法加载同名的第三方模块的。
下载目录的某次 commit 内容

如果不想完整 clone kernel 代码，又需要下载源码某个目录某次 commit 内容，新增 get-linux-kernel-commit.sh 脚本：

    #!/bin/sh
    K_ID="$1"
    K_DIR="$2"
    K_GIT="$3"
    if [ "x$K_GIT" = "x" -o "x$K_GIT" = "x-" ]; then
    K_GIT="https://git.kernel.org"
    fi
    K_PATH="$4"
    if [ "x$K_PATH" = "x" -o "x$K_PATH" = "x-" ]; then
    K_PATH="/pub/scm/linux/kernel/git/torvalds/linux.git"
    fi
    K_PATH="${K_PATH}/plain/"
    K_SUB=$5
    get_commit() {
    wget --no-check-certificate -q -O - "${K_GIT}${K_PATH}$1?id=$K_ID" | grep -v '>\.\./</a>' | sed -n '/\/plain\//{s/^.*href='\''//;s/'\''.*$//;p}' | while read T_PATH; do
    C_PATH="${T_PATH%*/?id=*}"
    if [ $C_PATH != $T_PATH ]; then
    if [ "x$K_SUB" != "x0" ]; then
    C_PATH="${C_PATH##*/}"
    mkdir $C_PATH
    OPWD=`pwd`
    cd $C_PATH
    get_commit $1/$C_PATH
    cd $OPWD
    fi
    else
    wget --no-check-certificate --content-disposition "${K_GIT}${T_PATH}"
    fi
    done
    }
    get_commit $K_DIR

使用方法（第一个参数为 commit id，第二个参数为源代码目录路径，第三个和第四个参数为 git 版本库地址，默认为 Linux kernel git，可以使用其它版本库地址，第五个可选参数指定 0 表示不递归下载下面的子目录）：

    get-linux-kernel-commit.sh e2f6ad4624dfbde3a6c42c0cfbfc5553d93c3cae fs/xfs

module_layout 符号不一致的问题

如果内核模块对应的内核版本相同，但使用的头文件不同，加载时可能会出现 disagrees about version of symbol module_layout 报错，如果需要强制加载，可以修改模块文件的 module_layout 符号版本：

    root@server:~# modprobe --dump-modversions new.ko | grep module_layout
    0x022a8bbb module_layout
    root@server:~# modprobe --dump-modversions old.ko | grep module_layout
    0xe89184a2 module_layout

内核模块的符号版本信息位于 ELF 的 __versions 段中，可以使用 objdump 命令显示所在位置：

    [root@centos7-dev centos]# objdump -h new.ko
    Sections:
    Idx Name Size VMA LMA File off Algn
    12 __versions 00000b00 0000000000000000 0000000000000000 00004978 2**3
    CONTENTS, ALLOC, LOAD, READONLY, DATA

例如从上面例子中的 0x4978 位置就可以找到符号版本信息，module_layout 符号一般是第一个，使用 16 进制编辑器改为与当前内核符号的版本就可以加载了。