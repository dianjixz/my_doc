#!/usr/bin/expect

spawn picocom /dev/ttyUSB0 -b 115200
sleep 0.5
send "\r"
sleep 0.5
expect {
    "Password:"             {send "iScas1001\r" ;   exp_continue}
    "MaixLinux login:"      {send "root\r" ;        exp_continue}
    "root@MaixLinux:~#"      {send "ls\r"}
}
sleep 0.5
expect "#"

set time_out_cnt 0
set return_cal 0

send "  \r"

