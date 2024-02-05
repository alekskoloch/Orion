cmake_minimum_required(VERSION 3.10)

project(FontManager)

set(CMAKE_CXX_STANDARD 23)

find_package(SFML 2.6 COMPONENTS graphics REQUIRED)

set(SOURCE_FILES FontManager.cpp)
set(HEADER_FILES FontManager.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} sfml-graphics)