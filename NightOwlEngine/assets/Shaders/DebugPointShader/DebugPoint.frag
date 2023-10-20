out vec4 FragColor;

in vertexData
{
    vec3 materialColor;
} inVertexData;

void main()
{
    FragColor = vec4(inVertexData.materialColor, 1.0);
}