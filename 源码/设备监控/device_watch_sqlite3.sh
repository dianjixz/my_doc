#!/bin/bash


INIT_SQL=$(cat <<EOF
CREATE TABLE IF NOT EXISTS device (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    cpu_load REAL,
    mem_load TEXT,
    temperature REAL,
    insert_time DATETIME DEFAULT (datetime('now', 'localtime'))
);
EOF
)

sqlite3 device.db "$INIT_SQL"

while true;
do
cpu_load=`top -bn1 | grep "Cpu(s)" |   awk '{print $2 + $4}'`
mem_load=`free -m | awk '/Mem:/ {print $3"/"$2}'`
soc_temp=`cat /sys/class/thermal/thermal_zone0/temp`
echo "(cpu_load, mem_load, temperature) VALUES ($cpu_load, $mem_load, $soc_temp)"   
sqlite3_insert_cmd=`printf "INSERT INTO device (cpu_load, mem_load, temperature) VALUES (%f, '%s', %d);" $cpu_load $mem_load $soc_temp`
sqlite3 device.db "$sqlite3_insert_cmd"
sleep 1
done