# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)





# 项目信息
project (Demo1)
# 添加版本号
# 首先修改顶层 CMakeLists 文件，在 project 命令之后加入如下两行：
# set (Demo_VERSION_MAJOR 1)
# set (Demo_VERSION_MINOR 0)



# 支持gdb
# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")



# 加入一个配置头文件，用于处理 CMake 对源码的设置
# configure_file (
#   "${PROJECT_SOURCE_DIR}/config.h.in"
#   "${PROJECT_BINARY_DIR}/config.h"
#   )
# 是否使用自己的 MathFunctions 库
# option (USE_MYMATH "Use provided math implementation" ON) # 设置为ON，或OFF



# 设置变量

# set 直接设置变量的值
# set(SRC_LIST main.cpp test.cpp)
# set 追加设置变量的值
# set(SRC_LIST ${SRC_LIST} test.cpp)

# 常用变量
# CMAKE_C_COMPILER              : c 编译器名字,默认 /usr/bin/cc
# CMAKE_CXX_COMPILER            : c++ 编译器名字,默认 /usr/bin/c++
# CMAKE_CXX_STANDARD            : c++ std版本
# CMAKE_CXX_FLAGS               ：设置 'C++ 编译'选项,也可以通过指令 add_definitions() 添加
# CMAKE_C_FLAGS                 ：设置 'C 编译'选项,也可以通过指令 add_definitions() 添加
# CMAKE_BUILD_TYPE              : 控制构建类型。可选参数：None: default , Debug: 生成调试信息, Release: 发布版本，进行最佳化

# CMAKE_INCLUDE_PATH            : 头文件搜索路径. 默认为空。配合 find_file() 以及 find_path 使用
# CMAKE_LIBRARY_PATH            : 库文件搜索路径. 默认为空。配合 find_library() 使用
# CMAKE_INSTALL_PREFIX          : cmake 安装的路径, 默认 /usr/local
# BUILD_SHARED_LIBS             : 将所有程序库的 target 设置为共享库。如果未设置，使用 ADD_LIBRARY 时又没有指定库类型，默认编译生成静态库

# CMAKE_MAJOR_VERSION           ：cmake 主版本号，比如 3.4.1 中的 3
# CMAKE_MINOR_VERSION           ：cmake 次版本号，比如 3.4.1 中的 4
# CMAKE_PATCH_VERSION           ：cmake 补丁等级，比如 3.4.1 中的 1
# CMAKE_SYSTEM                  ：系统名称，比如 Linux-­2.6.22
# CMAKE_SYSTEM_NAME             ：'不包含版本的系统名'，比如 Linux
# CMAKE_SYSTEM_VERSION          ：'系统(内核)版本'，比如 2.6.22
# CMAKE_SYSTEM_PROCESSOR        ：'处理器名称',比如 i686
# UNIX                           ：在所有的类 UNIX 平台下该值为 'TRUE',包括 OS X 和 cygwin
# WIN32                         ：在所有的 win32 平台下该值为 TRUE,包括 cygwin


# PROJECT_SOURCE_DIR            ：工程的根目录
# PROJECT_BINARY_DIR            ：运行cmake命令的目录，通常为${PROJECT_SOURCE_DIR}/build
# PROJECT_NAME                  ：返回通过 project 命令定义的项目名称
# CMAKE_CURRENT_SOURCE_DIR      ：当前处理的 CMakeLists.txt 所在的路径
# CMAKE_CURRENT_BINARY_DIR      ：target 编译目录
# CMAKE_CURRENT_LIST_DIR        ：CMakeLists.txt 的完整路径
# EXECUTABLE_OUTPUT_PATH        ：重新定义目标二进制可执行文件的存放位置
# LIBRARY_OUTPUT_PATH           ：重新定义目标链接库文件的存放位置
# CMAKE_PREFIX_PATH             : 指定要搜索的安装前缀的目录 find_package()， find_program()， find_library()， find_file()，和 find_path() 命令。每个命令将添加相应的子目录（例如 bin，lib 或 include），作为其自己的文档中指定。默认空,由项目设定.
# CMAKE_MODULE_PATH             : cmake 为上百个软件包提供了查找器 (finder):FindXXXX.cmake 当使用非 cmake 自带的 finder 时，需要指定 finder 的路径，这就是 CMAKE_MODULE_PATH，配合 FIND_PACKAGE() 使用
# CMAKE_CURRENT_LIST_FILE       : 输出调用这个变量的CMakeLists.txt的完整路径
# CMAKE_CURRENT_LIST_LINE       : 输出这个变量所在的行



