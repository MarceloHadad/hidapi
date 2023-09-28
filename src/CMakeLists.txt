cmake_minimum_required(VERSION 3.20.0)

project(${CMAKE_PROJECT_NAME} VERSION 0.0.1)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(hidapi CONFIG REQUIRED)

add_executable(${CMAKE_PROJECT_NAME})

set(SRC_Main
    main.cpp
)
source_group("\\" FILES ${SRC_Main})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${SRC_Main}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE hidapi::libusb)

# hidapi::hidapi hidapi::hidraw hidapi::libusb hidapi::include