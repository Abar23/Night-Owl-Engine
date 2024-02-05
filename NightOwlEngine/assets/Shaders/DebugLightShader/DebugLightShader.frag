#include "FragInputs.glsl"
#include "LightFragData.glsl"

uniform int lightIndex;

void main()
{
    fragColor = vec4(lights[lightIndex].color, 1.0);
}