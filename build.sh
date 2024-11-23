#!/bin/bash

# Function to build
build() {
    # Create build directory if it doesn't exist
    mkdir -p "${PWD}/build"
    
    # Copy the GLFW dynamic library to the build directory
    cp "${PWD}/dependencies/glfw-3.4.bin.MACOS/lib-arm64/libglfw.3.dylib" "${PWD}/build/"
    
    # Compile with rpath set to the executable's directory
    g++ -std=c++17 -g \
        "${PWD}/src/main.cpp" \
        -o "${PWD}/build/program" \
        -framework OpenGL \
        -I"${PWD}/dependencies/glfw-3.4.bin.MACOS/include" \
        -L"${PWD}/dependencies/glfw-3.4.bin.MACOS/lib-arm64" \
        -lglfw.3 \
        -Wl,-rpath,@executable_path
}

# Function to watch
watch() {
    clear
    echo "Building..."
    build
    echo "Watching for changes in src directory..."
    fswatch -o "${PWD}/src" | while read; do
        clear
        echo "Building..."
        build
    done
}

# Handle commands
case "$1" in
    "watch")
        watch
        ;;
    *)
        build
        ;;
esac