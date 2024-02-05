#include "VertexInputs.glsl"

void main()
{
    vec4 worldPosition = modelMatrix * vec4(materialPosition, 1.0);
    outVertexData.materialPosition = worldPosition.xyz;
    outVertexData.materialUvs = materialUvs;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    outVertexData.materialNormals = normalMatrix * materialNormals;

    gl_Position = viewProjectionMatrix * worldPosition;
}