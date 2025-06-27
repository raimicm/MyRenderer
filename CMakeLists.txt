cmake_minimum_required(VERSION 3.16)
project(MyRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SDL)
add_subdirectory(external/glm)

add_executable(MyRenderer src/main.cpp)

target_link_libraries(MyRenderer PRIVATE SDL3::SDL3)
target_link_libraries(MyRenderer PRIVATE glm)