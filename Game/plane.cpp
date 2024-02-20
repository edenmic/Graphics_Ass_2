#include "plane.h"


Plane:: Plane(double d, glm::vec3 normal, glm::vec3 rgb_color, float shininess){
	this->d = d;
	this->normal = glm::normalize(normal);
	this->rgb_color = rgb_color;
	this->shininess = shininess;
	this->flag = 0;

}

Plane:: ~Plane(void) {

}
glm::vec3 Plane::getNormal(glm::vec3 hitPoint) {
	return this->normal;
}

/*double Plane::findIntersect(glm::vec3 ray, glm::vec3 src) {
	return -1;
}*/
double Plane::findIntersect(glm::vec3 ray, glm::vec3 src) {
	double multi = 1;
	if (normal.x != 0)
		multi *= normal.x;
	if (normal.y != 0)
		multi *= normal.y;
	if (normal.z != 0)
		multi *= normal.z;

	glm::vec3 q0 = glm::vec3(d / multi);//dot on the plane
    // Calculate the distance along the ray to the intersection point
	//double t = glm::dot(normal,(q0-src)) / glm::dot(normal,ray);
    double numerator = -(glm::dot(src, normal)+d);//N*(Q0-P0)/N*V
    double denominator = glm::dot(ray, normal);
    
    // Check if the ray is parallel or pointing away from the plane
    if (denominator == 0 ) {
        return -1; // No intersection
    }
    
    // Calculate the intersection point
    double t = numerator / denominator;
    
    // Check if the intersection point is behind the ray's source
    if (t < 0) {
        return -1; // No intersection
    }
    
    return t; // Return the distance to the intersection point
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