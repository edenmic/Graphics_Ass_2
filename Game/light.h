#pragma once
#include <vector>

class Light
{
public:
    char type;  // 'd' for directional light, 'p' for spotlight
    std::vector<float> direction;  // Light direction (x, y, z, w)
    std::vector<float> position;   // Light position (x, y, z, w) for spotlights
    std::vector<float> intensity;  // Light intensity (R, G, B, A)
    Light(char type, std::vector<float> direction, std::vector<float> position, std::vector<float> intensity);
    Light(char type, std::vector<float> direction);
};