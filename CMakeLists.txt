cmake_minimum_required(VERSION 3.20)

project(HttpCerver LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create the library
add_library(libs STATIC
    libs/classes/source/classes.c
)

# Specify where the library headers are located
target_include_directories(libs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/classes/headers

)

# Create the executable
add_executable(server
    Http-Cerver.c
    routing/source/routing.c
)

# for basic dev workflow
target_compile_options(server PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
#warnings for libs 
target_compile_options(libs PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(server PUBLIC libs)