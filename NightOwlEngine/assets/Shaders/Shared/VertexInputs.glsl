layout(location = 0) in vec3 materialPosition;
layout(location = 1) in vec3 materialColor;
layout(location = 2) in vec2 materialUvs;
layout(location = 3) in vec3 materialNormals;
layout(location = 4) in vec3 materialTangents;
layout(location = 5) in vec3 materialBitangents;
layout(location = 6) in ivec4 boneIds;
layout(location = 7) in vec4 boneWeights;

layout (location = 100) uniform mat4 modelMatrix; // Assign a unique location to modelMatrix
layout (location = 101) uniform mat4 viewProjectionMatrix; // Assign a unique location to viewProjectionMatrix

layout (location = 102) uniform bool hasBones;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
layout (location = 103) uniform mat4 finalBonesMatrices[MAX_BONES]; // Assign a unique location to finalBonesMatrices

out vertexData
{
    vec3 materialPosition;
    vec3 materialColor;
    vec2 materialUvs;
    vec3 materialNormals;
    vec3 materialTangents;
    vec3 materialBitangents;
    flat ivec4 materialBoneIds;
    vec4 materialBoneWeights;
} outVertexData;