#version 460 core

out vec4 FragColor;

in vertexData
{
    vec3 materialPositon;
    vec3 materialColor;
    vec2 materialUvs;
} inVertexData;

in vec4 outputColor;

void main(void){
	FragColor = vec4(inVertexData.materialColor, 1.0);
} 