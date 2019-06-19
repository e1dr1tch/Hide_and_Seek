#include "../shader/shader.hpp"

#include "../framebuffer/framebuffer.hpp"
#include "../framebuffer/colorbuffer.hpp"

#include "../window/renderquad.hpp"
#include "../window/glfwevents.hpp"
#include "../window/window.hpp"

#include "camera.hpp"

Camera::Camera(Window* window, vec3 cameraPos, vec3 cameraForward, float speed)
{
    this->window = window;

    sensitivity.x = 2 / window->getSize().x;
    sensitivity.y = (2 / (window->getSize().x / window->getSize().y)) / window->getSize().y;

    Pos = cameraPos;
    moveDirection = vec3(0, 0, 0);
    Forward = normalize(cameraForward);
    Up = vec3(0, 1, 0);

    Left = normalize(cross(Up, Forward));

    this->speed = speed;

    prevCoords = window->getSize() / 2.0f;
}

void Camera::lookAction()
{
    if (!window->isMouseMoved())
    {
        return;
    }
        
    vec2 pos = window->getMousePosition();

    float xoffset = pos.x - prevCoords.x; 
    float yoffset = pos.y - prevCoords.y;

    xoffset *= sensitivity.x;
    yoffset *= sensitivity.y;


    mat4 camView(1.0);

    Forward = mat3(rotate(camView, GLfloat(-xoffset), normalize(Up))) * Forward;
        
    Left = normalize(cross(Up, Forward));
       
    vec3 ForwardTmp = Forward;
    Forward = mat3(rotate(camView, GLfloat(yoffset), normalize(Left))) * Forward;
        
    if (dot(cross(Up, Forward), Left) < 0)
    {
        Forward = ForwardTmp;
    }
    
    Forward = normalize(Forward);

    prevCoords = pos;
}

void Camera::moveAction()
{
    if (window->isKeyPressed(GLFW_KEY_W))
    {
        moveDirection += Forward;
    }

    if (window->isKeyPressed(GLFW_KEY_S))
    {
        moveDirection -= Forward;
    }

    if (window->isKeyPressed(GLFW_KEY_D))
    {
        moveDirection -= Left;
    }

    if (window->isKeyPressed(GLFW_KEY_A))
    {
        moveDirection += Left;
    }

    if (window->isKeyPressed(GLFW_KEY_E))
    {
        moveDirection += Up;
    }

    if (window->isKeyPressed(GLFW_KEY_Q))
    {
        moveDirection -= Up;
    }
}

void Camera::resetPrevCoords()
{
    prevCoords = window->getMousePosition();
}

void Camera::setPosition(float worldX, float worldY, float worldZ)
{
    Pos = vec3(worldX, worldY, worldZ);
}
        
void Camera::setUp(vec3 Up)
{
    this->Up = normalize(Up);
}
        
void Camera::setSpeed(float speed)
{
    this->speed = speed;
}

vec2 Camera::getWindowSize() const
{
    return window->getSize();
}

vec3 Camera::getPosition() const
{
    return Pos;
}

vec3 Camera::getForward() const
{
    return Forward;
}

vec3 Camera::getLeft() const
{
    return Left;
}

vec3 Camera::getUp() const
{
    return Up;
}

mat4 Camera::getView() const
{
    return lookAt(Pos, Pos + Forward, Up);
}

/* consider mode? */
void Camera::update(/* mode */)
{
    lookAction();
    moveAction();

    if (moveDirection != vec3(0, 0, 0))
    {
        Pos += normalize(moveDirection) * vec3(speed);
    }

    moveDirection = vec3(0, 0, 0);
}

Camera::~Camera(){}