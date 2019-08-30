#include "animation.hpp"

Animation::Animation(string name)
{
    this->name = name;
    animId = 0;

    framesRange = vec2(0); // default is from start to the end

    curFrame = 0.0;

    speed = 0.0; // default is 24 frames per second
    loop = false;
}

Animation::Animation(string name, int animId, vec2 framesRange, float speed, bool loop)
{
    this->name = name;
    this->animId = animId;

    this->framesRange = framesRange;

    curFrame = framesRange.x;

    this->speed = speed;

    this->loop = loop;
}

void Animation::setName(string name)
{
    this->name = name;
}

void Animation::setAnimId(int animId)
{
    this->animId = animId;
}

void Animation::setFramesRange(vec2 framesRange)
{
    this->framesRange = framesRange;
}

void Animation::setSpeed(float speed)
{
    this->speed = speed;
}

void Animation::setLoop(bool loop)
{
    this->loop = loop;
}
        
string Animation::getName() const
{
    return name;
}

int Animation::getAnimId() const
{
    return animId;
}

int Animation::getStartFrame() const
{
    return framesRange.x;
}

int Animation::getEndFrame() const
{
    return framesRange.y;
}

vec2 Animation::getFramesRange() const
{
    return framesRange;
}

float Animation::getSpeed() const
{
    return speed;
}

bool Animation::getLoop() const
{
    return loop;
}

void Animation::fromStart()
{
    curFrame = framesRange.x;    
}

void Animation::fromFrame(float frame)
{
    if (frame >= framesRange.x && frame <= framesRange.y)
    {
        curFrame = frame;
    }
}

bool Animation::nextFrame()
{ 
    if (curFrame < framesRange.x)
    {
        fromStart();
    }
    else if (curFrame >= framesRange.y)
    {
        if (loop)
        {
            fromStart();
        }
        else
        {
            return false;
        }
    }
    
    curFrame += speed;

    return true;
}

float Animation::getCurFrame() const
{
    return curFrame;
}

Animation::~Animation(){}