#pragma once
#include <vector>
#include "object.h"

class Sphere : public Object
{
private:
    double radius;
    std::vector<float> centerPoint;

public:
    Sphere(double radius, std::vector<float> centerPoint, std::vector<float> color, float shininess);
    ~Sphere(void);
};
