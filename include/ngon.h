#pragma once

#include <vector>

#include "glad/glad.h"

struct Vertex {
  float x;
  float y;
};

class NgonFractal {
public:
  float current_vertices;
  unsigned int max_vertices;
  unsigned int depth;
  float radius;

  GLuint shader_program;

  NgonFractal(float current_vertices, unsigned int max_vertices, unsigned int depth, float radius);
  ~NgonFractal();

  void update_vertices();
  void draw(float x, float y);
  
  private:
  GLuint m_vao;
  GLuint m_vbo;
  GLuint m_ebo;
  
  std::vector<Vertex> m_vertices;
  
  void draw_ngon(float x, float y);
  void fractal(float x, float y, unsigned int d);
};