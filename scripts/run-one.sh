#!/bin/bash
docker run -d -p 6666:22 myubuntu:16.04 /usr/sbin/sshd -D
./test.sh
id=$(docker ps | grep myubuntu | awk '{print $1}')

docker commit $id myubuntu:16.04

docker stop $id



docker run -d -p 6666:6666  -p 6667:6667  -v /home/nihao/work:/home/nihao/work  myubuntu:18.04 /usr/sbin/sshd -D





exit 0
