#version 460 core

out vec4 FragColor;

in vertexData
{
    vec3 materialPositon;
    vec3 materialColor;
    vec2 materialUvs;
} inVertexData;

in vec4 outputColor;

uniform sampler2D inputTexture;

void main(void)
{
    vec4 texelColor = texture(inputTexture, inVertexData.materialUvs);
	FragColor = vec4(texelColor.x, texelColor.y, texelColor.z, 0.0, 1.0);
} 