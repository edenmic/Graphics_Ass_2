#include "object.h"

Object:: Object(glm::vec3 rgb_color, float shininess) {
	this->rgb_color = rgb_color;
	this->shininess = shininess;
	this->reflective = false;
	this->transparent = false;
}
Object:: ~Object(void) {

}

void Object::setReflectiveTrue() {
	this->reflective = true;
}

void Object::setTransparentTrue() {
	this->transparent = true;
}