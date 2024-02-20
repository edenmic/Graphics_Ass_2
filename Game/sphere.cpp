#include "sphere.h"


Sphere:: Sphere(float radius, glm::vec3 centerPoint, glm::vec3 rgb_color, float shininess) {
	this->radius = radius;
	this->centerPoint = centerPoint;
	this->rgb_color = rgb_color;
	this->shininess = shininess;
	this->flag = 1;

}

Sphere:: ~Sphere(void) {

}
glm::vec3 Sphere::getNormal(glm::vec3 hitPoint) {
	return glm::normalize(hitPoint - this->centerPoint);
}
double Sphere::findIntersect(glm::vec3 ray, glm::vec3 src) {
		//t is the return value- if t is negative there is no intersect, else- return the t value of the intersect
	double a = 1;
	double b = 2 * glm::dot(ray, src - this->centerPoint);
	double c = glm::dot(src - this->centerPoint, src - this->centerPoint) - this->radius * this->radius;
	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		return -1; //there is no intersect
	}
	else if (discriminant == 0)
	{
		return (-b / (2 * a));
	}
	else {
		double t1 = ((-b + sqrt(discriminant)) / (2 * a));
		double t2 = ((-b - sqrt(discriminant)) / (2 * a));
		return std::min(t1, t2); //add later if we calc the inner intersect return max
	}
}