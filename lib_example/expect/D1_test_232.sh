#!/usr/bin/expect

spawn picocom /dev/ttyUSB0 -b 115200
sleep 0.2
send "\r"
expect {
    "Password:"             {send "iScas1001\r" ;   exp_continue}
    "MaixLinux login:"      {send "root\r" ;        exp_continue}
    "root@MaixLinux:~#"      {send "ls\r"}
}

expect "#"

set time_out_cnt 0
set return_cal 0


send "D1_init=\`ps aux | grep \"D1_init.sh\" | grep -v grep\`\r"
expect "#"
send "if \[ \"\$D1_init\" == \"mh\" \];  then echo \"yes\";  else  echo \"no\";  fi\r"

expect {
    "no" {
        set return_cal 1
        exp_continue
    }
    "yes" {
        set return_cal 2
    }
}

puts "nihao$return_cal"
exit 



# D1_init=`ps aux | grep "D1_init.sh" | grep -v grep`

# if \[ \"\$D1_init\" == \"as\" \];  then echo \"yes\";  else  echo \"no\";  fi

# if [ "$D1_init" == "as" ]; 
# then echo "yes"; 
# else 
# echo "no"; 
# fi