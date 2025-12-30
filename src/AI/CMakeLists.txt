cmake_minimum_required(VERSION 3.14)
project(zappy_ai LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES "*.cpp")
file(GLOB BRAIN_SOURCES "brain/*.cpp")

add_executable(zappy_ai
    ${SOURCES}
    ${BRAIN_SOURCES}
)

target_include_directories(zappy_ai PRIVATE src)
