#version 460 core

layout (location = 0) in vec3 materialPosition;
layout (location = 1) in vec3 materialColor;
layout (location = 2) in vec2 materialUvs;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vertexData
{
    vec3 materialPositon;
    vec3 materialColor;
    vec2 materialUvs;
} outVertexData;

void main(void)
{
    outVertexData.materialPositon = materialPosition;
    outVertexData.materialColor = materialColor;
    outVertexData.materialUvs = materialUvs;

    gl_Position = viewProjectionMatrix * modelMatrix * vec4(materialPosition, 1.0);
} 