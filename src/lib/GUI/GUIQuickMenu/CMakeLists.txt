cmake_minimum_required(VERSION 3.10)

project(GUIQuickMenu)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES GUIQuickMenu.cpp)
set(HEADER_FILES GUIQuickMenu.h) 

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SFML_INCLUDE_DIR}    
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    sfml-graphics
    EnTT::EnTT
    TextureManager
    SoundManager
)