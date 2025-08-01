# 2DProject: SDL 2D Scroller Engine

> **Made with passion by Shawn Bohn**

![C++](https://img.shields.io/badge/language-C++-blue.svg)
![SDL2](https://img.shields.io/badge/engine-SDL2-yellow.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)

---

## 🚀 Overview

Welcome to **2DProject** — a modular 2D game engine and side-scroller framework built from scratch in C++ using SDL2!  
This project is a playground for learning, experimenting, and pushing the boundaries of what’s possible with 2D engines.  
It’s all about fun, features, and flexibility.

---

## ✨ Features

- **Modular GameObjects**: Inspired by Unity, create objects with fully pluggable components.
- **Texture Rendering & Animation**: Powerful sprite sheet support with conditional animation cycles.
- **UI System**: Draw text, interact with UI objects, and handle callback events.
- **Input Detection**: Robust keyboard and mouse input system (Down, Held, Release) with event callbacks.
- **Camera System**: Move, follow, and scale your world.
- **Collision & Physics**: AABB collision, collider systems, and basic 2D physics for dynamic gameplay.
- **Lighting**: Both static and raycasted lighting for mood and depth!
- **Raycasting**: For both scene and UI interactions, with additional support for dynamic lighting.
- **Audio Engine**: WASAPI-based sound system; play effects and music.
- **Efficient Rendering**: Proper z-ordering, batching, and minimal overdrawing for high performance.
- **Extensible**: Easily add your own scenes, objects, and mechanics.
- **Debug-Friendly**: Built with debugging in mind with verbose logs.

---

## 🖼️ Screenshots

![Game Engine Example Image](https://github.com/BIGDummyHead/2DProject/blob/main/Game%20Engine%20Example.png "Game Engine Example Picture")

---

## 🛠️ Getting Started

### Prerequisites

- C++20 compatible compiler
- [SDL2](https://www.libsdl.org/), [SDL2_image](https://github.com/libsdl-org/SDL_image), [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
- CMake 3.29+
- Windows (native), but should be portable to other platforms with some tweaks

### Build Instructions

```bash
git clone https://github.com/BIGDummyHead/2DProject.git
cd 2DProject
cmake -B build
cmake --build build
```

- Make sure SDL2, SDL2_image, and SDL2_ttf libraries are available and configured in your environment.  
- Adjust `CMakeLists.txt` if your SDL libraries are in custom locations.

### Run

```bash
./build/2DProject
```

---

## 🕹️ Gameplay & Engine Concepts

- **Scenes**: Define and load scenes using the modular engine API.
- **UI**: Render text, interactive buttons, and menus.
- **Lighting**: Add dynamic and static lights to scenes.
- **Audio**: Play sound effects and background music with ease.
- **Extend**: Add your own minigames, levels, or mechanics by subclassing `GameObject`.

---

## ❤️ Contributing

Pull requests, issues, and feature requests are *highly* encouraged.  
Make this project even more awesome by suggesting features, reporting bugs, or submitting code.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---
