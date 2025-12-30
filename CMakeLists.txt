# AI generated CMakeLists.txt, I have no idea how to write these, sorry

cmake_minimum_required(VERSION 3.10)
project(day-tracker LANGUAGES C)

# Use a modern C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Collect all .c files in src/ (non-recursive). Use GLOB_RECURSE if you want subdirectories.
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")

if(NOT SRC_FILES)
    message(FATAL_ERROR "No .c sources found in ${CMAKE_SOURCE_DIR}/src")
endif()

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include headers from src/include
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/include")

# Find and link libcrypt (fall back to linking "crypt" if find_library fails)
find_library(CRYPT_LIB NAMES crypt)
if(CRYPT_LIB)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CRYPT_LIB})
else()
    message(WARNING "libcrypt not found via find_library; linking against -lcrypt name instead")
    target_link_libraries(${PROJECT_NAME} PRIVATE crypt)
endif()

