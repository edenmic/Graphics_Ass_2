#pragma once
#include <vector>
#include "../res/includes/glm/glm.hpp"
#include "object.h"

class Plane : public Object
{
private:
    double d;
    glm::vec3 normal;

public:
    Plane(double d, glm::vec3 normal, glm::vec3 rgb_color, float shininess);
    ~Plane(void);
    glm::vec3 getColor(glm::vec3 hitPoint);
    glm::vec3 Plane::getNormal(glm::vec3 hitPoint);
    double findIntersect(glm::vec3 ray, glm::vec3 src);
    void setReflectiveTrue() {
        this->reflective = true;
    }
    void setTransparentTrue() {
        this->transparent = true;
    }
};