#version 460 core

layout(location = 0) in vec2 aPos;

void main()
{
  gl_Position = vec4(aPos.x / 2.0, aPos.y / 2.0, 0.0, 1.0);
}