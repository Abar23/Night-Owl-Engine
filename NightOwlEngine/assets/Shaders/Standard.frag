#version 460 core

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_shading_language_include : require

out vec4 FragColor;

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

void main(void)
{
    vec4 outputColor = diffuseColor;

    outputColor *= texture(diffuseTexture, inVertexData.materialUvs);
    
    if(outputColor.a < 0.1)
    {
        discard;
    }

	FragColor = vec4(inVertexData.materialNormals, 1.0);
} 