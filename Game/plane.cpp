#include "plane.h"


Plane:: Plane(double d, glm::vec3 normal, glm::vec3 rgb_color, float shininess) : Object(rgb_color, shininess) {
	this->d = d;
	this->normal = normal;
}

Plane:: ~Plane(void) {

}

glm::vec3 Plane::getColor(glm::vec3 hitPoint) {
	float scale_parameter = 0.5f;
	float chessboard = 0;
	if (hitPoint.x < 0) {
		chessboard += floor((0.5 - hitPoint.x) / scale_parameter);
	}
	else{
		chessboard += floor(hitPoint.x / scale_parameter);
	}
	if(hitPoint.y < 0) {
			chessboard += floor((0.5 - hitPoint.y) / scale_parameter);
	}
	else{
			chessboard += floor(hitPoint.y / scale_parameter);
	}
	chessboard = (chessboard * 0.5) - int(chessboard * 0.5);
	chessboard *= 2;
	if (chessboard > 0.5) {
		return 0.5f * this->rgb_color;
	}
	return this->rgb_color;

}