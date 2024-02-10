#include "sphere.h"


Sphere:: Sphere(double radius, std::vector<float> centerPoint, std::vector<float> color, float shininess) : Object(color, shininess) {
	this->radius = radius;
	this->centerPoint = centerPoint;
}

Sphere:: ~Sphere(void) {

}