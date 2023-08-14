cmake_minimum_required(VERSION 3.10)

project(Orion)
add_definitions(-DASSETS_PATH=\"${CMAKE_SOURCE_DIR}/assets/\")

set(CMAKE_CXX_STANDARD 23)

#SFML
find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

#EnTT
include(FetchContent)
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt
    GIT_TAG v3.12.2
)
FetchContent_MakeAvailable(EnTT)

#GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

# Main Executable
add_executable(main ${SOURCES})

# PCH setup
target_precompile_headers(main PRIVATE src/pch.h)

#SFML
target_link_libraries(main sfml-graphics sfml-audio)

#EnTT
target_link_libraries(main EnTT::EnTT)

#GoogleTest
enable_testing()

# create library for TextureManager
add_library(texture_manager src/managers/TextureManager.cpp)
target_include_directories(texture_manager PUBLIC src/managers)
target_link_libraries(texture_manager sfml-graphics EnTT::EnTT)

file(GLOB TEST_SRC tests/*.cpp)
add_executable(tests ${TEST_SRC})
target_link_libraries(tests gtest_main texture_manager sfml-graphics)

target_precompile_headers(tests PRIVATE src/pch.h)

#GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(tests)