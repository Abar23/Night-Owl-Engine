#include "FragInputs.glsl"

layout (location = 0) out vec4 gBufferPosition;
layout (location = 1) out vec4 gBufferNormal;
layout (location = 2) out vec4 gBufferAlbedoSpec;
layout (location = 3) out vec4 gBufferViewSpacePosition;
layout (location = 4) out vec4 gBufferViewSpaceNormal;

in vec3 viewSpacePosition;
in vec3 viewSpaceNormal;

void main()
{    
    gBufferPosition = vec4(inVertexData.materialPosition, 1.0);
    gBufferViewSpacePosition = vec4(viewSpacePosition, 1.0);
    
    gBufferNormal = vec4(normalize(inVertexData.materialNormals), 1.0);
    gBufferViewSpaceNormal = vec4(normalize(viewSpaceNormal), 1.0);

    gBufferAlbedoSpec.rgb = diffuseColor.rgb;
    gBufferAlbedoSpec.a = 1.0;
}
