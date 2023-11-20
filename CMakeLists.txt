cmake_minimum_required(VERSION 3.1)
project(MineSweeper LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Путь к SFMLConfig.txt
set(SFML_DIR "F:\\cpp\\Libs\\SFML-2.5.1-windows-vc15-64-bit\\SFML-2.5.1\\lib\\cmake\\SFML")
find_package(SFML COMPONENTS graphics window system REQUIRED)

add_library(field Field.h Field.cpp)
add_library(getRandom getRandom.cpp)
add_library(ms MineSweeper.h MineSweeper.cpp MineSweeper.jpg)
target_link_libraries(ms PRIVATE field getRandom)

add_executable(MineSweeper main.cpp)
target_link_libraries(MineSweeper PRIVATE ms sfml-graphics ${SFML_LIBRARIES})