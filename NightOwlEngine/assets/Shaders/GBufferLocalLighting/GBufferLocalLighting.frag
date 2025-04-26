#include "Constants.glsl"
#include "FragInputs.glsl"
#include "GraphicsLightStructs.glsl"
#include "LightFragData.glsl"
#include "HammersleyStruct.glsl"
#include "HammersleyData.glsl"
#include "SphericalSkyboxFunctions.glsl"
#include "BrdfLightCalculationFunctions.glsl"
#include "ColorConversion.glsl"

uniform int lightIndex;

uniform float width;
uniform float height;
uniform float exposure;
uniform float contrast;

uniform vec3 cameraPosition;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main()
{
    // retrieve data from G-buffer
    vec3 fragmentPosition = texture(gPosition, gl_FragCoord.xy/vec2(width,height)).rgb;
    vec3 normal = texture(gNormal, gl_FragCoord.xy/vec2(width,height)).rgb;
    vec3 albedo = texture(gAlbedoSpec, gl_FragCoord.xy/vec2(width,height)).rgb;

    vec4 lightColor = CalculatePointLightBrdf(fragmentPosition, cameraPosition, normal, albedo, metallic, roughness, pointLights[lightIndex]);
    
    fragColor = ToneMapping(lightColor.xyz, exposure, contrast);
}
