#pragma once

#include "glad/glad.h"

struct Vertex {
  float x;
  float y;
};

class NgonFractal {
public:
  unsigned int max_vertices;
  int depth;
  float radius;

  NgonFractal(unsigned int max_vertices, int depth, float radius);
  ~NgonFractal();

private:
  GLuint m_vao;
  GLuint m_vbo;
  GLuint m_ebo;
};

inline void ngon_fractal(float x, float y, unsigned int sides, int depth)
{
  if (depth == 0) {
    return;
  }


}