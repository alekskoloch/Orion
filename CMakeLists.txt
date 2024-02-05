cmake_minimum_required(VERSION 3.10)

project(Orion)
add_definitions(-DASSETS_PATH=\"${CMAKE_SOURCE_DIR}/assets/\")
add_definitions(-DCONFIG_PATH=\"${CMAKE_SOURCE_DIR}/config/\")

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

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

# Main Executable
add_executable(main ${SOURCES})

# PCH setup
target_precompile_headers(main PRIVATE src/pch.h)

#SFML
target_link_libraries(main sfml-graphics sfml-audio)

#EnTT
target_link_libraries(main EnTT::EnTT)

# Include directories for nlohmann/json
include_directories(${NLOHMANN_JSON_INCLUDE_DIR})

#GoogleTest
enable_testing()

add_subdirectory(src/lib/Managers)
target_link_libraries(main EventManager FontManager SceneManager SoundManager TextureManager)

file(GLOB TEST_SRC tests/*.cpp)
add_executable(unit_tests ${TEST_SRC})
target_link_libraries(unit_tests gtest_main texture_manager sfml-graphics)

target_precompile_headers(unit_tests PRIVATE src/pch.h)

#GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(unit_tests)