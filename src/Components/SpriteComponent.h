#ifndef SPRITECOMPONENT_h
#define SPRITECOMPONENT_h

#include <SDL2/SDL.h>

struct SpriteComponent
{
    std::string assetId;
    int width;
    int height;
    int zIndex;
    bool isFixed;
    SDL_Rect srcRect;

    SpriteComponent(std::string assetId = "", int width = 1, int height = 1, int zIndex = 0, bool isFixed = false, int srcRectX = 0, int srcRectY = 0)
    {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->isFixed = isFixed;
        this->srcRect = {srcRectX, srcRectY, width, height};
    }
};

#endif
