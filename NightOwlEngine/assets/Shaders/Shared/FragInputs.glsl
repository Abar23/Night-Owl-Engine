out vec4 FragColor;

layout (location = 900) uniform vec4 diffuseColor;
layout (location = 910) uniform vec4 specularColor;
layout (location = 920) uniform vec4 emissiveColor;
layout (location = 930) uniform vec4 transparentColor;
layout (location = 940) uniform vec4 reflectiveColor;
layout (location = 950) uniform vec4 ambientColor;
 
layout (location = 960) uniform sampler2D diffuseTexture;
layout (location = 970) uniform sampler2D specularTexture;
layout (location = 980) uniform sampler2D ambientOcclusionsTexture;
layout (location = 990) uniform sampler2D normalsTexture;
layout (location = 1000) uniform sampler2D roughnessTexture;

layout (location = 1010) uniform float shininess;
layout (location = 1020) uniform float shininessStrength;

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

struct Light
{
    vec3 position;
    vec3 color;
};

layout (binding = 0, std430) buffer lightStorageBlock
{
    Light lights[];
};
