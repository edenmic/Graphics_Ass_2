#include "light.h"

Light::Light(glm::vec3 direction, glm::vec3 intensity, int flag) {
	this->direction = glm::normalize(direction);
	this->intensity = intensity;
	this->flag = flag;
}
void Light::spotLight(glm::vec4 spotPar) {
	this->position = glm::vec3(spotPar);
	this->angleCutOff = spotPar.w;
}
Light::~Light() {

}