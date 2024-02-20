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
    double findIntersect(glm::vec3 ray, glm::vec3 src);
    glm::vec3 Sphere::getNormal(glm::vec3 hitPoint);
    void setReflectiveTrue() {
        this->reflective = true;
    }
    void setTransparentTrue() {
        this->transparent = true;
    }
};
