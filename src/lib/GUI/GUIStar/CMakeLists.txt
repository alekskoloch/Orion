cmake_minimum_required(VERSION 3.10)

project(GUIStar)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES GUIStar.cpp)
set(HEADER_FILES GUIStar.h) 

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SFML_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    sfml-graphics
)