cmake_minimum_required(VERSION 3.10)

project(SystemManager)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES SystemManager.cpp)
set(HEADER_FILES SystemManager.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PCH_DIR})

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    EnTT::EnTT
    ConfigManager
    PlayerInitializationSystem
    SceneManager
    ParticleSystem
    QuestSystem
    EnemyGroupSystem
    BackgroundManager
    MusicSystem
    InputSystem
    CollisionSystem
    EnergySystem
    RotateTowardsMouseSystem
    WaypointsMovementSystem
    EntityStateSystem
    ShieldSystem
    ShootingSystem
    BulletSystem
    AccelerationSystem
    MovementSystem
    WeaponSystem
    HealthSystem
    CameraSystem
    DropSystem
    RemovalSystem
    InfoSystem
    PointSystem
    NotifySystem
    QuestSystem
    TimeControlSystem
    RenderSystem
    DebugSystem
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "${PCH_DIR}/pch.h")