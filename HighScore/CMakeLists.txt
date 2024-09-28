#cmake_minimum_required(VERSION 3.29)
#project(HighScore)
#
#set(CMAKE_CXX_STANDARD 20)
#
#add_executable(HighScore main.cpp)

cmake_minimum_required(VERSION 3.29)
project(HighScoreDLL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(HighScoreDLL SHARED HighScore.h main.cpp)

set_target_properties(HighScoreDLL PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)