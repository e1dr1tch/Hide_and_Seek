#version 330 core

#define BONES_AMOUNT 6
#define MAX_BONES_AMOUNT 50

layout (location = 0) in vec3 position;

layout (location = 3) in float boneIDs[BONES_AMOUNT];
layout (location = 9) in float boneWeights[BONES_AMOUNT];

uniform mat4 localTransform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 bones[MAX_BONES_AMOUNT];
uniform int meshWithBones;

void main()
{
    mat4 bonesTransform;

    if (meshWithBones == 1)
    {
        bonesTransform = mat4(0.0);

        for (int i = 0; i < BONES_AMOUNT; i++)
        {
            bonesTransform += bones[int(boneIDs[i])] * boneWeights[i];
        }
    }
    else
    {
        bonesTransform = mat4(1.0);    
    }

    gl_Position = projection * view * model * localTransform * bonesTransform * vec4(position, 1.0f);    
}
