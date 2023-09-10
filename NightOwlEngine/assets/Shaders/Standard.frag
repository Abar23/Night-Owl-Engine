#version 460 core

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

uniform bool isInputTextureSet = false;
uniform sampler2D inputTexture;

void main(void)
{
    vec4 outputColor = vec4(inVertexData.materialColor, 1.0);

    if(isInputTextureSet)
    {
        outputColor = texture(inputTexture, inVertexData.materialUvs);

        if(outputColor.a < 0.1)
        {
            discard;
        }
    }

	FragColor = vec4(inVertexData.materialNormals, 1.0);
} 