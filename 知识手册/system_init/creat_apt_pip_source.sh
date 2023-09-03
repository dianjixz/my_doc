#!/bin/bash

pip config set global.index-url https://pypi.mirrors.ustc.edu.cn/simple/ 


sed -i 's/archive.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list


sed -i 's/mirrors.ustc.edu.cn/archive.ubuntu.com/g' /etc/apt/sources.list