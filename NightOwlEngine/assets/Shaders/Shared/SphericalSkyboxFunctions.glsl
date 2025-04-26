vec2 DirectionToSkyboxUv(vec3 directionVector)
{
    float u = 0.5 - atan(directionVector.x, directionVector.z) / (2.0 * PI);

    float v = acos(directionVector.y) / PI;

    return vec2(u, v); 
}

vec3 SkyboxUvToDirection(vec2 skyboxUv)
{
    float horizontalScale = 2.0 * PI * (0.5 - skyboxUv.x);

    float verticalScale = PI * skyboxUv.y;

    float sinVerticalScale = sin(verticalScale);

    float x = cos(horizontalScale) * sinVerticalScale;
    float y = sin(horizontalScale) * sinVerticalScale;
    float z = cos(verticalScale);

    return vec3(x, y, z);
}

vec3 DirectionVectorToSkyboxColor(sampler2D skyboxTexture, vec3 directionVector)
{
    vec2 skyboxUv = DirectionToSkyboxUv(directionVector);

    return texture(skyboxTexture, skyboxUv).xyz;
}

vec3 DirectionVectorToSkyboxColorAtMipMapLevel(sampler2D skyboxTexture, vec3 directionVector, float level)
{
    vec2 skyboxUv = DirectionToSkyboxUv(directionVector);

    return textureLod(skyboxTexture, skyboxUv, level).rgb;
}
