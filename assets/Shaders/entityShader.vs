#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform mat4 model;

out vec4 outCol;

void main(void)
{
    gl_Position = model * vec4(aPos, 1.0);
    outCol = vec4(aCol, 1.0);
} 