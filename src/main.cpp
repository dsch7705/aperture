#include <cmath>
#include <print>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "ngon.h"

void gl_debug_callback(GLenum source, 
  GLenum type, 
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam)
{
  if (type == GL_DEBUG_TYPE_OTHER) {
    return;
  }
  std::println("OpenGL Message | {}", message);
}

GLuint shader_program;
void window_size_callback(GLFWwindow* window, int w, int h)
{
  glViewport(0, 0, w, h);
  GLint viewport_loc = glGetUniformLocation(shader_program, "u_viewport");
  glUniform2f(viewport_loc, w, h);

  GLint position_loc = glGetUniformLocation(shader_program, "u_position");
  glUniform2f(position_loc, w / 2.f, h / 2.f);
}

std::vector<float> gen_ngon_verts(size_t n)
{
  if (n < 3) {
    throw "n must be no less than 3";
  }

  constexpr float two_pi = 2 * std::numbers::pi;

  std::vector<float> verts;
  for (size_t i = 0; i < n; ++i) {
    float theta = i * (two_pi / n);
    verts.push_back(std::cos(theta));
    verts.push_back(std::sin(theta));
  }

  return verts;
}

std::vector<GLuint> gen_ngon_inds(size_t n)
{
  if (n < 3) {
    throw "n must be no less than 3";
  }

  std::vector<GLuint> inds;
  for (size_t i = 0; i < n; ++i) {
    inds.push_back(i);
  }

  return inds;
}

void draw_fractal(GLuint vao, size_t n)
{
  glBindVertexArray(vao);

}

int main(void)
{
  GLFWwindow* window;

  if (!glfwInit()) {
    return -1;
  }

  int windowW = 800;
  int windowH = 600;
  window = glfwCreateWindow(windowW, windowH, "Aperture", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // GLFW callbacks
  glfwSetWindowSizeCallback(window, window_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::println("Failed to load GL functions");
    return -1 ;
  }
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_debug_callback, nullptr);

  std::println("OpenGL Version {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

  // Generate n-gons' vertices and indices
  constexpr size_t num_ngons = 5;
  std::vector<std::vector<float>> ngons_vertices;
  std::vector<std::vector<GLuint>> ngons_indices;
  for (int i = 3; i < num_ngons + 3; ++i) {
    ngons_vertices.push_back(gen_ngon_verts(i));
    ngons_indices.push_back(gen_ngon_inds(i));
  }

  // Generate GL objects
  std::vector<GLuint> ngon_vaos;
  for (size_t i = 0; i < num_ngons; ++i) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    ngon_vaos.push_back(vao);

    GLuint vbo;
    const auto& verts = ngons_vertices[i];
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    GLuint ebo;
    const auto& inds = ngons_indices[i];
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(GLuint), inds.data(), GL_STATIC_DRAW);
  }

  // Shaders
  auto check_program = [](GLuint program) {
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
      GLchar log[512];
      glGetProgramInfoLog(program, sizeof(log), nullptr, log);
      std::println("Shader linkage failed:\n{}", log);
      return false;
    }
    return true;
  };

  const GLchar vert_shader_src[] = {
    #embed "shaders/vert.glsl"
    , 0
  };
  const GLchar* vert_shader_str = vert_shader_src;
  GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vert_shader_str, nullptr);
  glCompileShader(vert_shader);//check_shader(vert_shader);

  const GLchar frag_shader_src[] = {
    #embed "shaders/frag.glsl"
    , 0
  };
  const GLchar* frag_shader_str = frag_shader_src;
  GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &frag_shader_str, nullptr);
  glCompileShader(frag_shader);//check_shader(frag_shader);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vert_shader);
  glAttachShader(shader_program, frag_shader);
  check_program(shader_program);
  glUseProgram(shader_program);
  window_size_callback(window, windowW, windowH);

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  // Shader uniforms
  GLint radius_loc = glGetUniformLocation(shader_program, "u_radius");
  glUniform1f(radius_loc, 30.f);

  while (!glfwWindowShouldClose(window)) {
    //glClearColor(0.f, 1.f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);


    auto draw_ngon = [&](size_t n) {
      glBindVertexArray(ngon_vaos[n - 3]);
      glDrawElements(GL_LINE_LOOP, ngons_indices[n - 3].size(), GL_UNSIGNED_INT, nullptr);
    };

    draw_ngon(5);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}