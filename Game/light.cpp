#include "light.h"

Light::Light(glm::vec3 direction, glm::vec4 intensity) {
	this->direction = direction;
	this->intensity = intensity;
}

Light::~Light() {

}