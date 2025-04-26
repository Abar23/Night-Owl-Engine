#include "Constants.glsl"
#include "FragInputs.glsl"

uniform float screenWidth;
uniform float screenHeight;
uniform float worldSpaceRange;
uniform float scale;
uniform float contrast;

uniform vec3 cameraPosition;

uniform sampler2D gBufferViewSpacePosition;
uniform sampler2D gBufferViewSpaceNormal;

uniform int numberOfSamples;

uniform bool enableAmbientObscurrance;

float HeavisideStepFunction(float x) 
{
    return x < 0.0 ? 0.0 : 1.0;
}

float CalculateAmbientObscurrance()
{
    ivec2 integerCoefficients = ivec2(gl_FragCoord.xy);
    vec2 floatCoordinates = vec2(gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight);
    int phi = (30 * integerCoefficients.x ^ integerCoefficients.y) + 10 * integerCoefficients.x * integerCoefficients.y;

    // retrieve data from G-buffer
    vec3 fragmentPosition = texture(gBufferViewSpacePosition, floatCoordinates).rgb;
    vec3 normal = texture(gBufferViewSpaceNormal, floatCoordinates).rgb;
    float cameraSpaceDepth = fragmentPosition.z;

    float c = 0.1 * worldSpaceRange;
    float cSquared = c * c;
    float sigma = 0.0001;
    float constant = 2.0 * PI * c / float(numberOfSamples);

    float occlusion = 0.0;
    for(int pointIndex = 0; pointIndex < numberOfSamples; ++pointIndex)
    {
        float alpha = (pointIndex + 0.5) / float(numberOfSamples);

        float h = alpha * worldSpaceRange / cameraSpaceDepth;

        float theta = 2.0 * PI * alpha * (7.0 * float(numberOfSamples) / 9.0) + float(phi);

        vec2 spiralPointTextureLocation = floatCoordinates + h * vec2(cos(theta), sin(theta));
        vec3 spiralPosition = texture(gBufferViewSpacePosition, spiralPointTextureLocation).rgb;
        float spiralPointDepth = spiralPosition.z;

        vec3 currentFragmentPositionToSpiralPosition = spiralPosition - fragmentPosition;

        float heavisideValue = HeavisideStepFunction(worldSpaceRange - length(currentFragmentPositionToSpiralPosition));
        float numerator = max(0, dot(normal, currentFragmentPositionToSpiralPosition) - sigma * spiralPointDepth) * heavisideValue;
        float denominator = max(cSquared, dot(currentFragmentPositionToSpiralPosition, currentFragmentPositionToSpiralPosition));

        occlusion += numerator / (denominator + 0.0001);        
    }

    float S = constant * occlusion;

    float ambientFactor = max(0.0, pow(1.0 - scale * S, contrast));

    return ambientFactor;
}

void main()
{    
    float ambientObscurrance = 1.0;
    if (enableAmbientObscurrance)
    {
        ambientObscurrance = CalculateAmbientObscurrance(); 
    }
    fragColor = vec4(ambientObscurrance, 0.0, 0.0, 1.0);
}
