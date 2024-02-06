struct Light
{
    vec3 position;
    vec3 color;
    float range;
};

layout (binding = 0, std430) buffer lightStorageBlock
{
    Light lights[];
};
