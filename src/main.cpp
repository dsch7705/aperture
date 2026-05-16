#include <cmath>
#include <print>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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

int windowW = 800;
int windowH = 600;
void window_size_callback(GLFWwindow* window, int w, int h)
{
  windowW = w;
  windowH = h;

  glViewport(0, 0, w, h);
  GLint viewport_loc = glGetUniformLocation(shader_program, "u_viewport");
  glUniform2f(viewport_loc, w, h);

  GLint position_loc = glGetUniformLocation(shader_program, "u_position");
  glUniform2f(position_loc, w / 2.f, h / 2.f);
}

int main(void)
{
  GLFWwindow* window;

  if (!glfwInit()) {
    return -1;
  }

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

  NgonFractal fractal(5.0f, 21, 2, 30.f);
  fractal.shader_program = shader_program;

  // ImGui
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
  ImGuiIO& io = ImGui::GetIO();

  while (!glfwWindowShouldClose(window)) {
    //glClearColor(0.f, 1.f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    fractal.draw(windowW / 2.f, windowH / 2.f);

    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Test");

    ImGui::DragFloat("vertices", &fractal.current_vertices, .1f, 3.f, fractal.max_vertices);

    unsigned int depth_min = 1;
    unsigned int depth_max = 6;
    ImGui::DragScalar("depth", ImGuiDataType_U32, &fractal.depth, 1.f, &depth_min, &depth_max);

    ImGui::DragFloat("radius", &fractal.radius, 1.f, 10.f, 20000.f);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  glfwTerminate();
  return 0;
}