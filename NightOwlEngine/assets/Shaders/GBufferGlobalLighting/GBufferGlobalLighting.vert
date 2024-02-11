#include "VertexInputs.glsl"

out mat3 normalMatrix;

void main()
{
    outVertexData.materialUvs = materialUvs;
    
    normalMatrix = transpose(inverse(mat3(modelMatrix)));

    gl_Position = vec4(materialPosition, 1.0);
}