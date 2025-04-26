vec4 ToneMapping(vec3 linearColor, float exposure, float contrast)
{
    vec3 exposureTimesLinearColor = exposure * linearColor;

    vec3 toneMappedColor = pow(exposureTimesLinearColor / (exposureTimesLinearColor + vec3(1.0, 1.0, 1.0)), vec3(contrast / 2.2));

    return vec4(toneMappedColor, 1.0);
}