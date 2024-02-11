#include "object.h"

Object:: Object(glm::vec3 color, float shininess) {
	this->color = color;
	this->shininess = shininess;
}
Object:: ~Object(void) {

}