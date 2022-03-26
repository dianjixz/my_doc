#!/usr/bin/expect

set timeout 60
spawn ssh root@192.168.10.2
expect {
    "(yes/no)" {send "yes\r"; exp_continue}
    "password:" {send "iScas1001\r"}
}
set return_cal 1


expect "#"  {send "exit\r";set return_cal 0}
expect eof


if {$return_cal == 0} {
    puts "ssh success!";
    exit 0
}

if {$return_cal != 0} {
    puts "ssh fail!";
    exit 1
}