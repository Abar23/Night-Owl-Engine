#include "/VertexInputs.glsl"

out vec3 nearPoint;
out vec3 farPoint;
out mat4 fragmentViewProjection;
out float near;
out float far;

vec3 UnprojectPoint(float x, float y, float z) 
{
    vec4 unprojectedPoint =  inverse(viewProjectionMatrix) * vec4(x, y, z, 1.0);

    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
    outVertexData.materialPosition = materialPosition;
    
    nearPoint = UnprojectPoint(materialPosition.x, materialPosition.y, 0.0);
    farPoint = UnprojectPoint(materialPosition.x, materialPosition.y, 1.0);
    
    fragmentViewProjection = viewProjectionMatrix;
    near = 0.01;
    far = 100.0;

    gl_Position = vec4(materialPosition, 1.0);
}
