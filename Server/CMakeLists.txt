cmake_minimum_required(VERSION 3.10)

project(AutoRepairServer VERSION 0.1)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB Alg_Sources src/algorithms/*)
file(GLOB Util_Sources src/utils/*)
file(GLOB CtrlCenter_Sources src/controlcenter/*)


add_executable(AutoRepairServer   src/main.cpp 
                            # ${Alg_Sources} 
                            ${Util_Sources} 
                            ${CtrlCenter_Sources}
                            )
include_directories(src/)
target_include_directories(AutoRepairServer PUBLIC include)