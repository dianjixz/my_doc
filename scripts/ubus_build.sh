#!/bin/bash

git clone https://github.com/json-c/json-c.git
git clone http://git.openwrt.org/project/libubox.git
git clone https://git.openwrt.org/project/ubus.git


cd json-c
cmake -DCMAKE_INSTALL_PREFIX=/home/nihao/workdir/ubus/dist -DCMAKE_TOOLCHAIN_FILE=../toolChain_json.cmake .
make
make install
cd ..
ln -s /home/nihao/workdir/ubus/dist/include/json-c /home/nihao/workdir/ubus/dist/include/json

cd libubox
cmake -DBUILD_LUA=OFF -DCMAKE_INSTALL_PREFIX=/home/nihao/workdir/ubus/dist -DCMAKE_TOOLCHAIN_FILE=../toolChain_libubox.cmake .
make
make install
cd ..


cd ubus
cmake -DBUILD_LUA=OFF -DCMAKE_INSTALL_PREFIX=/home/nihao/workdir/ubus/dist -DCMAKE_TOOLCHAIN_FILE=../toolChain_ubus.cmake .
make
make install
cd ..




























