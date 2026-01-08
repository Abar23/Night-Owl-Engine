#include "FragInputs.glsl"

uniform sampler2D finalRenderedScene;
uniform sampler2D blurredBloomTexture;

void main()
{    
    vec3 renderedScenePixel = texture(finalRenderedScene, inVertexData.materialUvs).rgb;
    vec3 blurredBloomColor = texture(blurredBloomTexture, inVertexData.materialUvs).rgb;

    vec3 finalColor = renderedScenePixel + blurredBloomColor;

    fragColor = vec4(finalColor, 1.0);
}