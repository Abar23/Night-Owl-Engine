#include "VertexInputs.glsl"

out vec3 viewSpacePosition;
out vec3 viewSpaceNormal;

void main()
{
    vec4 worldPosition = modelMatrix * vec4(materialPosition, 1.0);
    outVertexData.materialPosition = worldPosition.xyz;
    viewSpacePosition = (viewMatrix * worldPosition).xyz;

    outVertexData.materialUvs = materialUvs;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    outVertexData.materialNormals = normalMatrix * materialNormals;

    mat3 normalViewMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));
    viewSpaceNormal = normalViewMatrix * materialNormals;

    gl_Position = viewProjectionMatrix * worldPosition;
}