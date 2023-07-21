cmake_minimum_required(VERSION 3.10)

project(Orion)

set(CMAKE_CXX_STANDARD 17)

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

add_executable(main src/main.cpp)

#SFML
target_link_libraries(main sfml-graphics sfml-audio)

#EnTT
target_link_libraries(main EnTT::EnTT)

#GoogleTest
enable_testing()
add_executable(tests tests/tests.cpp)
target_link_libraries(tests gtest_main)