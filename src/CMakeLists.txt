find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(APERTURE_SRC
  main.cpp
  ngon.cpp
  glad.c
)

add_executable(aperture ${APERTURE_SRC})
target_link_libraries(aperture PRIVATE 
  OpenGL::OpenGL 
  glfw
)
target_include_directories(aperture PRIVATE ../include)
target_compile_options(aperture PRIVATE -freflection)