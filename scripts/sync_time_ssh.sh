#!/bin/bash
passworld=""
remote_ip="192.168.8.188"
hostname="root"

tmp_file=`mktemp`
date +"%Y-%m-%d %H:%M:%S" > $tmp_file

if [ "$passworld" == "" ] ; then
    scp $tmp_file $hostname@$remote_ip:$tmp_file
else
    sshpass -p $passworld scp $tmp_file $hostname@$remote_ip:$tmp_file
fi

cmdstr="date -s \"\$(cat $tmp_file)\""

if [ "$passworld" == "" ] ; then
    ssh $hostname@$remote_ip $cmdstr
else
    sshpass -p $passworld ssh $hostname@$remote_ip $cmdstr
fi

rm $tmp_file
