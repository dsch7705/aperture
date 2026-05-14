#version 460 core

layout(location = 0) in vec2 a_pos;

uniform vec2 u_viewport;
float aspect = u_viewport.x / u_viewport.y;

uniform vec2 u_position;
uniform float u_radius;

void main()
{
  vec2 pixel_pos = a_pos * u_radius + u_position;
  vec2 ndc = (pixel_pos / u_viewport) * 2.0 - 1.0;

  gl_Position = vec4(ndc, 0.0, 1.0);
}