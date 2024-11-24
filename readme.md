# OpenGL engine

This repository contains experiments made while following the course [OpenGL - The Cherno](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&ab_channel=TheCherno) on YouTube.


### Installation

1. Install the required libraries using Homebrew:
```bash
brew install glfw glew
```

## Build

- On VSCode or Cursor, press `Cmd+Shift+B` to build the project or run `./build.sh`.
- To build on watch mode, run `./build.sh watch`.

## Run

- Use `./build/program` or press `Ctrl+F5` to run the program.
- Press `F5` to to run in debug mode.

## Notes

gl docs: https://docs.gl/

### Creating vertex buffers

```cpp
// Vertex data
float positions[6] = {
      -0.5f, -0.5f,
      0.0f, 0.5f,
      0.5f, -0.5f};

unsigned int buffer;
glGenBuffers(1, &buffer);
// Tells opengl that I want to perform operations on the buffer
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

// on the render loop
// this will draw the selected buffer
glDrawArrays(GL_TRIANGLES, 0, 3);
```

https://docs.gl/gl4/glBufferData

Gl_STATIC_DRAW: CPU writes once, GPU reads frequently. (3D model loaded).

GL_DYNAMIC_READ: GPU writes into buffer, GPU reads frequently. (Simulation, physics).

### glVertexAttribPointer

It's used to tell OpenGL how to interpret the vertex data.
This way we can have multiple attributes like position, UV, normals, etc.
  
  ```cpp

float positions[6] = {
  -0.5f, -0.5f, // Vertex 1: (X, Y)
  0.0f, 0.5f, // Vertex 2: (X, Y)
  0.5f, -0.5f // Vertex 3: (X, Y)
};

// Enable the first vertex attribute
glEnableVertexAttribArray(0);

// Tell OpenGL how to interpret the vertex data
glVertexAttribPointer(
  0, // Index of the attribue (0 usually is vertex position)
  2, // Number of components (2 for x and y)
  GL_FLOAT, // Type of the data
  GL_FALSE, // Normalize the data
  sizeof(float) * 2, // Stride (size of the vertex)
  0 // Offset
);
```