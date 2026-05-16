#include "ngon.h"

#include <cmath>
#include <vector>
#include <print>

NgonFractal::NgonFractal(float current_vertices, unsigned int max_vertices, unsigned int depth, float radius)
  : current_vertices(current_vertices)
  , max_vertices(max_vertices)
  , depth(depth)
  , radius(radius)
{
  m_vertices.resize(max_vertices);

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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_vertices * sizeof(GLuint), inds.data(), GL_STATIC_DRAW);
}

NgonFractal::~NgonFractal()
{
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void NgonFractal::update_vertices()
{
  if (current_vertices > max_vertices) {
    current_vertices = max_vertices;
  }

  static constexpr float two_pi = 2.f * std::numbers::pi;
  for (int i = 0; static_cast<float>(i) < current_vertices; ++i) {
    float angle = (two_pi / current_vertices) * i;
    float x = std::cos(angle);
    float y = std::sin(angle);
    m_vertices[i] = {x, y};
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLuint>(current_vertices) * sizeof(Vertex), m_vertices.data());
}

void NgonFractal::draw(float x, float y)
{
  static GLuint u_radius_loc = glGetUniformLocation(shader_program, "u_radius");
  glUniform1f(u_radius_loc, radius);

  update_vertices();
  glBindVertexArray(m_vao);
  fractal(x, y, depth);
}

void NgonFractal::draw_ngon(float x, float y)
{
  static GLuint u_position_loc = glGetUniformLocation(shader_program, "u_position");
  glUniform2f(u_position_loc, x, y);

  glDrawElements(GL_LINE_LOOP, static_cast<GLsizei>(current_vertices), GL_UNSIGNED_INT, nullptr);
}

void NgonFractal::fractal(float x, float y, unsigned int d) 
{
  if (d == 0) {
    return;
  }

  draw_ngon(x, y);
  for (int i = 0; i < static_cast<int>(current_vertices); ++i) {
    const Vertex& v = m_vertices[i];
    fractal(x + v.x * radius, y + v.y * radius, d - 1);
  }
}

