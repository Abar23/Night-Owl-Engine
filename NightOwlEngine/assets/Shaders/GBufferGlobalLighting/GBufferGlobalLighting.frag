#include "Constants.glsl"
#include "FragInputs.glsl"
#include "GraphicsLightStructs.glsl"
#include "HammersleyStruct.glsl"
#include "HammersleyData.glsl"
#include "SphericalSkyboxFunctions.glsl"
#include "BrdfLightCalculationFunctions.glsl"

uniform float screenWidth;
uniform float screenHeight;

uniform vec3 cameraPosition;

uniform mat4 shadowViewProjectionMatrix;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D shadowMap;

uniform sampler2D hdrIrradianceMap;
uniform sampler2D hdrSkybox;

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

    vec4 fragmentLightSpacePosition = shadowViewProjectionMatrix * vec4(fragmentPosition, 1.0);
    float shadow = MomentShadowMapCalculation(shadowMap, fragmentLightSpacePosition, fragmentPosition, normal, globalLight);

    vec2 screenDimensions = vec2(screenWidth, screenHeight);
    fragColor = CalculateDirectionalLightBrdf(fragmentPosition, cameraPosition, normal, albedo, screenDimensions, metallic, roughness, shadow, globalLight, hdrIrradianceMap, hdrSkybox);
}
