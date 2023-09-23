out vec4 FragColor;

uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec4 emissiveColor;
uniform vec4 transparentColor;
uniform vec4 reflectiveColor;
uniform vec4 ambientColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D ambientOcclusionsTexture;
uniform sampler2D normalsTexture;
uniform sampler2D roughnessTexture;

uniform float shininess;
uniform float shininessStrength;

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