#!/usr/bin/expect

set timeout 10
spawn ssh nihao@127.0.0.1 -p 6666
expect "*password*"
send "nihao\r"

expect "nihao*"

interact


exit

