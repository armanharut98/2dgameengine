#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>

struct BoxColliderComponent
{
    int width;
    int height;
    glm::vec2 offset;

    BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0)) // glm allows us to pass only one 0 instead of 0.0, 0.0
    {
        this->width = width;
        this->height = height;
        this->offset = offset;
    }
};

#endif
