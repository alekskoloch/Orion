cmake_minimum_required(VERSION 3.10)

project(Orion)

set(CMAKE_CXX_STANDARD 17)

#SFML
find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

add_executable(main main.cpp)

#SFML
target_link_libraries(main sfml-graphics sfml-audio)