#include "Renderer.h"
#include <iostream>
#include <GL/glew.h>

void GLClearError()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}

bool GLLogCall(const char *functionName)
{
  while (GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error] (" << error << ") \nFunction: " << functionName << std::endl;
    return false;
  }
  return true;
}