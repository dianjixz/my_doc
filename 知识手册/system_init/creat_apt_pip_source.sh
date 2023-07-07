#!/bin/bash

pip config set global.index-url https://pypi.mirrors.ustc.edu.cn/simple/ 


sed -i 's/archive.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list


