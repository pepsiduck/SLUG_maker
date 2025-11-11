cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(SLUG_maker)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb -Wall")
set(CMAKE_C_STANDARD 11)  
# Add raylib
option(BUILD_EXAMPLES "Build examples" OFF)
# option(SUPPORT_MODULE_RAUDIO "Build RAudio" OFF)
add_subdirectory(vendor/raylib)


# Create the executable
file(GLOB_RECURSE SOURCES src/**/*.c src/*.c src/raygui.c)
add_executable(SLUG_maker ${SOURCES} )

# Include raylib headers
target_include_directories(SLUG_maker PRIVATE vendor/raylib/include)
target_include_directories(SLUG_maker PRIVATE vendor/raylib/src/external)
target_include_directories(SLUG_maker PRIVATE src)

# Link raylib
target_link_libraries(SLUG_maker PRIVATE raylib)
