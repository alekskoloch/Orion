cmake_minimum_required(VERSION 3.10)

project(GUIManager)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES GUIManager.cpp)
set(HEADER_FILES GUIManager.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PCH_DIR})

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    EnTT::EnTT
    GUIScenes
    GUIQuickMenu
    GUIEnergyBar
    GUIMinimap
    GUIWeaponTile
    GUIExpInfo
    GUIJournal
    SceneManager
    WeaponSystem
    ShieldSystem
    PlayerInitializationSystem
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "${PCH_DIR}/pch.h")