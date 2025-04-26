#include "FragInputs.glsl"

void main()
{
    // homogenous coords are [-1, 1], converting to [0, 1]
    float zDepth = 0.5 * (inVertexData.materialPosition.z + 1.0);
    float zDepthSquared = zDepth * zDepth;
    float zDepthCubed = zDepthSquared * zDepth;
    float zDepthFourthed = zDepthCubed * zDepth; 

    fragColor = vec4(zDepth, zDepthSquared, zDepthCubed, zDepthFourthed);
}