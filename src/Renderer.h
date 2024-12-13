#pragma once

#include <GL/glew.h>

// Handle GL errors

#define ASSERT(x)                                   \
  if (!(x))                                         \
  {                                                 \
    std::cerr << "Assertion failed: " << #x << "\n" \
              << "File: " << __FILE__ << "\n"       \
              << "Line: " << __LINE__ << "\n";      \
    std::abort();                                   \
  }
#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x))

void GLClearError();

bool GLLogCall(const char *functionName);