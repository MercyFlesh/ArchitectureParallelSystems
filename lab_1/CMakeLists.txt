cmake_minimum_required(VERSION 3.8)
project(lab1 VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(PROJECT_SOURCES
        main.cxx
        MultiplyMatrix.hpp
)

add_executable(main ${PROJECT_SOURCES})