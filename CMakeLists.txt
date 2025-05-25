cmake_minimum_required(VERSION 3.29)
project(2DProject)

set(CMAKE_CXX_STANDARD 20)

# SDL2 Root Path
set(SDL2_PATH "SDL")
set(SDL2_image_PATH "SDL_image")

# Include and lib directories
include_directories(${SDL2_PATH}/include)
include_directories(${SDL2_PATH}/include/SDL2)

link_directories(${SDL2_PATH}/lib)

include_directories(${SDL2_image_PATH}/include)
include_directories(${SDL2_image_PATH}/include/SDL3_image)

link_directories(${SDL2_image_PATH}/lib)

# Add your executable
add_executable(2DProject main.cpp
        defs.h
        structs.h
        init.cpp
        init.h
        input.cpp
        input.h
        draw.cpp
        draw.h
        Vector2.cpp
        Vector2.h
        Game/GObject.cpp
        Game/GObject.h
        Game/Transform.cpp
        Game/Transform.h
        Game/Texture.cpp
        Game/Texture.h
        Game/Sheet.cpp
        Game/Sheet.h
        Game/Background.cpp
        Game/Background.h
        Game/Collider.cpp
        Game/Collider.h
        Game/Camera.cpp
        Game/Camera.h
        Game/Scene.cpp
        Game/Scene.h
        Game/Physics/Raycaster.cpp
        Game/Physics/Raycaster.h
        Game/Physics/RayInfo.h
)

# Link against SDL2 libraries
target_link_libraries(2DProject SDL2 SDL2main)

target_link_libraries(2DProject SDL2_image SDL2_image)

# Enable debugging symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
