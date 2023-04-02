#!/bin/bash

# 日志默认目录
Log_DIR="/data/shell/log"
# 连续计数
NETWORK_CHECK_COUNTER_FILE="${Log_DIR}/network-check-counter.log"
# 执行日志
NETWORK_CHECK_LOG_FILE="${Log_DIR}/network-check.log"
# 默认计数为0
COUNTER=0

# 连续失败计数大于该数值，则进行 RESTART_INTERVAL 秒等待，再执行重新检测
COUNTER_THRESHOLD=10
# 持续失败，后默认等待时间(秒)，然后再重启
RESTART_INTERVAL=600

# 检查目录是否存在，不存在则创建
if [ ! -d "$Log_DIR" ]; then
    echo "Creating directory $Log_DIR"
    mkdir -p "$Log_DIR"
fi

# 检查文件是否存在，如果不存在则创建文件
touch $NETWORK_CHECK_LOG_FILE

if [ ! -e $NETWORK_CHECK_COUNTER_FILE ]; then
    touch $NETWORK_CHECK_COUNTER_FILE
    echo "0" >$NETWORK_CHECK_COUNTER_FILE
fi
COUNTER=$(cat $NETWORK_CHECK_COUNTER_FILE)

# 检测网络是否畅通
function ping_domain() {
    # ping的域名或者DNS
    local domain=114.114.114.114
    # ping的次数
    local tries=6
    # 请求成功次数
    local packets_responded=0

    for i in $(seq 1 $tries); do
        if ping -c 1 $domain >/dev/null; then
            ((packets_responded++))
            sleep 1
        fi
    done

    # 如果请求成功总次数大于2，则表示成功
    if [ $packets_responded -ge 2 ]; then
        echo "true"
    else
        echo "false"
    fi
}

# 检测网络连接函数
function check_network() {
    # 如果ping 6次至少有2次包未响应，则执行一下代码
    if [ $(ping_domain) = "false" ]; then
        # 如果N无法连接网络，则重启网络
        echo "$(date '+%Y-%m-%d %H:%M:%S') 网络连接失败"
        echo "$(date '+%Y-%m-%d %H:%M:%S') 网络连接失败" >>$NETWORK_CHECK_LOG_FILE
        /etc/init.d/network restart
        sleep 20
        if [ $(ping_domain) = "false" ]; then
            # 如果仍无法连接网络，则重启WLAN服务
            echo "$(date '+%Y-%m-%d %H:%M:%S') 网络连接仍然失败，重启WLAN服务"
            echo "$(date '+%Y-%m-%d %H:%M:%S') 网络连接仍然失败，重启WLAN服务" >>$NETWORK_CHECK_LOG_FILE

            /sbin/ifup wan
            echo "$(date '+%Y-%m-%d %H:%M:%S') Wlan服务已重启"
            echo "$(date '+%Y-%m-%d %H:%M:%S') Wlan服务已重启" >>$NETWORK_CHECK_LOG_FILE
            echo $(($(cat $NETWORK_CHECK_COUNTER_FILE) + 1)) >$NETWORK_CHECK_COUNTER_FILE
            sleep 30

            if [ $(ping_domain) = "false" ]; then
                echo "$(date '+%Y-%m-%d %H:%M:%S') 重启WLAN后，联网失败，准备重启服务器"
                echo "$(date '+%Y-%m-%d %H:%M:%S') 重启WLAN后，联网失败，准备重启服务器" >>$NETWORK_CHECK_LOG_FILE
                echo $(($(cat $NETWORK_CHECK_COUNTER_FILE) + 1)) >$NETWORK_CHECK_COUNTER_FILE
                /sbin/reboot
            else
                echo "$(date '+%Y-%m-%d %H:%M:%S') 重启WLAN后，连接已恢复"
                echo "$(date '+%Y-%m-%d %H:%M:%S') 重启WLAN后，连接已恢复" >>$NETWORK_CHECK_LOG_FILE
                echo "0" >$NETWORK_CHECK_COUNTER_FILE
            fi
        else
            echo "$(date '+%Y-%m-%d %H:%M:%S') 重启网络网络后，连接已恢复"
            echo "$(date '+%Y-%m-%d %H:%M:%S') 重启网络网络后，连接已恢复" >>$NETWORK_CHECK_LOG_FILE
            echo "0" >$NETWORK_CHECK_COUNTER_FILE
        fi
    else
        echo "$(date '+%Y-%m-%d %H:%M:%S') 网络连接正常"
        # 可以通过server 酱发送消息
        #if [ $(cat $NETWORK_CHECK_COUNTER_FILE) -ge 1 ]; then
        # 如果网络畅通且之前有失败计数，则进行POST请求
        #echo "$(date '+%Y-%m-%d %H:%M:%S') 网络连接已恢复，发送POST请求"
        #echo "$(date '+%Y-%m-%d %H:%M:%S') 网络连接已恢复，发送POST请求" >>$NETWORK_CHECK_LOG_FILE
        # 使用curl发送POST请求到 http://aaa.com
        # curl -X POST http://baidu.com
        #fi
        echo "0" >$NETWORK_CHECK_COUNTER_FILE
    fi
}

# 计数器检查函数
function check_counter() {
    COUNTER=$(cat $NETWORK_CHECK_COUNTER_FILE)
    if [[ $COUNTER -ge $COUNTER_THRESHOLD ]]; then
        echo "$(date '+%Y-%m-%d %H:%M:%S') 计数器值大于等于 $COUNTER_THRESHOLD ，等待 $RESTART_INTERVAL 秒后重新检测网络连接"
        echo "$(date '+%Y-%m-%d %H:%M:%S') 计数器值大于等于 $COUNTER_THRESHOLD ，等待 $RESTART_INTERVAL 秒后重新检测网络连接" >>$NETWORK_CHECK_LOG_FILE
        sleep $RESTART_INTERVAL # 等待
        echo "$(date '+%Y-%m-%d %H:%M:%S') 等待 $RESTART_INTERVAL 秒后，开始重新检测网络" >>$NETWORK_CHECK_LOG_FILE
        check_network
    else
        check_network
    fi
}

check_counter

echo "$(date '+%Y-%m-%d %H:%M:%S') network 检查完毕"





# 脚本思路
# #

# 1、ping指定域名5次，只要要有2次以上的包响应，才表示网络畅通
# 2、如果低于2次，则视为网络不通，重启网络，并等待20秒
# 3、再次检测网络如果还是网络不通的话，则重启openwrt服务
# 4、当连续失败计数大道10次以上，则重启次数改为10分钟，重复上述步骤，当网络畅通后，计数归零