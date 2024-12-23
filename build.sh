#!/bin/bash

compile() {
    # Create build directory if it doesn't exist
    mkdir -p "${PWD}/build"

    # Find all cpp files
    CPP_FILES=$(find "${PWD}/src" -name "*.cpp")
    
    # Compile with system-installed libraries
    g++ -std=c++17 -g \
        ${CPP_FILES} \
        -o "${PWD}/build/program" \
        -framework OpenGL \
        -I/opt/homebrew/include \
        -L/opt/homebrew/lib \
        -lglfw \
        -lGLEW

}

# Function to build
build() {
    clear
    echo "Building..."
    compile
    echo "Built."
}

# Function to watch
watch() {
    clear
    echo "Building..."
    compile
    echo "Built."
    echo "Watching for changes in src directory..."
    fswatch -o "${PWD}/src" | while read; do
        echo "Building..."
        compile
        echo "Built."
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