#include "VertexInputs.glsl"

void main()
{
    outVertexData.materialUvs = materialUvs;
    
    gl_Position = vec4(materialPosition, 1.0);
}