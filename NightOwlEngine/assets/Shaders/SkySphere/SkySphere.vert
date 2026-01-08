#include "VertexInputs.glsl"

void main()
{
    vec4 worldPosition = modelMatrix * vec4(materialPosition, 1.0);
    outVertexData.materialPosition = worldPosition.xyz;

    gl_Position = viewProjectionMatrix * worldPosition;
}
