cmake_minimum_required(VERSION 3.10)

project(SkillManager)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES SkillManager.cpp)
set(HEADER_FILES SkillManager.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PCH_DIR})

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    EnTT::EnTT
    nlohmann_json::nlohmann_json
    GUIDialogBox
    GUIStar
    GUIBox
    GUISkill
    StoneSystem
    ExperienceSystem
    SkillSystem
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "${PCH_DIR}/pch.h")