#include "FragInputs.glsl"

layout (location = 0) out vec4 gBufferPosition;
layout (location = 1) out vec4 gBufferNormal;
layout (location = 2) out vec4 gBufferAlbedoSpec;

void main()
{    
    gBufferPosition = vec4(inVertexData.materialPosition, 1.0);
    
    gBufferNormal = vec4(normalize(inVertexData.materialNormals), 1.0);
    
    gBufferAlbedoSpec.rgb = diffuseColor.rgb;
    gBufferAlbedoSpec.a = 1.0;
}
