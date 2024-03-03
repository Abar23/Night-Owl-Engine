#include "FragInputs.glsl"
#include "GraphicsLightStructs.glsl"
#include "LightFragData.glsl"

void main()
{
    vec4 outputColor = diffuseColor;

    outputColor *= texture(diffuseTexture, inVertexData.materialUvs);
    
    if(outputColor.a < 0.1)
    {
        discard;
    }

    vec3 color = vec3(1.0);
    for(int lightIndex = 0; lightIndex < pointLights.length(); ++lightIndex)
    {
        color = mix(color, pointLights[lightIndex].color, 0.5);
    }

	fragColor = vec4(inVertexData.materialNormals, 1.0) * vec4(color, 1.0);
} 