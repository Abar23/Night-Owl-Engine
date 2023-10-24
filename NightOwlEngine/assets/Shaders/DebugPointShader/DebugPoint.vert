layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 viewProjectionMatrix;

out vertexData
{
    vec3 materialColor;
} outVertexData;

void main()
{
    outVertexData.materialColor = color;

    gl_PointSize = 10.0;
    gl_Position = viewProjectionMatrix * vec4(position, 1.0);
}