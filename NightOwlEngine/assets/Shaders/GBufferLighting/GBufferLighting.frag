#include "FragInputs.glsl"
#include "LightFragData.glsl"
#include "Constants.glsl"

uniform vec3 cameraPosition;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith2(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------

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

// Smith's method using Schlick-GGX (combination of the GGX and Schlick-Beckmann approximation)
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
	return SchlickGGX(N, L, k) * SchlickGGX(N, V, k);
}

vec3 FresnelSchilck(vec3 V, vec3 H, vec3 F0)
{
	float vdotH = clamp(dot(H, V), 0.0, 1.0);
	return F0 + (1.0 - F0) * pow((1.0 - vdotH), 5.0);
}

vec3 FresnelSchilckRoughness(vec3 V, vec3 H, vec3 F0, float roughness)
{
	float vdotH = clamp(dot(H, V), 0.0, 1.0);
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - vdotH, 5.0);
}

// void main()
// {             
//     // retrieve data from G-buffer
//     vec3 FragPos = texture(gPosition, inVertexData.materialUvs).rgb;
//     vec3 Normal = texture(gNormal, inVertexData.materialUvs).rgb;
//     vec3 Albedo = texture(gAlbedoSpec, inVertexData.materialUvs).rgb;
//     float Specular = texture(gAlbedoSpec, inVertexData.materialUvs).a;
    
//     // then calculate lighting as usual
//     vec3 lighting = Albedo * 0.1; // hard-coded ambient component
//     vec3 viewDir = normalize(cameraPosition - FragPos);
//     for(int i = 0; i < lights.length(); ++i)
//     {
//         // diffuse
//         vec3 lightDir = normalize(lights[i].position - FragPos);
//         vec3 diffuse = max(dot(Normal, lightDir), 0.0) * lights[i].color;
//         lighting += diffuse;
//     }
    
//     fragColor = vec4(lighting, 1.0);
// }

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
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < lights.length(); ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lights[i].position - fragPos);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - fragPos);
        float attenuation = max(1.0 / (distance * distance) - 1.0 / (2.0 * 2.0), 0.0);
        vec3 radiance = lights[i].color * attenuation;

        // Cook-Torrance BRDF
        // float NDF = TrowbridgeReitzNormalDistribution(N, H, roughness);   
        // float G = GeometrySmith(N, V, L, roughness);      
        // vec3 F = FresnelSchilckRoughness(H, V, F0, roughness);
           
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith2(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

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

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * nDotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ambientOcclusion;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    fragColor = vec4(color, 1.0);
}