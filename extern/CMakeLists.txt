set(IMGUI_SRC
  imgui/imgui.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_draw.cpp
  imgui/backends/imgui_impl_glfw.cpp
  imgui/backends/imgui_impl_opengl3.cpp
)

add_library(ImGui STATIC ${IMGUI_SRC})
target_include_directories(ImGui PUBLIC imgui)
target_link_libraries(ImGui PUBLIC glfw)