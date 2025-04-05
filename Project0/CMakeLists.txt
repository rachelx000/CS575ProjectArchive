cmake_minimum_required(VERSION 3.19)

project(Project0 LANGUAGES CXX)

find_package(OpenMP COMPONENTS CXX REQUIRED)

add_executable(Project0 Project0.cpp)
target_link_libraries(Project0 PRIVATE OpenMP::OpenMP_CXX)
target_compile_definitions(Project0 PRIVATE NUMT=${NUMT})