cmake_minimum_required(VERSION 3.0)
project(Gears C)

set(CMAKE_C_STANDARD 99)

# fetching Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

# source files
set(PROJECT_SOURCES
        main.c
        constructions/ConstructCircle.c
        constructions/ConstructBisector.c
        constructions/ConstructEquilateral.c
        constructions/ConstructEqualLine.c
        constructions/ConstructAngleBisector.c
        constructions/ConstructEqualLengthCut.c
        constructions/ConstructEqualAngle.c
        constructions/ConstructParallel.c
        constructions/ConstructPerpendicular.c
        FadeConstruction.c
        gear.c
        gear.h
        input.c
        input.h
)

# executables
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# linking Raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# including directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (EMSCRIPTEN)

    set_target_properties(Gears PROPERTIES SUFFIX ".html")

    target_link_options(Gears PRIVATE
            "-sUSE_GLFW=3"
            "-sASYNCIFY=1"
            "-sSTACK_SIZE=1048576"
            "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/resources@../resources"
    )
endif()
