#cmake_minimum_required(VERSION 3.29)
#project(snakeGame)
#
#set(CMAKE_CXX_STANDARD 20)
#
#link_directories(${CMAKE_BINARY_DIR})
#include_directories(${CMAKE_SOURCE_DIR})
#
#add_executable(snakeGame main.cpp
#        SnakeGame.rc
#        resource.h
#        HighScore.h)
#
#target_link_libraries(snakeGame HighScoreDLL kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32)

cmake_minimum_required(VERSION 3.29)
project(snakeGame)

set(CMAKE_CXX_STANDARD 20)

# Set the runtime output directory to cmake-build-debug
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add your source files
add_executable(snakeGame
        main.cpp
        SnakeGame.rc
        resource.h
        HighScore.h
)

# Specify the location of the DLL
set(HIGH_SCORE_DLL "${CMAKE_BINARY_DIR}/libHighScoreDLL.dll")

# Link against the DLL
target_link_libraries(snakeGame
        ${HIGH_SCORE_DLL}
        kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32
)

# No need to copy the DLL as it's already in the correct location

