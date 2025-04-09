cmake_minimum_required(VERSION 3.19)

project(Project1 LANGUAGES CXX)

find_package(OpenMP COMPONENTS CXX REQUIRED)

add_executable(Project1 Project1.cpp)
target_link_libraries(Project1 PRIVATE OpenMP::OpenMP_CXX)
target_compile_definitions(Project1 PRIVATE NUMT=${NUMT})