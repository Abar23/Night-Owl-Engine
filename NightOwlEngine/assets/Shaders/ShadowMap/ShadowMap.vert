#include "VertexInputs.glsl"

uniform mat4 shadowViewProjectionMatrix;

void main()
{
    gl_Position = shadowViewProjectionMatrix * modelMatrix * vec4(materialPosition, 1.0);
}