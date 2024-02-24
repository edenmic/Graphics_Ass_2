#pragma once
#include "../res/includes/glm/glm.hpp"
#include <vector>

class Object
{
public:
    glm::vec3 rgb_color;     
    float shininess;    
    bool reflective;
    bool transparent;
    int flag;


    //Object(glm::vec3 rgb_color, float shininess);
    //~Object(void);
    virtual glm::vec3 getNormal(glm::vec3 hitPoint)=0;

    virtual void setReflectiveTrue() = 0;
    virtual void setTransparentTrue() = 0;
    virtual double findIntersect(glm::vec3 ray, glm::vec3 src) = 0;

    virtual glm::vec3 getColor(glm::vec3 hitPoint) = 0;
};
