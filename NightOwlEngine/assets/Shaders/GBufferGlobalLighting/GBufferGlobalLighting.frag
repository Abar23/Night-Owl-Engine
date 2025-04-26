#include "Constants.glsl"
#include "FragInputs.glsl"
#include "GraphicsLightStructs.glsl"
#include "HammersleyStruct.glsl"
#include "HammersleyData.glsl"
#include "SphericalSkyboxFunctions.glsl"
#include "BrdfLightCalculationFunctions.glsl"
#include "ColorConversion.glsl"

uniform float screenWidth;
uniform float screenHeight;
uniform float exposure;
uniform float contrast;

uniform vec3 cameraPosition;

uniform mat4 shadowViewProjectionMatrix;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D shadowMap;

uniform sampler2D hdrIrradianceMap;
uniform sampler2D hdrSkybox;

uniform sampler2D blurredAmbientObscurrance;

layout (std430, binding = 0) buffer globalDirectionalLight
{
    DirectionalLight globalLight;
};

void main()
{
    // retrieve data from G-buffer
    vec3 fragmentPosition = texture(gPosition, inVertexData.materialUvs).rgb;
    vec3 normal = texture(gNormal, inVertexData.materialUvs).rgb;
    vec3 albedo = texture(gAlbedoSpec, inVertexData.materialUvs).rgb;
    float ambientObscurranceColor = texture(blurredAmbientObscurrance, inVertexData.materialUvs).r;

    vec4 fragmentLightSpacePosition = shadowViewProjectionMatrix * vec4(fragmentPosition, 1.0);
    float shadow = VarianceShadowMapCalculation(shadowMap, fragmentLightSpacePosition, fragmentPosition, normal, globalLight);

    vec2 screenDimensions = vec2(screenWidth, screenHeight);
    vec4 lightColor = CalculateDirectionalLightBrdf(fragmentPosition, cameraPosition, normal, albedo, screenDimensions, metallic, roughness, shadow, ambientObscurranceColor, globalLight, hdrIrradianceMap, hdrSkybox);

    fragColor = ToneMapping(lightColor.xyz, exposure, contrast);
}
