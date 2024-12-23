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

# Notes

gl docs: https://docs.gl/

## Vertex buffers

They are used to store the vertex data.

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

## Vertex Attributes (glVertexAttribPointer)

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
// Specify the layout of our data for each vertex attribute
glVertexAttribPointer(
  0, // Index of the attribue (0 usually is vertex position)
  2, // Number of components (2 for x and y)
  GL_FLOAT, // Type of the data
  GL_FALSE, // Normalize the data
  sizeof(float) * 2, // Stride (size of the vertex)
  0 // Offset
);
```

### Stride

The stride is the size of the data for each vertex. If a vertex a 2 floats for position and 2 floats for UVs, the stride is 4 floats.

### Example with UVs

```cpp
float positions[8] = {
  -0.5f, -0.5f, 0.0f, 0.0f, // Vertex 1: (X, Y, U, V)
  0.5f, -0.5f, 1.0f, 0.0f, // Vertex 2: (X, Y, U, V)
  0.0f, 0.5f, 0.5f, 1.0f // Vertex 3: (X, Y, U, V)
};

// Enable the position vertex attribute
glEnableVertexAttribArray(0);

// Tell OpenGL how to interpret the position vertex data
glVertexAttribPointer(
  0, // Index of the attribue (0 usually is vertex position)
  2, // Number of components (2 for x and y)
  GL_FLOAT, // Type of the data
  GL_FALSE, // Normalize the data
  sizeof(float) * 4, // Stride (total size of each vertex)
  0 // Offset (where the position data starts on each vertex)
);

// Enable the UV vertex attribute
glEnableVertexAttribArray(1);

// Tell OpenGL how to interpret the UV vertex data
glVertexAttribPointer(
  1, // Index of the attribue
  2, // Number of components (2 for u and v)
  GL_FLOAT, // Type of the data
  GL_FALSE, // Normalize the data
  sizeof(float) * 4, // Stride (total size of each vertex)
  (const void*)(sizeof(float) * 2) // Offset (where the UV data starts on each vertex, in this case we need to skip the position data, wich is 2 floats)
);
```

## Index Buffers

They are used to store the index data. It will tell openGL what vertex to render next. It's also used to avoid redefining the same vertex multiple times. For example, a cube has 8 vertices, but it's made of 6 faces, so we can define the 8 vertices once and then use the index buffer to tell OpenGL which vertices to use for each face.

We could also use index buffers to render one face of the cube with a different material.

```cpp
unsigned int buffer;
glGenBuffers(1, &buffer);
// ...create buffer

// Create indices to draw a square
unsigned int indices[6] = {
  0, 1, 2,
  2, 3, 4
};

// Create a index buffer
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

## Compiling programs and shaders

```cpp
// Create a program
unsigned int programId = glCreateProgram();

// create source code for the shaders

std::string vertexShaderSource = R"glsl(
  #version 330 core

  layout(location = 0) in vec4 position;

  void main() {
    gl_Position = position;
  }
)glsl";

std::string fragmentShaderSource = R"glsl(
  #version 330 core

  layout(location = 0) out vec4 color;

  void main() {
    color = vec4(1.0, 0.0, 0.0, 1.0);
  }
)glsl";

// Creating a shader
unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER); // or GL_FRAGMENT_SHADER
const char *src = vertexShaderSource.c_str();
glShaderSource(vertexShaderId, 1, &src, NULL);
glCompileShader(vertexShaderId);

// ... same for fragment shader

// attach shaders to the program
glAttachShader(programId, vertexShaderId);
glAttachShader(programId, fragmentShaderId);

// link and validate the program
glLinkProgram(programId);
glValidateProgram(programId);

// delete the shaders
glDeleteShader(vs);
glDeleteShader(fs);

// use the program
glUseProgram(programId);

// ...frame loop

//cleanup after the app is closed
glDeleteProgram(programId);
```

## Index buffers usage

Vertex buffer can contain large amount of data, for example the geometry for a spaceship.

Lets say that we want to renden the body of the spaceship and the windows with different materails (programs). What we should do is bind the `vertex buffer` once.

To render the ship I would enable the first program, with the first `index buffer` to render the body, then I would enable the second program, with the second `index buffer` to render the windows.

So `index buffers` are a way to select the data we want to render next.

# Vertex Array Objects

VAO is a container for multiple buffers.

```cpp
// create a VAO
// vertex array object
unsigned int vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);


// ...bind the buffer
glBindBuffer(GL_ARRAY_BUFFER, buffer);
// and enable the attribute
glEnableVertexAttribArray(0);

// This buffer is not binded to the VAO, to do that, we need to link them using glVertexAttribPointer
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
```

Cleanup:

```cpp
glDeleteVertexArrays(1, &vao);
```
