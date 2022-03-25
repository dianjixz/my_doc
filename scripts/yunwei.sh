 #!/bin/bash

#@File    :   shell.sh
#@Time    :   2022/01/20 15:48:39
#@Author  :   Li Ruilong
#@Version :   1.0
#@Desc    :   SPN 系统维护脚本
#@Contact :   1224965096@qq.com

echo "      ___           ___         ___
     /  /\         /  /\       /__/\
    /  /:/_       /  /::\      \  \:\
   /  /:/ /\     /  /:/\:\      \  \:\
  /  /:/ /::\   /  /:/~/:/  _____\__\:\
 /__/:/ /:/\:\ /__/:/ /:/  /__/::::::::\
 \  \:\/:/~/:/ \  \:\/:/   \  \:\~~\~~\/
  \  \::/ /:/   \  \::/     \  \:\  
   \__\/ /:/     \  \:\      \  \:\
     /__/:/       \  \:\      \  \:\
     \__\/         \__\/       \__\/
"
echo "SPN 日常巡查脚本输出"
# 清理屏幕
clear

function bash_os() {
    # "系统基础信息"

    #内核信息
    kernel=$(uname -r)
    #操作系统版本
    release=$(cat /etc/redhat-release)
    #主机名称
    hostname=$HOSTNAME
    #当前时间及运行时间
    dateload=$(uptime | awk -F "," '{print $1}')
    # 当前登录用户数
    users=$(uptime | awk -F "," '{print $2}')

    echo -e "\n\033[32m##############   系统基础信息 #######\033[0m\n"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|内核信息:\033[31m        $kernel           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|操作系统版本:\033[31m   $release        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|当前时间及运行时间:\033[31m   $dateload        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|当前登录用户数:\033[31m   $users        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
}

function memory() {

    # 内存相关数据统计`free -m`
    #总内存容量
    mem_total=$(free -m | awk '/Mem/{printf "%.2fG", $2/1024}')
    # 用户程序占用内存量
    mem_user=$(free -m | awk '/Mem/{printf "%.2fG", $3/1024}')
    #  多进程共享占用内存量
    mem_shared=$(free -m | awk '/Mem/{printf "%.2fG", $5/1024}')
    #缓存占用内存量
    mem_buff_cache=$(free -m | awk '/Mem/{printf "%.fMB",  $(NF-1)}')
    #空闲内存容量
    mem_free=$(free -m | awk '/Mem/{printf "%.2fG", $4/1024 }')
    # 剩余可用内存容量
    mem_available=$(free -m | awk 'NR==2{printf "%.2fG",$NF/1024}')
    # 可用内存使用占比
    mem_percentage=$(free -m | awk '/Mem/{printf "%.2f", $NF/$2*100}')
    #总的交换分区容量
    swap_total=$(free -m | awk '/Swap/{printf "%.2fG", $2/1024}')
    #用户使用的交换分区容量
    swap_user=$(free -m | awk '/Swap/{printf "%.2fG",$3/1024}')
    #剩余交换分区容量
    swap_free=$(free -m | awk '/Swap/{printf "%.2fG",$4/1024}')
    #可用交换分区占比
    swap_percentage=$(free -m | awk '/Swap/{printf "%.2f",$4/$2*100}')
    #占用内存资源最多的10个进程列表
    top_proc_mem=$(ps --no-headers -eo rss,args | sort -k1 -n -r | head -10)

    echo -e "\n\033[32m################## 内存 ############\033[0m\n"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|总内存容量:\033[31m        $mem_total           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|用户程序内存量:\033[31m   $mem_user        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|多进程共享内存量:\033[31m $mem_shared     \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|缓存占用内存量:\033[31m   $mem_buff_cache    \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|空闲内存容量:\033[31m     $mem_free           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|剩余可用内存容量:\033[31m  $mem_available    \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|可用内存百分比：\033[31m   $mem_percentage%   \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "\033[32m############## 交换分区 #############\033[0m\n"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "总的交换分区容量：\033[31m    $swap_total    \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|用户使用的交换分区容量：\033[31m   $swap_user   \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|剩余交换分区容量：\033[31m  ${swap_free}"
    echo -e "\033[32m------------------------------------\033[0m"
    if [ $(free -m | awk '/Swap/{print $2}') -ne 0 ]; then

        echo -e "|可用交换分区占比：\033[31m  $swap_percentage%    \033[0m"
        echo -e "\033[32m------------------------------------\033[0m"
    fi
    echo -e "|占用内存资源最多的10个进程列表:"
    echo -e "\033[31m$top_proc_mem%    \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
}

function cpu() {
    #CPU型号
    cpu_info=$(LANG=C lscpu | awk -F: '/Model name/ {print $2}')

    #CPU内核数量
    cpu_core=$(awk '/processor/{core++} END{print core}' /proc/cpuinfo)

    #CPU最近1/5/15分钟的平均负载
    load1515=$(uptime | sed 's/,/ /g' | awk '{for(i=NF-2;i<=NF;i++)print $i }' | xargs)

    #发生中断数量
    irq=$(vmstat 1 1 | awk 'NR==3{print $11}')

    #上下文切换数量
    cs=$(vmstat 1 1 | awk 'NR==3{print $12}')

    #占用CPU资源最多的10个进程列表
    top_proc_cpu=$(ps --no-headers -eo %cpu,args | sort -k1 -n -r | head -10)

    echo -e "\n\033[32m################## CPU 相关 ############\033[0m\n"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|CPU型号:\033[31m$cpu_info           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|CPU内核数量:\033[31m$cpu_core        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|CPU最近1/5/15分钟的平均负载:\033[31m$load1515        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|CPU中断数量:\033[31m$irq        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|上下文切换数量:\033[31m$cs        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|占用CPU资源最多的10个进程列表:"
    echo -e "\033[31m$top_proc_cpu           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"

}

function disk_io() {
    #分区挂载信息
    disk=$(df -h)
    # 磁盘总数
    disk_total=$(vmstat -D | awk 'NR==1{print $1}')
    # 分区总数
    disk_sub=$(vmstat -D | awk 'NR==2{print $1}')
    #磁盘分区信息
    lsblk_=$(lsblk -n)
    #写入磁盘的总块数
    bo=$(vmstat 1 1 | awk 'NR==3{print $10}')
    #从磁盘读出的块数
    bi=$(vmstat 1 1 | awk 'NR==3{print $9}')
    #每秒写磁盘块的速率
    wa=$(vmstat 1 1 | awk 'NR==3{print $16}')

    echo -e "\n\033[32m################## 磁盘IO 相关 ############\033[0m\n"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|磁盘总数:\033[31m$disk_total           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|分区总数:\033[31m$disk_sub        \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|磁盘分区信息:"
    echo -e "\033[31m$lsblk_           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|写入磁盘的总块数:\033[31m$bo       \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|从磁盘读出的块数:\033[31m$bi       \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|每秒写磁盘块的速率:\033[31m$wa       \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|分区挂载信息:"
    echo -e "\033[31m$disk           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"

}

function procs() {
    #进程数量
    procs=$(ps aux | wc -l)
    #用户的最大进程数
    ulimit_=$(ulimit -u)
    #内核设置的最大进程数
    pid_max=$(sysctl kernel.pid_max | awk '{print $3}')

    echo -e "\n\033[32m################## 进程 相关 ############\033[0m\n"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|进程数量:\033[31m$procs       \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|用户的最大进程数:\033[31m$ulimit_       \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|内核设置的最大进程数:\033[31m$pid_max       \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"

}

function network() {
    #获取网卡流量信息,接收|发送的数据流量,单位为字节(bytes)
    net_monitor=$(cat /proc/net/dev | tail -n +3 | awk 'BEGIN{ print "网卡名称 入站数据流量(bytes) 出站数据流量(bytes)"} {print $1,$2,$10}' | column -t)
    #获取暴露端口信息
    ip_port=$(ss -ntulpa)

     #本地IP地址列表
    localip=$(ip a s | awk '/inet /{print $2}' )


    echo -e "\n\033[32m################## 网络 相关 ############\033[0m\n"
    echo -e "|本地IP地址列表:"
    echo -e "\033[31m$localip    \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|获取网卡流量信息:"
    echo -e "\033[31m$net_monitor           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"
    echo -e "|获取暴露端口信息:"
    echo -e "\033[31m$ip_port           \033[0m"
    echo -e "\033[32m------------------------------------\033[0m"

}

case $1 in
all)
    bash_os
    memory
    cpu
    disk_io
    procs
    network
    ;;
bash_os)
    bash_os
    ;;
memory)
    memory
    ;;
cpu)
    cpu
    ;;
disk_io)
    disk_io
    ;;
procs)
    procs
    ;;
network)
    network
    ;;
*)
    echo "Usage: bash_os|memory|cup|disk_io|procs|network|all"
    ;;

esac
