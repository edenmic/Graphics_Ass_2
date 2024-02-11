#pragma once
#include <vector>
#include "../res/includes/glm/glm.hpp"
#include "object.h"

class Sphere : public Object
{
private:
    float radius;
    glm::vec3 centerPoint;

public:
    Sphere(float radius, glm::vec3 centerPoint, glm::vec3 rgb_color, float shininess);
    ~Sphere(void);
};
