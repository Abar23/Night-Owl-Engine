#version 460 core

out vec4 FragColor;

in vertexData
{
    vec3 materialPositon;
    vec3 materialColor;
    vec2 materialUvs;
} inVertexData;

uniform bool isInputTextureSet = false;
uniform sampler2D inputTexture;

void main(void)
{
    vec4 outputColor = vec4(inVertexData.materialColor, 1.0);

    if(isInputTextureSet)
    {
        outputColor = texture(inputTexture, inVertexData.materialUvs);
    }

	FragColor = outputColor;
} 