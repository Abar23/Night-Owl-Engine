#include "FragInputs.glsl"
#include "Constants.glsl"

uniform vec3 cameraPosition;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

float TrowbridgeReitzNormalDistribution(vec3 N, vec3 H, float roughness)
{
    float roughnessSquared = roughness * roughness;
    float a2 = roughnessSquared * roughnessSquared;
    float nDotH = max(dot(N, H), 0.0);
    float nDotHSquared = nDotH * nDotH;

    float distribution = nDotH * nDotH * (a2 - 1.0) + 1.0;

    return a2 / (PI * distribution * distribution);
}

float SchlickGGX(vec3 N, vec3 V, float k)
{
	float nDotV = max(dot(N, V), 0.0);
	float nom = nDotV;
	float denom = nDotV * (1.0 - k) + k; 
	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
	return SchlickGGX(N, L, k) * SchlickGGX(N, V, k);
}

vec3 FresnelSchilckRoughness(vec3 V, vec3 H, vec3 F0, float roughness)
{
	float vdotH = clamp(dot(H, V), 0.0, 1.0);
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - vdotH, 5.0);
}

void main()
{
    // retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, inVertexData.materialUvs).rgb;
    vec3 normal = texture(gNormal, inVertexData.materialUvs).rgb;
    vec3 albedo = texture(gAlbedoSpec, inVertexData.materialUvs).rgb;
    //float specular = texture(gAlbedoSpec, inVertexData.materialUvs).a;

    vec3 N = normal;
    vec3 V = normalize(cameraPosition - fragPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 accumulatedRadiance = vec3(0.0);

    // calculate per-light radiance
    vec3 L = normalize(vec3(0.0, 2.0, 0.0));
    vec3 H = normalize(V + L);
    
    vec3 radiance = vec3(6.0);

    // Cook-Torrance BRDF
    float NDF = TrowbridgeReitzNormalDistribution(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = FresnelSchilckRoughness(H, V, F0, roughness);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
    
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	  

    // scale light by NdotL
    float nDotL = max(dot(N, L), 0.0);

    // accumulatte radiance
    accumulatedRadiance = (kD * albedo / PI + specular) * radiance * nDotL;

    vec3 color = accumulatedRadiance;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamm corraect
    color = pow(color, vec3(1.0/0.4)); 

    fragColor = vec4(color, 1.0);
}