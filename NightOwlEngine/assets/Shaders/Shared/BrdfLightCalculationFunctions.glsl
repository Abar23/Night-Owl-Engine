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

vec2 quadraticEquation(float a, float b, float c) 
{
    float epsilon = 1e-6;

    float discriminant = b * b - 4.0 * a * c;

    if (discriminant > epsilon) 
    {
        // Two real roots
        float root1 = (-b - sqrt(discriminant)) / (2.0 * a);
        float root2 = (-b + sqrt(discriminant)) / (2.0 * a);
        return vec2(min(root1, root2), max(root1, root2));
    } 
    
    if (discriminant > 0.0 && discriminant < epsilon) 
    {
        // One real root (double root)
        float root = -b / (2.0 * a);
        return vec2(root, root);
    }

    // Complex roots
    float realPart = -b / (2.0 * a);
    float imaginaryPart = sqrt(-discriminant) / (2.0 * a);
    return vec2(realPart, imaginaryPart);
}

vec3 CholeskyDecomposition(float m11, float m12, float m13, float m22, float m23, float m33, float z1, float z2, float z3)
{
    float a = sqrt(m11);
    float b = m12 / a;
    float c = m13 / a;
    float d = sqrt(m22 - b * b);
    float e = (m23 - b * c) / d;
    float f = sqrt(m33 - c * c - e * e);

    // Prevent negative values for a, d, f since they use sqrt function
    if (a < 0.0)
    {
        a = 1e-4;
    }

    if (d < 0.0)
    {
        d = 1e-4;
    }

    if (f < 0.0)
    {
        f = 1e-4;
    }

    float cHat1 = z1 / a;
    float cHat2 = (z2 - b * cHat1) / d;
    float cHat3 = (z3 - c * cHat1 - e * cHat2) / f;

    float c3 = cHat3 / f;
    float c2 = (cHat2 - e * c3) / d;
    float c1 = (cHat1 - b * c2 - c * c3) / a;

    return vec3(c1, c2, c3);
}

float MomentShadowMapCalculation(sampler2D shadowMap, 
                                 vec4 fragmentPositionInLightSpace, 
                                 vec3 fragmentPosition,
                                 vec3 normal,
                                 DirectionalLight directionalLight)
{
    // perform perspective divide
    vec3 projectedLightCoordinate = fragmentPositionInLightSpace.xyz / fragmentPositionInLightSpace.w;
    
    // transform to [0,1] range
    projectedLightCoordinate = projectedLightCoordinate * 0.5 + 0.5;
    float zF = projectedLightCoordinate.z;
    float zFSquared = zF * zF;

    // Step 1
    vec4 b = texture(shadowMap, projectedLightCoordinate.xy);
    vec4 bPrime = (1.0 - 1e-3) * b + 1e-3 * vec4(0.5); 

    // Step 2
    vec3 c = CholeskyDecomposition(1.0, bPrime.x, bPrime.y, bPrime.y, bPrime.z, bPrime.w, 1, zF, zFSquared);

    // Step 3
    vec2 solutions = quadraticEquation(c.z, c.y, c.x);
    float z2 = solutions.x;
    float z3 = solutions.y;

    // Step 4
    float epsilon = 1e-6;
    if (zF < z2 || abs(zF - z2) < epsilon)
    {
        return 0.0;
    }

    // Step 5
    if (zF < z3 || abs(zF - z3) < epsilon)
    {
        float numerator = zF * z3 - bPrime.x * (zF + z3) + bPrime.y;
        float denominator = (z3 - z2) * (zF - z2);
        return numerator / denominator;
    }

    // Step 6
    float numerator = z2 * z3 - bPrime.x * (z2 + z3) + bPrime.y;
    float denominator = (zF - z2) * (zF - z3);
    return 1.0 - (numerator / denominator);
}

float CalculateDirectionalLightShadow(sampler2D shadowMap, 
                                      vec4 fragmentPositionInLightSpace, 
                                      vec3 fragmentPosition,
                                      vec3 normal,
                                      DirectionalLight directionalLight)
{
    // perform perspective divide
    vec3 projectedLightCoordinate = fragmentPositionInLightSpace.xyz / fragmentPositionInLightSpace.w;
    
    // transform to [0,1] range
    projectedLightCoordinate = projectedLightCoordinate * 0.5 + 0.5;
    
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projectedLightCoordinate.xy).r; 
    
    // get depth of current fragment from light's perspective
    float currentDepth = projectedLightCoordinate.z;

    // calculate bias (based on depth map resolution and slope)
    float bias = max(directionalLight.shadowBias * (1.0 - dot(normal, directionalLight.direction)), 0.00005);
   
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    // Moment shadow map stuff here?
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projectedLightCoordinate.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec4 CalculatePointLightBrdf(vec3 fragmentPosition,
                             vec3 cameraPosition,
                             vec3 normal, 
                             vec3 albedo, 
                             float metallic, 
                             float roughness, 
                             PointLight pointLight)
{
    float distance = length(pointLight.position - fragmentPosition);
    if (distance > pointLight.range)
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }

    vec3 N = normal;
    vec3 V = normalize(cameraPosition - fragmentPosition);

    // calculate reflectance at normal incidence   
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // calculate per-light radiance
    vec3 L = normalize(pointLight.position - fragmentPosition);
    vec3 H = normalize(V + L);

    float attenuation = (1.0 / (distance * distance) - 1.0 / (pointLight.range * pointLight.range));
    vec3 radiance = pointLight.color * attenuation;

    // Cook-Torrance BRDF
    float NDF = TrowbridgeReitzNormalDistribution(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = FresnelSchilckRoughness(H, V, F0, roughness);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
    
    // kS is equal to Fresnel
    vec3 kS = F;
    // to preserve energy, Kd, the diffuse component, should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply Kd by the inverse the metallic so only non-metals have diffuse lighting
    kD *= 1.0 - metallic;	  

    float nDotL = max(dot(N, L), 0.0);
    vec3 color = (kD * albedo / PI + specular) * radiance * nDotL * pointLight.intensity;

    // HDR tonemapping and gamma correct
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/0.4));

    return vec4(color, 1.0);
}

vec4 CalculateDirectionalLightBrdf(vec3 fragmentPosition, 
                                   vec3 cameraPosition,
                                   vec3 normal, 
                                   vec3 albedo, 
                                   float metallic, 
                                   float roughness,
                                   float shadow,
                                   DirectionalLight directionalLight)
{
    vec3 N = normal;
    vec3 V = normalize(cameraPosition - fragmentPosition);

    // calculate reflectance at normal incidence    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // calculate per-light radiance using a global light that is facing downwards
    vec3 L = directionalLight.direction;
    vec3 H = normalize(V + L);
    
    // Cook-Torrance BRDF
    float NDF = TrowbridgeReitzNormalDistribution(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = FresnelSchilckRoughness(H, V, F0, roughness);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
    
    // kS is equal to Fresnel
    vec3 kS = F;
    // to preserve energy, Kd, the diffuse component, should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply Kd by the inverse the metallic so only non-metals have diffuse lighting
    kD *= 1.0 - metallic;	

    // scale light by NdotL
    float nDotL = max(dot(N, L), 0.0);
    vec3 color = (kD * albedo / PI + specular) * directionalLight.color * nDotL * directionalLight.intensity * (1.0 - shadow);

    // HDR tonemapping and gamma correct
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/0.4));

    return vec4(color, 1.0);
}