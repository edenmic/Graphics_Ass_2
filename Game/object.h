#pragma once
#include "../res/includes/glm/glm.hpp"
#include <vector>

class Object
{
protected:
    glm::vec3 rgb_color;     
    float shininess;    
    bool reflective;
    bool transparent;

public:
    Object(glm::vec3 rgb_color, float shininess);
    ~Object(void);
    void setReflectiveTrue();
    void setTransparentTrue();
};
