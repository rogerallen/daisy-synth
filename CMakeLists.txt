cmake_minimum_required(VERSION 3.0)
project(cimgui-sokol-starterkit)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
# Linux -pthread shenanigans
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

#=== LIBRARY: cimgui + Dear ImGui
add_library(cimgui STATIC
    cimgui/cimgui.cpp
    cimgui/cimgui.h
    cimgui/imgui/imgui.cpp
    cimgui/imgui/imgui.h
    cimgui/imgui/imgui_widgets.cpp
    cimgui/imgui/imgui_draw.cpp
    cimgui/imgui/imgui_tables.cpp
    cimgui/imgui/imgui_demo.cpp)
target_include_directories(cimgui INTERFACE cimgui)

#=== LIBRARY: sokol
# add headers to the the file list because they are useful to have in IDEs
set(SOKOL_HEADERS
    sokol/sokol_gfx.h
    sokol/sokol_app.h
    sokol/sokol_imgui.h
    sokol/sokol_glue.h
    sokol/sokol_audio.h
    sokol/sokol_gl.h)
if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    add_library(sokol STATIC sokol/sokol.c ${SOKOL_HEADERS})
    target_compile_options(sokol PRIVATE -x objective-c)
    target_link_libraries(sokol PUBLIC
        "-framework QuartzCore"
        "-framework Cocoa"
        "-framework MetalKit"
        "-framework Metal")
else()
    add_library(sokol STATIC sokol/sokol.c ${SOKOL_HEADERS})
    if (CMAKE_SYSTEM_NAME STREQUAL Linux)
        target_link_libraries(sokol INTERFACE X11 Xi Xcursor GL dl m asound)
        target_link_libraries(sokol PUBLIC Threads::Threads)
    endif()
endif()
target_link_libraries(sokol PUBLIC cimgui)
target_include_directories(sokol INTERFACE sokol)

#=== DaisySP
# must be cloned & built in DAISYSP_DIR
set(DAISYSP_DIR ${CMAKE_SOURCE_DIR}/../DaisySP)
#   lib in ../DaisySP/build/libDaisySP.a
#   incs in ../DaisySP/Source/daisysp.h

#=== EXECUTABLE: demo
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_executable(demo WIN32 demo/demo.c demo/synth.cpp demo/synth.h common/keyboard_piano.c common/keyboard_piano.h)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT demo)
else()
    add_executable(demo demo/demo.c demo/synth.cpp demo/synth.h common/keyboard_piano.c common/keyboard_piano.h)
endif()
target_include_directories(demo PRIVATE ${DAISYSP_DIR}/Source common)
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    target_link_libraries(demo sokol ${DAISYSP_DIR}/build/libDaisySP.a)
endif()
if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    target_link_libraries(demo sokol ${DAISYSP_DIR}/build/$(ConfigurationName)/DaisySP.lib)
endif()

#=== EXECUTABLE: s1
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_executable(s1 WIN32 s1/s1.c s1/synth.cpp s1/synth.h common/keyboard_piano.c common/keyboard_piano.h)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT s1)
else()
    add_executable(s1 s1/s1.c s1/synth.cpp s1/synth.h common/keyboard_piano.c common/keyboard_piano.h)
endif()
target_include_directories(s1 PRIVATE ${DAISYSP_DIR}/Source common)
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    target_link_libraries(s1 sokol ${DAISYSP_DIR}/build/libDaisySP.a)
endif()
if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    target_link_libraries(s1 sokol ${DAISYSP_DIR}/build/$(ConfigurationName)/DaisySP.lib)
endif()

# Emscripten-specific linker options
#if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
#    set(CMAKE_EXECUTABLE_SUFFIX ".html")
#    # use our own minimal shell.html
#    target_link_options(demo PRIVATE --shell-file ../sokol/shell.html)
#    # link with WebGL2
#    target_link_options(demo PRIVATE -sUSE_WEBGL2=1)
#    # WASM+JS size optimizations
#    target_link_options(demo PRIVATE -sNO_FILESYSTEM=1 -sASSERTIONS=0 -sMALLOC=emmalloc --closure=1)
#endif()

# explicitly strip dead code
if (CMAKE_C_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    target_link_options(demo PRIVATE LINKER:-dead_strip)
    target_link_options(s1 PRIVATE LINKER:-dead_strip)
endif()

# this hack removes the xxx-CMakeForceLinker.cxx dummy file
set_target_properties(demo PROPERTIES LINKER_LANGUAGE C)
set_target_properties(s1 PROPERTIES LINKER_LANGUAGE C)
