#!/usr/bin/expect

spawn bluetoothctl

sleep 0.5

set time_out_cnt 0
set return_cal 0

expect "#" {send "power on\r"}
sleep 0.5
expect "#" {send "discoverable on\r"}
sleep 0.5


wait



# send "\r"
# sleep 0.5
# expect {
#     "Password:"             {send "iScas1001\r" ;   exp_continue}
#     "MaixLinux login:"      {send "root\r" ;        exp_continue}
#     "root@MaixLinux:~#"      {send "ls\r"}
# }
# sleep 0.5
# expect "#"

# set time_out_cnt 0
# set return_cal 0

# send "  \r"


# bluetoothctl

# power on

# discoverable on
