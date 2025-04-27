cmake_minimum_required(VERSION 3.19)

project(Project2 LANGUAGES CXX)

find_package(OpenMP COMPONENTS CXX REQUIRED)

add_executable(Project2 Project2.cpp)
target_link_libraries(Project2 PRIVATE OpenMP::OpenMP_CXX)