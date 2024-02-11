#include "sphere.h"


Sphere:: Sphere(float radius, glm::vec3 centerPoint, glm::vec3 color, float shininess) : Object(color, shininess) {
	this->radius = radius;
	this->centerPoint = centerPoint;
}

Sphere:: ~Sphere(void) {

}