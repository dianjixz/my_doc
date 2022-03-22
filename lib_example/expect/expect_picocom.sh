#!/usr/bin/expect


set timeout 60

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
set return_cal 1

while { $return_cal != 0 } {
    send "D1_init=\`ps aux | grep \"D1_init.sh\" | grep -v grep\`\r"
    sleep 0.5
    expect "#"
    send "if \[ \"\$D1_init\" == \"\" \];  then echo \"yes\";  else  echo \"no\";  fi\r"
    sleep 0.5
    expect {
        -timeout 1
        "yes" {
            set return_cal 0
            exp_continue
        }
        "no" {
            puts "D1_init process delte fail"
            set return_cal 1
        }
    }
    incr time_out_cnt
    if { $time_out_cnt > 5} {
        exit 1
    }
    puts $time_out_cnt
    sleep 0.5
}


set time_out_cnt 0
set return_cal 1

while {$return_cal != 0} {
    send "fdisk -l\r"
    sleep 0.5
    expect {
        -timeout 3
        "Disk /dev/sda" {
            set return_cal 0
        }
    }
    incr time_out_cnt
    if { $time_out_cnt > 5} {
        exit 1
    }
    sleep 0.5
}

send "mount /dev/sda1 /mnt -o rw \r"
sleep 0.5
expect "#"

set time_out_cnt 0
set return_cal 1


while {$return_cal != 0} {
    send "if \[ -f \"/mnt/test/test.sh\" \]; then echo \"yes\"; else echo \"no\"; fi \r"
    sleep 0.5
    expect {
        -timeout 1
        "yes" {
            set return_cal 0
            exp_continue
        }
        "no" {
            puts "file delte fail"
            set return_cal 2
        }
    }
    incr time_out_cnt;
    if { $time_out_cnt > 5} {
        exit 2;
    }
    sleep 1
}

send "/mnt/test/test.sh \r"
sleep 0.2
expect "#"

send "cat /mnt/run.log \r"
sleep 0.2
expect "#"

exit $return_cal



# D1_init=`ps aux | grep "D1_init.sh" | grep -v grep`;if [ "$D1_init" == "" ]; then echo "no"; else echo "yes"; fi


# if [ -f "/mnt/test/test.sh" ]; then echo "yes"; else echo "no"; fi

# expect {
#     -timeout 30
#     "#" {set return_cal 0}
#     timeout {set return_cal 2}
# }

# if { $return_cal != 0 } {
#     exit 3
# }

# send "chmod +x /root/test/test.sh && nohup \r"

# sleep 1
# expect "Password:" {send "iScas1001\r"}
# sleep 1

# expect "root@MaixLinux:~#" {send "ls\r"}
# sleep 1
# expect "root@MaixLinux:~#" {send "ls\r"}

# send "\x01\x11"

# set timeout 5
# set return_cal 0

# expect {
#     -timeout 3
#     "nihao" {send "request timeout"}
#     timeout {set return_cal 1}
# }

# send_user "nihao"

# exit $return_cal
# expect "#"
# # sleep 0.2
# interact


# exit


# EOF