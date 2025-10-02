cmake_minimum_required(VERSION 3.22)
project(MyProject VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_NAME_NOIPC GausssianBlur)
set(EXECUTABLE_NAME_IPC GausssianBlurIPC)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(GAUSSIAN_BLUR_LIBRARY LibGaussianBlur)
set(IMAGE_IO_LIBRARY ImageIo)
set(IPC_MANAGER_LIBRARY IpcShmManager)
set(TILE_MANAGER_LIBRARY LibTileManager)

# Options
option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_LTO "Enable link-time optimization" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)

# Include git submodules for non-CMake projects
include(AddGitSubmodule)
add_git_submodule(external/stb)

# Include CPM for fetching dependencies
include(CPM)

# External libraries
add_subdirectory(external)

# Source libraries
add_subdirectory(src)

# App
add_subdirectory(app)
