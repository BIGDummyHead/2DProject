cmake_minimum_required(VERSION 3.29)
project(2DProject)

set(CMAKE_CXX_STANDARD 20)

# SDL2 Root Path
set(SDL2_PATH "SDL")
set(SDL2_image_PATH "SDL_image")
set(SDL2_ttf_PATH "SDL_ttf")

# Include and lib directories
include_directories(${SDL2_PATH}/include)
include_directories(${SDL2_PATH}/include/SDL2)

link_directories(${SDL2_PATH}/lib)

include_directories(${SDL2_image_PATH}/include)
include_directories(${SDL2_image_PATH}/include/SDL3_image)

link_directories(${SDL2_image_PATH}/lib)

include_directories(${SDL2_ttf_PATH}/include)
link_directories(${SDL2_ttf_PATH}/lib/x64)



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
        Game/Physics/Ray.h
        Game/LightSource.h
        Game/LightSource.cpp
        Game/Component.cpp
        Game/Component.h
        Game/UiObject.cpp
        Game/UiObject.h
        Game/Physics/Castable.h
        TextFont.h
        UiObjectFont.cpp
        UiObjectFont.h
        TextFont.cpp
        IP_Games/Knight/Knight_Game.cpp
        IP_Games/Knight/Knight_Game.h
        Settings/Parser.cpp
        Settings/Parser.h
        IP_Games/Knight/Player.cpp
        IP_Games/Knight/Player.h
        IP_Games/Knight/PlayerStats.h
        Game/SheetAnimator.cpp
        Game/SheetAnimator.h
        Game/AnimationCycle.h
        Game/AnimationCycle.cpp
        Game/CycleManager.cpp
        Game/CycleManager.h
        Audio/AudioManager.cpp
        Audio/AudioManager.h
        Audio/Device.h
        Audio/Sound.cpp
        Audio/Sound.h
        Audio/WavFileHeader.h
        Audio/TimeSound.h
)

# Link against SDL2 libraries
target_link_libraries(2DProject SDL2 SDL2main SDL2_image SDL2_ttf winmm)

# Enable debugging symbols
set(CMAKE_BUILD_TYPE Debug)