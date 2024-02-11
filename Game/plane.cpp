#include "plane.h"


Plane:: Plane(double d, glm::vec3 normal, glm::vec3 color, float shininess) : Object(color, shininess) {
	this->d = d;
	this->normal = normal;
}

Plane:: ~Plane(void) {

}