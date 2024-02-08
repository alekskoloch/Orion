cmake_minimum_required(VERSION 3.10)

project(DebugSystem)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES DebugSystem.cpp)
set(HEADER_FILES DebugSystem.h) 

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SFML_INCLUDE_DIR}
    ${PCH_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    sfml-graphics
    EnTT::EnTT
    BackgroundManager
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "${PCH_DIR}/pch.h")