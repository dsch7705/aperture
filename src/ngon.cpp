#include "ngon.h"

#include <vector>

NgonFractal::NgonFractal(unsigned int max_vertices, int depth, float radius)
  : max_vertices(max_vertices)
  , depth(depth)
  , radius(radius)
{
  glCreateVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glCreateBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, max_vertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
  glEnableVertexAttribArray(0);

  glCreateBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  std::vector<GLuint> inds;
  for (unsigned int i = 0; i < max_vertices; ++i) {
    inds.push_back(i);
  }
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_vertices, inds.data(), GL_STATIC_DRAW);
}

NgonFractal::~NgonFractal()
{
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}