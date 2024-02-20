layout (binding = 0, std430) buffer pointLightStorageBlock
{
    PointLight pointLights[];
};

layout (binding = 1, std430) buffer directionalLightStorageBlock
{
    DirectionalLight directionalLights[];
};
