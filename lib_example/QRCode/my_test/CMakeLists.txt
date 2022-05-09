# cmake needs this line
cmake_minimum_required(VERSION 3.1)
 
# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
 
project(qr)
find_package(OpenCV REQUIRED)
FILE(GLOB SC_FILES "*.cpp" "*.h")
add_executable(${PROJECT_NAME} ${SC_FILES})
target_link_libraries(qr PRIVATE ${OpenCV_LIBS})
