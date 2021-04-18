#!/usr/bin/expect

set timeout 10
spawn ssh nihao@127.0.0.1 -p 6666
expect "*password*"
send "nihao\r"

expect "nihao*"

interact


#expect "$"
#send "service scm-agent status\r"
#send "echo \"nihao\" >> 1.log \r"
#expect "$"
#send "ls\r"
#expect "$"
#send "exit\r"
exit

