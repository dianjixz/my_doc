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




# toolChain_json.cmake
# set(CMAKE_SYSTEM_NAME Linux)
# SET(TOOLCHAIN_DIR "/opt/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu")
# set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-linux-gnu-gcc)
# set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-linux-gnu-g++)
# set(CMAKE_FIND_ROOT_PATH "/home/nihao/workdir/ubus/json-c")
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# toolChain_libubox.cmake
# set(CMAKE_SYSTEM_NAME Linux)
# SET(TOOLCHAIN_DIR "/opt/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu")
# set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-linux-gnu-gcc)
# set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-linux-gnu-g++)
# set(INSTALL_PATH "/home/nihao/workdir/ubus/dist")
# include_directories(${INSTALL_PATH}/include)
# include_directories(${INSTALL_PATH}/include/json-c)
# set(CMAKE_FIND_ROOT_PATH "/home/nihao/workdir/ubus/dist")
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(json "/home/nihao/workdir/ubus/dist/lib/libjson-c.so")

# toolChain_ubus.cmake
# set(CMAKE_SYSTEM_NAME Linux)
# SET(TOOLCHAIN_DIR "/opt/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu")
# set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-linux-gnu-gcc)
# set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-linux-gnu-g++)
# set(INSTALL_PATH "/home/nihao/workdir/ubus/dist")
# include_directories(${INSTALL_PATH}/include)
# set(CMAKE_FIND_ROOT_PATH "/home/nihao/workdir/ubus/dist" $(INSTALL_PATH))
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(blob_library ${INSTALL_PATH}/lib/libblobmsg_json.so)
# set(json ${INSTALL_PATH}/lib/libjson-c.so)
# set(ubox_include_dir ${INSTALL_PATH}/include/libubox)
# set(ubox_library ${INSTALL_PATH}/lib/libubox.so)















