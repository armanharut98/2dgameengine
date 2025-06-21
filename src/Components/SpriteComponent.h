#ifndef SPRITECOMPONENT_h
#define SPRITECOMPONENT_h

struct SpriteComponent
{
    int width;
    int height;

    SpriteComponent(int witdth = 1, int height = 1)
    {
        this->width = width;
        this->height = height;
    }
};

#endif
