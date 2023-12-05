layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vertexData
{
    vec3 materialColor;
} outVertexData;

uniform mat4 viewProjectionMatrix;

void main()
{
    outVertexData.materialColor = color;
    gl_Position = viewProjectionMatrix * vec4(position, 1.0);
}