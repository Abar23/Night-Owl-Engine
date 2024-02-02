#include "FragInputs.glsl"

void main()
{
    vec4 outputColor = diffuseColor;

    outputColor *= texture(diffuseTexture, inVertexData.materialUvs);
    
    if(outputColor.a < 0.1)
    {
        discard;
    }

    vec3 color = vec3(1.0);
    for(int lightIndex = 0; lightIndex < lights.length(); ++lightIndex)
    {
        color = mix(color, lights[lightIndex].color, 0.5);
    }

	FragColor = vec4(inVertexData.materialNormals, 1.0) * vec4(color, 1.0);
} 