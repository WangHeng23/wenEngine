aux_source_directory(./src/wen/core SRC)
aux_source_directory(./src/wen/imgui SRC)
aux_source_directory(./src/wen/renderer SRC)
aux_source_directory(./src/platform/windows SRC)
aux_source_directory(./src/platform/opengl SRC)
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

############stb_image########
file(GLOB SRC_LIST_STB_IMAGE ${CMAKE_SOURCE_DIR}/3rdlibs/stb_image/*.cpp vendor/stb_image/*.h)
##############################

add_library(wen STATIC ${SRC} ${HEADERS} ${SRC_LIST_IMGUI} ${SRC_LIST_IMGUI_BACKENDS} ${SRC_LIST_STB_IMAGE})

target_link_libraries(wen PUBLIC glfw glad glm)

target_include_directories(wen PUBLIC ./include
                                      ${CMAKE_SOURCE_DIR}/3rdlibs/spdlog/include
                                      ${CMAKE_SOURCE_DIR}/3rdlibs/imgui
                                      ${CMAKE_SOURCE_DIR}/3rdlibs/imgui/backends
                                      ${CMAKE_SOURCE_DIR}/3rdlibs/stb_image)

target_precompile_headers(wen PUBLIC ./include/pch.hpp)