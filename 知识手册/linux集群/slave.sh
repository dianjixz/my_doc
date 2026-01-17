#!/bin/bash
SERVER_IP=192.168.1.1
SERVER_SSH_PORT=22
SERVER_USER=m5stack
SERVER_PASSWD=''

SLAVE_DEVICE_ID=$(cat /var/.device_id)

if [ -z "$SLAVE_DEVICE_ID" ]; then
    SLAVE_DEVICE_ID=$(ssh $SERVER_USER@$SERVER_IP -p $SERVER_SSH_PORT "uuidgen")
    echo $SLAVE_DEVICE_ID > /var/.device_id
fi

if [ -z "$SLAVE_DEVICE_ID" ]; then
    echo "获取设备ID失败"
    exit 1
fi
remote_port=$(ssh $SERVER_USER@$SERVER_IP -p $SERVER_SSH_PORT "SELECT v FROM kv WHERE k='your_key';")



my_device_id=$(hostname)
my_device_ip=$(ip addr | grep 'state UP' -A2 | tail -n1 | awk '{print $2}' | cut -f1  -d'/')
my_device_name=$(hostname)
my_device_type='linux'
my_device_status='online'




my_device_id=''