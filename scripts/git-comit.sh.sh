#!/usr/bin/expect

set timeout 10
spawn git push
expect "*https://*"
send "\r"
expect "*https://*"
send "\r"

expect







exit

