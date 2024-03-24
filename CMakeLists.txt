cmake_minimum_required(VERSION 3.10)

project(Orion)
add_definitions(-DASSETS_PATH=\"${CMAKE_SOURCE_DIR}/assets/\")
add_definitions(-DCONFIG_PATH=\"${CMAKE_SOURCE_DIR}/config/\")

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld" CACHE INTERNAL "")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld" CACHE INTERNAL "")

#SFML
find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

#Boost
cmake_policy(SET CMP0074 NEW)
find_package(Boost 1.84.0 REQUIRED)

#EnTT
include(FetchContent)
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt
    GIT_TAG v3.12.2
)
FetchContent_MakeAvailable(EnTT)

#nlohmann_json
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.10.0
)
FetchContent_MakeAvailable(nlohmann_json)

set(NLOHMANN_JSON_INCLUDE_DIR "${nlohmann_json_SOURCE_DIR}/single_include")

#GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

#GoogleTest
enable_testing()

# Main Executable
add_executable(main src/main.cpp src/Game.h src/Game.cpp)

target_precompile_headers(main PRIVATE src/pch.h)
set(PCH_DIR "${PROJECT_SOURCE_DIR}/src" CACHE INTERNAL "")

#SFML
target_link_libraries(main sfml-graphics sfml-audio)
include_directories(${SFML_INCLUDE_DIR})

#Boost
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} src/main.cpp)
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

#EnTT
target_link_libraries(main EnTT::EnTT)

# Include directories for nlohmann/json
include_directories(${NLOHMANN_JSON_INCLUDE_DIR})

add_subdirectory(src/lib/Utils)
include_directories(${UTILS_INCLUDE_DIR})

add_subdirectory(src/lib/Components)
include_directories(${COMPONENTS_INCLUDE_DIR})

add_subdirectory(src/lib/Managers)
target_link_libraries(main SystemManager GUIManager EventManager FontManager SceneManager SoundManager TextureManager BackgroundManager SkillManager ConfigManager)

add_subdirectory(src/lib/Schemes)
include_directories(${SCHEMES_INCLUDE_DIR})

add_subdirectory(src/lib/Systems)
target_link_libraries(main
    PointSystem
    QuestSystem
    CooldownSystem
    NotifySystem
    ProceduralGenerationSystem
    AccelerationSystem
    BulletSystem
    CameraSystem
    CollisionSystem
    DropSystem
    WeaponSystem
    PlayerInitializationSystem
    ShieldSystem
    EnergySystem
    DebugSystem
    EntityStateSystem
    HealthSystem
    InfoSystem
    InputSystem
    MovementSystem
    MusicSystem
    RemovalSystem
    RenderSystem
    RotateTowardsMouseSystem
    ShootingSystem
    TimeControlSystem
    WaypointsMovementSystem
    ParticleSystem
    EnemyGroupSystem
)

add_subdirectory(src/lib/Entities)
target_link_libraries(main Entities)

add_subdirectory(src/lib/GUI)
target_link_libraries(main
    GUIEnergyBar
    GUIQuickMenu
    GUIExpInfo
    GUIJournal
    GUIMinimap
    GUIWeaponTile
    GUIScenes
)

file(GLOB TEST_SRC tests/*.cpp)
add_executable(unit_tests ${TEST_SRC})
target_link_libraries(unit_tests gtest_main TextureManager
 sfml-graphics)

include(GoogleTest)
gtest_discover_tests(unit_tests)