# add_definitions(-DENABLE_DEBUG -DABC) 参数之间用'空格'分隔




# 添加文件到源文件列表
# aux_source_directory(. SRC_LIST) # 搜索目录下的所有.cpp文件
# or
# file(GLOB SRC_LIST "*.cpp" "protocol/*.cpp") 自定义搜索目录下的所有.cpp文件

# 查找指定的库文件
# find_library(VAR name path)查找到指定的预编译库，并将它的路径存储在变量中。默认的搜索路径为 cmake 包含的系统库，因此如果是 NDK 的公共库只需要指定库的name 即可（不需path）。
# 类似的命令还有 find_file()、find_path()、find_program()、find_package()。
# find_library(log-lib,log)

# 设置包含的目录
# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_BINARY_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# 添加子目录
# add_subdirectory(math)




# 指定生成目标
add_executable(demo demo.cpp) # 生成可执行文件
# add_library(common STATIC util.cpp) # 生成静态库
# add_library(common SHARED util.cpp) # 生成动态库或共享库


# 设置target需要链接的库
# target_link_libraries( # 目标库 demo # 目标库需要链接的库 ${log-lib} )
# 指定链接动态库或静态库
# target_link_libraries(demo libface.a) # 链接libface.a
# target_link_libraries(demo libface.so) # 链接libface.so
# 指定全路径
# target_link_libraries(demo ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.a)
# target_link_libraries(demo ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.so)
#  指定链接多个库
# target_link_libraries(demo
#     ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.a
#     boost_system.a
#     boost_thread
#     pthread)



# 安装和测试
# 指定 MathFunctions 库的安装路径
# install (TARGETS MathFunctions DESTINATION bin)
# install (FILES MathFunctions.h DESTINATION include)

# 指定安装路径
# install (TARGETS Demo DESTINATION bin)
# install (FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)


# 启用测试
# enable_testing()

# 测试程序是否成功运行
# add_test (test_run Demo 5 2)

# 测试帮助信息是否可以正常提示
# add_test (test_usage Demo)
# set_tests_properties (test_usage
#   PROPERTIES PASS_REGULAR_EXPRESSION "Usage: .* base exponent")

# 测试 5 的平方
# add_test (test_5_2 Demo 5 2)

# set_tests_properties (test_5_2
#  PROPERTIES PASS_REGULAR_EXPRESSION "is 25")

# 测试 10 的 5 次方
# add_test (test_10_5 Demo 10 5)

# set_tests_properties (test_10_5
#  PROPERTIES PASS_REGULAR_EXPRESSION "is 100000")

# PASS_REGULAR_EXPRESSION用来测试输出是否包含后面跟着的字符串。

# 定义一个宏，用来简化测试工作

# macro (do_test arg1 arg2 result)
#   add_test (test_${arg1}_${arg2} Demo ${arg1} ${arg2})
#   set_tests_properties (test_${arg1}_${arg2}
#     PROPERTIES PASS_REGULAR_EXPRESSION ${result})
# endmacro (do_test)
#  
# # 使用该宏进行一系列的数据测试
# do_test (5 2 "is 25")
# do_test (10 5 "is 100000")


# 生成安装包
# 为了完成这个任务，我们需要用到 CPack ，它同样也是由 CMake 提供的一个工具，专门用于打包。

# 首先在顶层的 CMakeLists.txt 文件尾部添加下面几行：
# 构建一个 CPack 安装包

# include (InstallRequiredSystemLibraries)
# set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
# set (CPACK_PACKAGE_VERSION_MAJOR "${Demo_VERSION_MAJOR}")
# set (CPACK_PACKAGE_VERSION_MINOR "${Demo_VERSION_MINOR}")
# include (CPack)

# 上面的代码做了以下几个工作：
# 
# 导入 InstallRequiredSystemLibraries 模块，以便之后导入 CPack 模块；
# 设置一些 CPack 相关变量，包括版权信息和版本信息，其中版本信息用了上一节定义的版本号；
# 导入 CPack 模块。
# 生成二进制安装包：
# cpack -C CPackConfig.cmake
# 生成源码安装包
# cpack -C CPackSourceConfig.cmake


# set_target_properties(Hazel PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})    : 设定连接参数