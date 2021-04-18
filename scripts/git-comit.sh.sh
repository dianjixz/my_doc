#!/usr/bin/expect

set timeout 10
spawn git push
expect "*https://github.com*"
send "\r"
expect "*https://dianjixz@github.com*"
send "\r"

expect





#spawn ssh nihao@127.0.0.1 -p 6666
#expect "*password*"
#send "\r"

#expect "*"

#interact


exit

