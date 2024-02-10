#pragma once
#include <vector>

class Object
{
private:
    std::vector<float> color;       // Object color (ambient and diffuse values: R, G, B, A)
    float shininess;      // Shininess value for specular reflection

public:
    Object(std::vector<float> color, float shininess);
    ~Object(void);
};