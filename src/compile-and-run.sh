#!/usr/bin/env bash
set -e

# Remove build directory if it exists
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build
fi

# Recreate the build directory and run CMake
echo "Creating new build directory..."
mkdir build
cd build

echo "Running CMake..."
cmake ..

echo "Building project..."
make

echo "Executing application..."
./dippo-qt
