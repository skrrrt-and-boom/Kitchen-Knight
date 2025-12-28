cmake_minimum_required(VERSION 3.15)
project(kitchen_knight C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find or fetch raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# Source files
set(SOURCES
    src/main.c
    src/game.c
    src/player.c
    src/enemy.c
    src/arena.c
    src/combat.c
    src/enemies/enemy_types.c
    src/map_loader.c
    src/particles.c
    src/audio.c
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Platform-specific settings
if(APPLE)
    # macOS frameworks
    target_link_libraries(${PROJECT_NAME}
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
elseif(WIN32)
    # Windows libraries
    target_link_libraries(${PROJECT_NAME} opengl32 gdi32 winmm)
    
    # Copy assets to build directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")
endif()

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
