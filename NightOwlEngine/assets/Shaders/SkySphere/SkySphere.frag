#include "FragInputs.glsl"
#include "Constants.glsl"
#include "SphericalSkyboxFunctions.glsl"

uniform vec3 cameraPosition;

uniform sampler2D hdrSkybox;

void main()
{
    vec3 viewVector = normalize(cameraPosition - inVertexData.materialPosition);
    fragColor = vec4(DirectionVectorToSkyboxColor(hdrSkybox, viewVector), 1.0);
}
