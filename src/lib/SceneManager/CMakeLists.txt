cmake_minimum_required(VERSION 3.10)

project(SceneManager)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES SceneManager.cpp)
set(HEADER_FILES SceneManager.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})