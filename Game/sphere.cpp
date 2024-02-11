#include "sphere.h"


Sphere:: Sphere(float radius, glm::vec3 centerPoint, glm::vec3 rgb_color, float shininess) : Object(rgb_color, shininess) {
	this->radius = radius;
	this->centerPoint = centerPoint;
}

Sphere:: ~Sphere(void) {

}