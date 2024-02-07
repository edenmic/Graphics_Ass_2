#include "plane.h"


Plane:: Plane(double d, std::vector<float> normal, std::vector<float> color, float shininess) : Object(color, shininess) {
	this->d = d;
	this->normal = normal;
}

Plane:: ~Plane(void) {

}