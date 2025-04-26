#include "FragInputs.glsl"

uniform sampler2D finalRenderedScene;

void main()
{
    vec3 renderedScenePixel = texture(finalRenderedScene, inVertexData.materialUvs).rgb;

    float brightness = dot(renderedScenePixel.rgb, vec3(0.2126, 0.7152, 0.0722));

    fragColor = (brightness > 0.9) ? vec4(renderedScenePixel.rgb, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}
