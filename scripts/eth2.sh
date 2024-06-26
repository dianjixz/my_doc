#!/bin/bash
# 监控实时网卡流量
# $1 接收所传第一个参数 即要监控的网卡
NIC=$1
# echo -e "traffic in --- traffic out"
while true;do
        # $0 命令输出结果 ~ 匹配模式
        OLD_IN=`awk '$0~"'$NIC'"{print $2}' /proc/net/dev`
        OLD_OUT=`awk '$0~"'$NIC'"{print $10}' /proc/net/dev`
        sleep 1
        NEW_IN=`awk '$0~"'$NIC'"{print $2}' /proc/net/dev`
        NEW_OUT=`awk '$0~"'$NIC'"{print $10}' /proc/net/dev`
        clear
        # printf不换行 %s占位符
        IN=$(printf "%.1f%s" "$(($NEW_IN-$OLD_IN))" "B/s")
        OUT=$(printf "%.1f%s" "$(($NEW_OUT-$OLD_OUT))" "B/s")
        echo "       traffic in  `date +%k:%M:%S`  traffic out "
        echo "$NIC   $IN              $OUT"

done