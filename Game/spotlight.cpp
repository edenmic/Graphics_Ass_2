#include "spotlight.h"


Spotlight:: Spotlight(glm::vec3 direction, glm::vec4 position, glm::vec4 intensity) : Light(direction, intensity) {
	this->position = position;
}

Spotlight:: ~Spotlight() {

}