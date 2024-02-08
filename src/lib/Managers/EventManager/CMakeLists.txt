cmake_minimum_required(VERSION 3.10)

project(EventManager)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES EventManager.cpp)
set(HEADER_FILES EventManager.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PCH_DIR})

target_precompile_headers(${PROJECT_NAME} PRIVATE "${PCH_DIR}/pch.h")