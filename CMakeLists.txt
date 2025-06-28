cmake_minimum_required(VERSION 3.16)
project(MyRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_SHARED OFF CACHE BOOL "Disable SDL3 shared library (DLL)")  
set(SDL_STATIC ON CACHE BOOL "Enable SDL3 static library")  

add_subdirectory(external/SDL)
add_subdirectory(external/glm)

add_executable(objRenderer src/main.cpp src/canvas.cpp)

target_include_directories(objRenderer PRIVATE include/)

target_link_libraries(objRenderer PRIVATE SDL3::SDL3)
target_link_libraries(objRenderer PRIVATE glm)