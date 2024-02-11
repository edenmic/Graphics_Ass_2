#pragma once
#include <vector>
#include "../res/includes/glm/glm.hpp"

class Light
{
private:
    glm::vec3 direction;  // Light direction (x, y, z, w)
    glm::vec4 intensity;  // Light intensity (R, G, B, A)
public:
    Light(glm::vec3 direction, glm::vec4 intensity);
    ~Light();
};
