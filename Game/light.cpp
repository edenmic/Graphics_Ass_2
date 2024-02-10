#include "light.h"

Light::Light(char type, std::vector<float> direction, std::vector<float> position, std::vector<float> intensity) {
	this->type = type;
	this->direction = direction;
	this->intensity = intensity;
	this->position = position;
}
Light::Light(char type, std::vector<float> direction) {
	this->type = type;
	this->direction = direction;
}