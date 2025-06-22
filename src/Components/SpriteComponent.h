#ifndef SPRITECOMPONENT_h
#define SPRITECOMPONENT_h

#include <SDL2/SDL.h>

struct SpriteComponent
{
    int width;
    int height;
    SDL_Rect srcRect;
    std::string assetId;

    SpriteComponent(std::string assetId = "", int width = 1, int height = 1, int srcRectX = 0, int srcRectY = 0)
    {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->srcRect = {srcRectX, srcRectY, width, height};
    }
};

#endif
