layout (location = 300) uniform vec4 diffuseColor;
layout (location = 301) uniform vec4 specularColor;
layout (location = 302) uniform vec4 emissiveColor;
layout (location = 303) uniform vec4 transparentColor;
layout (location = 304) uniform vec4 reflectiveColor;
layout (location = 305) uniform vec4 ambientColor;

layout (location = 310) uniform float roughness;
layout (location = 311) uniform float metallic;
layout (location = 312) uniform float ambientOcclusion; // just temporary for BRDF/PBR
 
layout (location = 320) uniform sampler2D diffuseTexture;
layout (location = 321) uniform sampler2D specularTexture;
layout (location = 322) uniform sampler2D ambientOcclusionsTexture;
layout (location = 323) uniform sampler2D normalsTexture;
layout (location = 324) uniform sampler2D roughnessTexture; // Also known as specular map

layout (location = 330) uniform float shininess;
layout (location = 331) uniform float shininessStrength;

out vec4 fragColor;

in vertexData
{
    vec3 materialPosition;
    vec3 materialColor;
    vec2 materialUvs;
    vec3 materialNormals;
    vec3 materialTangents;
    vec3 materialBitangents;
    flat ivec4 materialBoneIds;
    vec4 materialBoneWeights;
} inVertexData;
