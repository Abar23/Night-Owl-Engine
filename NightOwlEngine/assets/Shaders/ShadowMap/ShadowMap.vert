#include "VertexInputs.glsl"

uniform mat4 shadowViewProjectionMatrix;

void main()
{
    vec4 outputPositon = shadowViewProjectionMatrix * modelMatrix * vec4(materialPosition, 1.0);
    outVertexData.materialPosition = outputPositon.xyz / outputPositon.w;
    gl_Position = outputPositon;
}