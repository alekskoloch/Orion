cmake_minimum_required(VERSION 3.10)

project(TutorialSystem)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES TutorialSystem.cpp)
set(HEADER_FILES TutorialSystem.hpp) 

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SFML_INCLUDE_DIR}    
    ${PCH_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    sfml-graphics
    EnTT::EnTT
    EventManager
    NotifySystem
    ConfigManager
    QuestSystem
    EnemyInitializationSystem
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "${PCH_DIR}/pch.h")