#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class DirLight
{
    private:
        vec3 direction;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        mat4 view;
        mat4 projection;

        DepthBuffer* shadowBuffer;
        
    public:
        DirLight();
        DirLight(vec3 dir, vec3 amb, vec3 diff, vec3 spec);
       
        void genShadowBuffer(int width, int height);
        void genShadowBuffer(vec2 size);

        void setDirection(vec3 dir);
        void setAmbient(vec3 amb);
        void setDiffuse(vec3 diff);
        void setSpecular(vec3 spec);

        void setView(mat4 view);
        void setProjection(mat4 projection);

        vec3 getDirection() const;
        DepthBuffer* getShadowBuffer() const;
        
        void render(Shader* shader, GLuint index);
        void updateView(vec3 playerPosition);

        mat4 getView() const;
        mat4 getProjection() const;

        ~DirLight();
};
