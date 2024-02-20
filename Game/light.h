#pragma once
#include <vector>
#include "../res/includes/glm/glm.hpp"

class Light
{
public:
    glm::vec3 direction;  // Light direction (x, y, z, w)
    glm::vec3 intensity;  // Light intensity (R, G, B, A)
    int flag;
    glm::vec3 position;
    float angleCutOff;

    Light(glm::vec3 direction, glm::vec3 intensity,int flag);
    void spotLight(glm::vec4 spotPar);
    ~Light();
};
