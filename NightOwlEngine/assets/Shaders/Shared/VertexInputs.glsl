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

uniform bool hasBones;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

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