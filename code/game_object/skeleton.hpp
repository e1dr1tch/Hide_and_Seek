#pragma once

#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#define MAX_BONES_AMOUNT 50

class Skeleton
{
    private:
        map < string, Bone* > bones; 
        vector < mat4 > bonesMatrices; 

        float time; 

        Animation* activeAnimation; 

        void renderBonesMatrices(Shader* shader); 
        
    public:
        Skeleton(map < string, Bone* > &bones);

        void playAnimation(Animation* anim, bool reset = true); 
        void stopAnimation(); 

        void update(Shader* shader); 

        ~Skeleton();
};