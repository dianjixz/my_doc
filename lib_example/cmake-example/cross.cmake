# cross.cmake文件
# 设置为1则表示交叉编译，设置为0则表示x86 gcc编译
SET(CROSS_COMPILE 1)

IF(CROSS_COMPILE)
SET(CMAKE_SYSTEM_NAME Linux)
SET(TOOLCHAIN_DIR "/opt/hisi-linux/x86-arm/arm-hisiv300-linux/target")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-hisiv300-linux-g++)
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/arm-hisiv300-linux-gcc)

#set(GNU_FLAGS "-mfpu=vfp -fPIC")
#set(CMAKE_CXX_FLAGS "${GNU_FLAGS} ")
#set(CMAKE_C_FLAGS "${GNU_FLAGS}  ")

SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

ENDIF(CROSS_COMPILE)
# 使用时在 CMakeLists.txt 中 include 该文件就可以！
# include (./cross.cmake)