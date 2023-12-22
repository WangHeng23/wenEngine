aux_source_directory(./src/wen/core SRC)
aux_source_directory(./src/wen/imgui SRC)
aux_source_directory(./src/platform/windows SRC)
# message(STATUS "wen src: ${SRC}")

file(GLOB_RECURSE HEADERS "./include/*.hpp")
# message(STATUS "wen headers: ${HEADERS}")

#############imgui#############
file(GLOB SRC_LIST_IMGUI ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/*.cpp ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/*.h) 
set(SRC_LIST_IMGUI_BACKENDS
        ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/backends/imgui_impl_opengl3.h
        ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/backends/imgui_impl_opengl3_loader.h
        ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/backends/imgui_impl_glfw.h
        ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/backends/imgui_impl_glfw.cpp
)
##############################

add_library(wen STATIC ${SRC} ${HEADERS} ${SRC_LIST_IMGUI} ${SRC_LIST_IMGUI_BACKENDS})

target_link_libraries(wen PUBLIC glfw glad)

target_include_directories(wen PUBLIC ./include
                                      ${CMAKE_SOURCE_DIR}/3rdlibs/spdlog/include
                                      ${CMAKE_SOURCE_DIR}/3rdlibs/imgui
                                      ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/backends)

target_precompile_headers(wen PUBLIC ./include/pch.hpp)