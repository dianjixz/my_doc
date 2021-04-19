#!/usr/bin/expect

set timeout 10
spawn git push
expect "Username*"
send "dianjixz\r"
expect "Password*"
send "lgx19981124lgx\r"

expect







exit

