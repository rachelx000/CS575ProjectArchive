cmake_minimum_required(VERSION 3.19)

project(Project3 LANGUAGES CXX)

find_package(OpenMP COMPONENTS CXX REQUIRED)

add_executable(Project3 proj03.cpp)
target_link_libraries(Project3 PRIVATE OpenMP::OpenMP_CXX)