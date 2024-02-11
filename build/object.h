#pragma once
#ifndef OBJECT_H
#include "../res/includes/glm/glm.hpp"
#include <vector>

class Object
{
private:
    glm::vec3 color;       // Object color (ambient and diffuse values: R, G, B, A)
    float shininess;      // Shininess value for specular reflection

public:
    Object(glm::vec3 color, float shininess);
    ~Object(void);
};
#endif