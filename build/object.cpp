#include "object.h"

Object:: Object(std::vector<float> color, float shininess) {
	this->color = color;
	this->shininess = shininess;
}
Object:: ~Object(void) {

}