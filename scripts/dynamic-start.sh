#!/bin/bash

# 检查输入命令
if [ $# -lt 1 ]; then
    echo "Usage: $0 <command> [args...]"
    exit 1
fi

# 获取脚本的实际路径（考虑软链接）
SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_NAME=$(basename "$SCRIPT_PATH")

# 创建临时目录
TEMP_DIR=$(mktemp -d)
if [ ! -d "$TEMP_DIR" ]; then
    echo "Failed to create temporary directory."
    exit 1
fi

# 清理临时目录的函数
cleanup() {
    rm -rf "$TEMP_DIR"
}
trap cleanup EXIT

# Base64 编码的压缩文件内容（这里以 gz 格式文件为例）
BASE64_DATA="H4sIAAAAAAAAA+3BMQEAAADCoPVPbQwfoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD4B/B4AEBUwAAAA"

# 解码并解压文件到临时目录
echo "$BASE64_DATA" | base64 -d | tar -xz -C "$TEMP_DIR"
if [ $? -ne 0 ]; then
    echo "Failed to decode and extract the archive."
    exit 1
fi

# 设置临时目录的路径
ROOTFS="$TEMP_DIR/rootfs"
LD="$TEMP_DIR/rootfs/lib/ld-linux.so.2" # 动态查找 ld 加载器
LIB_DIR="$ROOTFS/lib:$ROOTFS/usr/lib:$ROOTFS/usr/lib/x86_64-linux-gnu"  # 假设 libc 所在目录
PATH="$ROOTFS/bin:$ROOTFS/usr/bin:$ROOTFS/usr/sbin"


if [ $SCRIPT_NAME != "dynamic-script.sh" ]; then
    APP="$ROOTFS/bin/$1"
else
    APP="$ROOTFS/bin/$SCRIPT_NAME"
fi

# 设置 LD_LIBRARY_PATH 并运行命令
exec LD_LIBRARY_PATH="$LIB_DIR" PATH="$PATH" "$LD" --library-path "$LIB_DIR" "$APP" "$@"


DLD=/home/root/rootfs/lib/ld-musl-armhf.so.1
DLIBPATH=/home/root/rootfs/lib:/home/root/rootfs/usr/lib:/home/root/rootfs/usr/lib/arm-linux-gnueabihf
DPATH=/home/root/rootfs/bin:/home/root/rootfs/usr/bin:/home/root/rootfs/usr/sbin
PATH="$DPATH" $DLD --library-path $DLIBPATH 