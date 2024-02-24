#include "FragInputs.glsl"

in vec4 test;

void main()
{
    float zDepth = 0.5 * ((test.z / test.w) + 1.0);
    float zDepthSquared = zDepth * zDepth;
    float zDepthCubed = zDepthSquared * zDepth;
    float zDepthFourthed = zDepthCubed * zDepth; 

    fragColor = vec4(zDepth, zDepthSquared, zDepthCubed, zDepthFourthed);
}