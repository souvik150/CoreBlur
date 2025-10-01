#!/bin/bash
set -e  # Exit immediately if a command fails

rm -rf build
mkdir build

cd build

# Run CMake
cmake ..

# Build project
cmake --build .
