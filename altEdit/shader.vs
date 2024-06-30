#version 410

in vec4 a_position;
uniform vec2 iResolution;
uniform vec2 iMouse;
uniform float iTime;

void main()
{
  gl_Position = a_position;
}