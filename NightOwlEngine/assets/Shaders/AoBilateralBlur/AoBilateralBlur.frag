#include "Constants.glsl"
#include "GaussianKernel.glsl"
#include "FragInputs.glsl"

uniform bool isHorizonalBlur;

uniform float screenWidth;
uniform float screenHeight;

uniform vec3 cameraPosition;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D ambientObscurrance;

const float rangeKernelVariance = 0.01;

float RangeKernel(vec3 inputPixelNormal, float inputPixelCameraDepth, vec3 surroundingPixelNormal, float surroundingPixelCameraDepth)
{
    float normalDot = max(0.0, dot(inputPixelNormal, surroundingPixelNormal));

    float depthDifference = surroundingPixelCameraDepth - inputPixelCameraDepth;
    float exponetial = exp(-1.0 * (depthDifference * depthDifference) / 2.0 * rangeKernelVariance);
    float constant = 1.0 / sqrt(2.0 * PI * rangeKernelVariance);

    return normalDot * exponetial * constant;
}

void main()
{
    vec2 inputPixelScreenLocation = gl_FragCoord.xy / vec2(screenWidth, screenHeight);

    float inputPixelColor = texture(ambientObscurrance, inputPixelScreenLocation).r;
    vec3 inputPixelNormal = texture(gNormal, inputPixelScreenLocation).xyz;
    vec3 inputPixelPosition = texture(gPosition, inputPixelScreenLocation).xyz;
    float inputPixelCameraDepth = length(inputPixelPosition - cameraPosition);

    int gaussianKernelHalfLength = ambientOcclusionKernel.length() / 2;
    float inputPixelGaussian = ambientOcclusionKernel[gaussianKernelHalfLength];
    float inputPixelRangeKernal = RangeKernel(inputPixelNormal, inputPixelCameraDepth, inputPixelNormal, inputPixelCameraDepth);
    
    float totalWeightedColor = inputPixelGaussian * inputPixelRangeKernal * inputPixelColor;
    float totalWeight = inputPixelGaussian * inputPixelRangeKernal;

    vec2 offsetAxis = vec2(0.0, 1.0 / float(screenHeight));
    if (isHorizonalBlur)
    {
        offsetAxis = vec2(1.0 / float(screenWidth), 0.0);
    }

    for (int pixelIndex = -gaussianKernelHalfLength; pixelIndex < gaussianKernelHalfLength; ++pixelIndex)
    {
        if (pixelIndex == 0)
        {
            continue;
        }

        vec2 surroundingPixelLocation = inputPixelScreenLocation + float(pixelIndex) * offsetAxis;

        float surroundingPixelColor = texture(ambientObscurrance, surroundingPixelLocation).r;
        vec3 surroundingPixelNormal = texture(gNormal, surroundingPixelLocation).xyz;
        vec3 surroundingPixelPosition = texture(gPosition, surroundingPixelLocation).xyz;
        float surroundingPixelCameraDepth = length(surroundingPixelPosition - cameraPosition);

        float surroundingPixelGaussian = ambientOcclusionKernel[pixelIndex + gaussianKernelHalfLength];
        float surroundingPixelRangeKernal = RangeKernel(inputPixelNormal, inputPixelCameraDepth, surroundingPixelNormal, surroundingPixelCameraDepth);

        totalWeightedColor += surroundingPixelGaussian * surroundingPixelRangeKernal * surroundingPixelColor;
        totalWeight += surroundingPixelGaussian * surroundingPixelRangeKernal;
    }

    float totalFilteredPixel = totalWeightedColor / (totalWeight + 0.0001);

    fragColor = vec4(totalFilteredPixel, 0.0, 0.0, 1.0);
}