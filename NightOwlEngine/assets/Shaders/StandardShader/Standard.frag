#include "FragInputs.glsl"

void main()
{
    vec4 outputColor = diffuseColor;

    outputColor *= texture(diffuseTexture, inVertexData.materialUvs);
    
    if(outputColor.a < 0.1)
    {
        discard;
    }

	FragColor = vec4(inVertexData.materialNormals, 1.0);
} 