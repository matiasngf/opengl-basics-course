#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// read shader source and split into vertex and fragment
struct ShaderProgamSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgamSource ParseShader(const std::string &filepath)
{
  // load file
  std::ifstream stream(filepath);

  enum class ShaderType
  {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };
  ShaderType type = ShaderType::NONE;

  // create two stringtreams to store vertex and fragment shader
  std::stringstream ss[2];

  // read file line by line
  std::string line;
  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      // line is shader type declaration
      if (line.find("vertex") != std::string::npos)
      {
        // set mode to vertex
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos)
      {
        // set mode to vertex
        type = ShaderType::FRAGMENT;
      }
    }
    else
    {
      // line is normal shader code
      ss[(int)type] << line << '\n';
    }
  }

  return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);

  // check for errors
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to complile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader."
              << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(void)
{
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  // Configure GLFW for macOS
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create window
  GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Make window's context current
  glfwMakeContextCurrent(window);

  // match refresh rate with screen fps
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
  {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Después de inicializar GLEW
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  // data for vertex buffer
  float positions[] = {
      -0.5f, -0.5f, // 0
      0.5f, -0.5f,  // 1
      0.5f, 0.5f,   // 2
      -0.5f, 0.5f   // 3
  };

  unsigned int indices[] = {
      0, 1, 2, // first triangle
      2, 3, 0  // second triangle
  };

  // vertex array object
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // send vertex data to the GPU
  unsigned int buffer;

  int verticesCount = 4;
  int vertexBufferSize = verticesCount * sizeof(float) * 2; // * 2 because x,y

  // Creates and binds our vertex buffer
  VertexBuffer vb(positions, vertexBufferSize);

  glEnableVertexAttribArray(0);
  // this function binds the buffer with the VAO
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  // create index buffer

  IndexBuffer ib(indices, sizeof(indices));

  unsigned int ibo;

  // create shaders
  ShaderProgamSource source = ParseShader("src/res/shaders/Basic.shader");
  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

  // Where in memory
  int uColorLocation = glGetUniformLocation(shader, "u_Color");
  ASSERT(uColorLocation != -1);

  float red = 0.0f;
  float increment = 0.01f;

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    // enable the shader
    glUseProgram(shader);
    // udpate uniform
    glUniform4f(uColorLocation, red, 1.f, 0.f, 1.f);

    // bind current index buffer
    ib.Bind();
    // draw elements combines index buffer + vertex buffer
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (red >= 1.f)
    {
      increment = -0.01f;
    }
    else if (red <= 0.f)
    {
      increment = 0.01f;
    }

    red += increment;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwTerminate();
  return 0;
}
