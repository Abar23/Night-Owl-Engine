#include "VertexInputs.glsl"

uniform mat4 shadowViewProjectionMatrix;

out vec4 test;

void main()
{
    vec4 outputPositon = shadowViewProjectionMatrix * modelMatrix * vec4(materialPosition, 1.0);
    test = outputPositon;
    gl_Position = test;
}