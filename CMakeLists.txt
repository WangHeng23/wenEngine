cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

project(
    wenEngine
    DESCRIPTION "wenEngine"
    LANGUAGES CXX C
)

add_subdirectory(3rdlibs)
add_subdirectory(wen)

option(WEN_BUILD_SANDBOX "build sandbox" ON)
if (WEN_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()