#include "VertexInputs.glsl"

void main()
{
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(materialPosition, 1.0);
}