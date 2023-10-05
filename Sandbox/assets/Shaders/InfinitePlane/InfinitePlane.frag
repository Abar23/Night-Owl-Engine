#include "/FragInputs.glsl"

in vec3 nearPoint;
in vec3 farPoint;
in mat4 fragmentViewProjection;
in float near;
in float far;

vec4 GetGridColor(vec3 fragmentPosition, float scale, bool drawAxis) 
{
    vec2 coordinateOnGrid = fragmentPosition.xz * scale;
    vec2 derivative = fwidth(coordinateOnGrid);
    vec2 grid = abs(fract(coordinateOnGrid - 0.5) - 0.5) / derivative;
    
    float gridLine = min(grid.x, grid.y);
    
    float minimumZ = min(derivative.y, 1);
    float minimumX = min(derivative.x, 1);

    vec4 gridColor = vec4(0.2, 0.2, 0.2, 1.0 - min(gridLine, 1.0));
    
    if(fragmentPosition.x > -0.1 * minimumX && fragmentPosition.x < 0.1 * minimumX)
    {
        gridColor.z = 1.0;
    }

    if(fragmentPosition.z > -0.1 * minimumZ && fragmentPosition.z < 0.1 * minimumZ)
    {
        gridColor.x = 1.0;
    }

    return gridColor;
}

float CalculateDepth(vec3 position) 
{
    vec4 clipSpacePosition = fragmentViewProjection * vec4(position.xyz, 1.0);

    return (clipSpacePosition.z / clipSpacePosition.w);
}

float CalculateLinearDepth(vec3 position) 
{
    vec4 clipSpacePosition = fragmentViewProjection * vec4(position.xyz, 1.0);
    
    float clipSpaceDepth = (clipSpacePosition.z / clipSpacePosition.w) * 2.0 - 1.0;
    
    float linearDepth = (2.0 * near * far) / (far + near - clipSpaceDepth * (far - near));
    
    return linearDepth / far;
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = ((gl_DepthRange.diff * CalculateDepth(fragPos3D)) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

    float linearDepth = CalculateLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    FragColor = (GetGridColor(fragPos3D, 1, true) + GetGridColor(fragPos3D, 5, true))* float(t > 0);
    FragColor.a *= fading;

    if (FragColor.a < 0.1)
    {
        discard;
    }
}