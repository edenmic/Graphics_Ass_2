#include "InputManager.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#include "../DisplayGLFW/display.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "camera.h"
#include "light.h"
#include "object.h"
#include "plane.h"
#include "sphere.h"
#include "spotlight.h"
#include <vector>

std::vector<Light> lights;
std::vector<Object> objects;
glm::vec4 ambientLight;

void readScene(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;

    std::vector<std::vector<float>> objectsTemp; //collect all the details of every object
    std::vector<std::vector<float>> lightsTemp; //collect all the details of every light

    int currLight4Position = 0;
    int currLight4Intensity = 0;

    int currObject = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'e') { // Camera coordinates
            float eye[4];
            iss >> eye[0] >> eye[1] >> eye[2] >> eye[3];
            // option 1 - use MoveCamera function
            // option 2 - store the values and consider every calculation
        }
        else if (type == 'a') { // Ambient light
            iss >> ambientLight.x >> ambientLight.y >> ambientLight.z >> ambientLight.w; //store the ambient values in global variable
        }
        else if (type == 'd') { //lightning - directional or spotlight
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            std::vector<float> light;
            if (cord4 == 0.0) { //directional
                light.push_back(0.0); //represent directional light
                
            }
            else { //spotlight
                light.push_back(1.0);
            }
            light.push_back(cord1); light.push_back(cord2); light.push_back(cord3); //direction values
            lightsTemp.push_back(light);
        }
        else if (type == 'p') { //position of spotlight
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            //position values:
            lightsTemp[currLight4Position].push_back(cord1);
            lightsTemp[currLight4Position].push_back(cord2);
            lightsTemp[currLight4Position].push_back(cord3);
            lightsTemp[currLight4Position].push_back(cord4);
            currLight4Position++;
        }
        else if (type == 'i') { //intensity of lights
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            //intensity values:
            lightsTemp[currLight4Intensity].push_back(cord1);
            lightsTemp[currLight4Intensity].push_back(cord2);
            lightsTemp[currLight4Intensity].push_back(cord3);
            lightsTemp[currLight4Intensity].push_back(cord4);
            currLight4Intensity++;
        }
        else if (type == 'o') { //object
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            std::vector<float> object;
            object.push_back(0.0); //represents regular object (not reflective or tranparant)
            object.push_back(cord1); object.push_back(cord2); object.push_back(cord3); object.push_back(cord4); //values of the object
            objectsTemp.push_back(object);
        }
        else if (type == 'r') {
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            std::vector<float> object;
            object.push_back(1.0); //represents reflective object 
            object.push_back(cord1); object.push_back(cord2); object.push_back(cord3); object.push_back(cord4); //values of the object
            objectsTemp.push_back(object);
        }
        else if (type == 't') {
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            std::vector<float> object;
            object.push_back(2.0); //represents tranparent object 
            object.push_back(cord1); object.push_back(cord2); object.push_back(cord3); object.push_back(cord4); //values of the object
            objectsTemp.push_back(object);
        }
        else if (type == 'c') {
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            objectsTemp[currObject].push_back(cord1); objectsTemp[currObject].push_back(cord2); objectsTemp[currObject].push_back(cord3); //rgb colors
            objectsTemp[currObject].push_back(cord4); //shininess
            currObject++;
        }
    }
    //build lights:
    for (int i = 0; i < lightsTemp.size(); i++) {
        std::vector<float> curr = lightsTemp[i];
        if (curr[0] == 0.0) { //directinal
            glm::vec3 direction;
            direction.x = curr[1]; direction.y = curr[2]; direction.z = curr[3];
            glm::vec4 intensity;
            intensity.x = curr[4]; intensity.y = curr[5]; intensity.z = curr[6]; intensity.w = curr[7];
            Light light = Light(direction, intensity);
            lights.push_back(light);
        }
        else { //spotlight
            glm::vec3 direction;
            direction.x = curr[1]; direction.y = curr[2]; direction.z = curr[3];
            glm::vec4 position;
            position.x = curr[4]; position.y = curr[5]; position.z = curr[6]; position.w = curr[7];
            glm::vec4 intensity;
            intensity.x = curr[8]; intensity.y = curr[9]; intensity.z = curr[10]; intensity.w = curr[11];
            Spotlight light = Spotlight(direction, position, intensity);
            lights.push_back(light);
        }
    }
    //build objects:
    for (int i = 0; i < objectsTemp.size(); i++) {
        std::vector<float> curr = objectsTemp[i];
        if (curr[4] > 0) { //sphere
            glm::vec3 centerPoint;
            centerPoint.x = curr[1]; centerPoint.y = curr[2]; centerPoint.z = curr[3];
            float radius = curr[4];
            glm::vec3 rgb_color;
            rgb_color.x = curr[5]; rgb_color.y = curr[6]; rgb_color.z = curr[7];
            float shininess = curr[8];
            Sphere sphere = Sphere(radius, centerPoint, rgb_color, shininess);
            if (curr[0] == 1.0) { //reflactive
                sphere.setReflectiveTrue();
            }
            else if (curr[0] == 2.0) { //tranparent
                sphere.setTransparentTrue();
            }
            objects.push_back(sphere);
        }
        else { //plane
            glm::vec3 normal;
            normal.x = curr[1]; normal.y = curr[2]; normal.z = curr[3];
            float d = curr[4];
            glm::vec3 rgb_color;
            rgb_color.x = curr[5]; rgb_color.y = curr[6]; rgb_color.z = curr[7];
            float shininess = curr[8];
            Plane plane = Plane(d, normal, rgb_color, shininess);
            if (curr[0] == 1.0) { //reflactive
                plane.setReflectiveTrue();
            }
            else if (curr[0] == 2.0) { //tranparent
                plane.setTransparentTrue();
            }
            objects.push_back(plane);
        }
    }
    file.close();
}

int main(int argc, char* argv[]) {
    const int DISPLAY_WIDTH = 800;
    const int DISPLAY_HEIGHT = 800;
    const float CAMERA_ANGLE = 0.0f;
    const float NEAR = 1.0f;
    const float FAR = 100.0f;

    Game* scn = new Game(CAMERA_ANGLE, (float)DISPLAY_WIDTH / DISPLAY_HEIGHT, NEAR, FAR);
    Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
    Init(display);
    scn->Init();
    display.SetScene(scn);
    //for scene 1:
    readScene("..res\txt scenes\scene1.txt");
    //for scene 2:
  //  readScene("..res\txt scenes\scene2.txt");
    //for scene 3:
  //   readScene("..res\txt scenes\scene3.txt");
    //for scene 4:
 //   readScene("..res\txt scenes\scene4.txt");
    //for scene 5:
  //  readScene("..res\txt scenes\scene5.txt");

    //now the the ambient, objects and lights vectors are set.

    //calculate every pixel value: 
    // ????????????????????????????????????????

    // Main loop
    while (!display.CloseWindow()) {
        scn->Draw(1, 0, scn->BACK, true, false);
        scn->Motion();
        display.SwapBuffers();
        display.PollEvents();
    }

    delete scn;
    return 0;
}