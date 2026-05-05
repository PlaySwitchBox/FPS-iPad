cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES “arm64”)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(FPSChanger VERSION 1.0.0)

# Geode SDK einbinden

if (NOT DEFINED ENV{GEODE_SDK})
message(FATAL_ERROR “Bitte setze die Umgebungsvariable GEODE_SDK auf den Pfad zum Geode SDK.”)
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} geode-sdk)
create_geode_file(${PROJECT_NAME})
