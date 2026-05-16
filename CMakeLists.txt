cmake_minimum_required(VERSION 4.0)
project(aperture LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(extern)
add_subdirectory(src)