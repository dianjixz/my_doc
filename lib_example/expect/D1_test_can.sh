#!/usr/bin/expect

 
spawn echo "no"

set mk 0

expect {
  "yes" { puts "a";set mk 1;exp_continue }
  "no" { set mk 2 ; puts "b"}
#   a* {
#     puts a*
#     exp_continue
#   }
#   eof {}
#   timeout {}
}
 
puts "nihao$mk"
 