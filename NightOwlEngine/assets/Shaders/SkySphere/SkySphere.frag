#include "FragInputs.glsl"
#include "Constants.glsl"
#include "SphericalSkyboxFunctions.glsl"
#include "ColorConversion.glsl"

uniform float exposure;
uniform float contrast;

uniform vec3 cameraPosition;

uniform sampler2D hdrSkybox;

void main()
{
    vec3 viewVector = normalize(cameraPosition - inVertexData.materialPosition);
    
    vec4 lightColor = vec4(DirectionVectorToSkyboxColor(hdrSkybox, viewVector), 1.0);

    fragColor = ToneMapping(lightColor.xyz, exposure, contrast);
}
