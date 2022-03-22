#!/usr/bin/expect
set timeout 60
spawn picocom /dev/ttyUSB0 -b 115200
sleep 0.2
send "\r"
expect {
    "Password:"             {send "iScas1001\r" ;   exp_continue}
    "MaixLinux login:"      {send "root\r" ;        exp_continue}
    "root@MaixLinux:~#"      {send "ls\r"}
}
set timeout 30
expect "#"

set time_out_cnt 0

set return_cal 1

while {$return_cal != 0} {
    send "fdisk -l\r"
    expect {
        -timeout 1
        "Disk /dev/sda" {set return_cal 0}
        timeout {set return_cal 1}
    }
    incr time_out_cnt
    if { $time_out_cnt > 60} {
        break;
    }
    sleep 1
}
if { $return_cal == 0 } {
    send "mount /dev/sda1 /mnt -o rw "
    expect "#"
}


# set timeout 1
# expect "#"
exit $return_cal