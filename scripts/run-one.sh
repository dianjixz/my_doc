#!/bin/bash
docker run -d -p 6666:22 myubuntu:16.04 /usr/sbin/sshd -D
./test.sh
id=$(docker ps | grep myubuntu | awk '{print $1}')

docker commit $id myubuntu:16.04

docker stop $id









exit 0
