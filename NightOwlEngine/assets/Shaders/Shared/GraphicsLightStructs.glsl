struct PointLight
{
    vec3 position;
    vec3 color;
    float range;
    float intensity;
    float shadowBias;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float intensity;
    float shadowBias;
};
