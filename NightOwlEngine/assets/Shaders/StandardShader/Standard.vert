#include "VertexInputs.glsl"

void main()
{
    outVertexData.materialPosition = materialPosition;
    outVertexData.materialColor = materialColor;
    outVertexData.materialUvs = materialUvs;
    outVertexData.materialNormals = materialNormals;
    outVertexData.materialTangents = materialTangents;
    outVertexData.materialBitangents = materialBitangents;
    outVertexData.materialBoneIds = boneIds;
    outVertexData.materialBoneWeights = boneWeights;

    vec4 totalPosition = modelMatrix * vec4(materialPosition, 1.0);
    if (hasBones)
    {    
        totalPosition = vec4(0.0);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE; ++i)
        {
            if(boneIds[i] == -1) 
            {
                break;
            }

            vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(materialPosition, 1.0);
            totalPosition += localPosition * boneWeights[i];
        }
    }

    gl_Position = viewProjectionMatrix * totalPosition;
} 
