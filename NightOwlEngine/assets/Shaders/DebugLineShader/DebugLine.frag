out vec4 fragColor;

in vertexData
{
    vec3 materialColor;
} inVertexData;

void main()
{
    fragColor = vec4(inVertexData.materialColor, 1.0);
}