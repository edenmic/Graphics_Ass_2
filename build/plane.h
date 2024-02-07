#pragma once
#include <vector>
#include "object.h"

class Plane : public Object
{
private:
    double d;
    std::vector<float> normal;

public:
    Plane(double d, std::vector<float> normal, std::vector<float> color, float shininess);
    ~Plane(void);
};