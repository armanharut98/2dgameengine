#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL2/SDL.h>

struct AnimationComponent
{
    int numFrames;
    int currentFrame;
    int frameSpeedRate;
    int startTime;
    bool isLooping;

    AnimationComponent(int numFrames = 0, int frameSpeedRate = 0, bool isLooping = false)
    {
        this->numFrames = numFrames;
        this->currentFrame = 1;
        this->frameSpeedRate = frameSpeedRate;
        this->startTime = SDL_GetTicks();
        this->isLooping = isLooping;
    }
};

#endif
