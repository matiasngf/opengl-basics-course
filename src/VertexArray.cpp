#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
  // create vertex array object
  GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
  GLCall(glDeleteVertexArrays(1, &m_RendererID))
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
  // 1: Bind the VAO
  Bind();
  // 2: Bind the buffer that we want to deal with
  vb.Bind();
  // 3: Setup layout for it
  // This buffer can have multiple layouts, we need to loop over each one of them
  const auto &elements = layout.GetElements();

  size_t offset = 0;

  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto &element = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    // this function binds the buffer with the VAO
    GLCall(glVertexAttribPointer(
        i,             // Index of the attribue
        element.count, // Number of components (eg: 2 for x and y)
        element.type,  // Type of the data
        element.normalized,
        layout.GetStride(),                       // Stride (total size of each vertex)
        reinterpret_cast<const void *>(offset))); // Offset (where the position data starts on each vertex)

    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}

void VertexArray::Bind() const
{
  GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
  GLCall(glBindVertexArray(0));
}
