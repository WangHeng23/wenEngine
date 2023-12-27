cmake_minimum_required(VERSION 3.25)

project(sandbox)

set(SRC ./sandboxApp.cpp
        ./exampleLayer.cpp ./exampleLayer.hpp
        ./sandbox2d.cpp ./sandbox2d.hpp
        ./game01/gameLayer.cpp ./game01/level.cpp 
        ./game01/gameLayer.hpp ./game01/level.hpp
        ./game01/player.cpp ./game01/player.hpp
        ./game01/random.cpp ./game01/random.hpp
        ./game01/particleSystem.cpp ./game01/particleSystem.hpp)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE wen)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/wen/include)
target_precompile_headers(${PROJECT_NAME} REUSE_FROM wen)