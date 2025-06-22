#ifndef SPRITECOMPONENT_h
#define SPRITECOMPONENT_h

struct SpriteComponent
{
    int width;
    int height;
    std::string assetId;

    SpriteComponent(std::string assetId = "", int width = 1, int height = 1)
    {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
    }
};

#endif
