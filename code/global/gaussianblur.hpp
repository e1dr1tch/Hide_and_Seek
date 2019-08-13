#pragma once

//opengl
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template < typename T >
class GaussianBlur
{
    private:
       Shader* blurShader; 
       Shader* scaleShader; 

       vector < T* > colorBuffers;
       T* upscaleBuffer;
       T* downscaleBuffer;
       RenderQuad* quad;

       GLuint bluredTexture;

    public:
        GaussianBlur();

        void genBuffer(float width, float height, float scaleFactor = 1);
        void genBuffer(vec2 size, float scaleFactor = 1);

        GLuint blur(GLuint textureID, float intensity, float radius = 1);

        GLuint getTexture() const;
        pair < GLuint, GLuint > getBuffers() const;

        ~GaussianBlur();

};
