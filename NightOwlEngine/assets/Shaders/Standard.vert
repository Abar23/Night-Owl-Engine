#version 460 core

layout (location = 0) in vec3 materialPosition;
layout (location = 1) in vec3 materialColor;
layout (location = 2) in vec2 materialUvs;
layout (location = 3) in vec3 materialNormals;
layout (location = 4) in vec3 materialTangents;
layout (location = 5) in vec3 materialBitangents;
layout (location = 6) in ivec4 boneIds;
layout (location = 7) in vec4 boneWeights;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vertexData
{
    vec3 materialPositon;
    vec3 materialColor;
    vec2 materialUvs;
    vec3 materialNormals;
    vec3 materialTangents;
    vec3 materialBitangents;
    flat ivec4 materialBoneIds;
    vec4 materialBoneWeights;
} outVertexData;

void main(void)
{
    outVertexData.materialPositon = materialPosition;
    outVertexData.materialColor = materialColor;
    outVertexData.materialUvs = materialUvs;
    outVertexData.materialNormals = materialNormals;
    outVertexData.materialTangents = materialTangents;
    outVertexData.materialBitangents = materialBitangents;
    outVertexData.materialBoneIds = boneIds;
    outVertexData.materialBoneWeights = boneWeights;

    gl_Position = viewProjectionMatrix * modelMatrix * vec4(materialPosition, 1.0);
} 