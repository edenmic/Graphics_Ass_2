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
#include <vector>
using namespace glm;

std::vector<Light> lights;
std::vector<Object*> objects;
glm::vec3 ambientLight;
glm::vec3 camera;

void readScene(const std::string filename) {
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

        if (type == 'e') { // Camera coordinates - add to global
            float eye[4];
            iss >> eye[0] >> eye[1] >> eye[2] >> eye[3];
            camera.x = eye[0]; camera.y = eye[1]; camera.z = eye[2];
            // option 1 - use MoveCamera function
            // option 2 - store the values and consider every calculation
        }
        else if (type == 'a') { // Ambient light
            iss >> ambientLight.x >> ambientLight.y >> ambientLight.z; //store the ambient values in global variable
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
            glm::vec3 intensity;
            intensity.x = curr[4]; intensity.y = curr[5]; intensity.z = curr[6];
            Light light = Light(direction, intensity, (int)curr[0]);
            lights.push_back(light);
        }
        else { //spotlight
            glm::vec3 direction;
            direction.x = curr[1]; direction.y = curr[2]; direction.z = curr[3];
            glm::vec4 position;
            position.x = curr[4]; position.y = curr[5]; position.z = curr[6]; position.w = curr[7];
            glm::vec3 intensity;
            intensity.x = curr[8]; intensity.y = curr[9]; intensity.z = curr[10];
            Light light = Light(direction, intensity, (int)curr[0]);
            light.spotLight(position);
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
            Object* sphere = new Sphere(radius, centerPoint, rgb_color, shininess);
            if (curr[0] == 1.0) { //reflactive
                sphere->setReflectiveTrue();
            }
            else if (curr[0] == 2.0) { //tranparent
                sphere->setTransparentTrue();
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
            Object* plane = new Plane(d, normal, rgb_color, shininess);
            if (curr[0] == 1.0) { //reflactive
                plane->setReflectiveTrue();
            }
            else if (curr[0] == 2.0) { //tranparent
                plane->setTransparentTrue();
            }
            objects.push_back(plane);
        }
    }
    file.close();
}

glm::vec3 calculate3Dcord(int i, int j, double lengthPixel) {
    glm::vec3 startingPoint = { -1,1,0 };
    glm::vec3 currPoint;
    currPoint.x = startingPoint[0] + lengthPixel / 2 + lengthPixel * i;
    currPoint.y = startingPoint[1] - lengthPixel / 2 - lengthPixel * j;
    currPoint.z = startingPoint[2];
    return currPoint;

}
glm::vec3 diffuseRef(Object* o, Light light, vec3 hitPoint) {
    vec3 n = o->getNormal(hitPoint);//Normal hit point
    vec3 diffColor = vec3(0);

    glm::vec3 objectColor;
    if (o->flag == 0) { //is plane
        objectColor = o->getColor(hitPoint);
    }
    else { //is sphere
        objectColor = o->rgb_color;
    }

    if (light.flag == 0) {
        vec3 l = normalize( - light.direction);
        diffColor += objectColor * dot(n, l) * light.intensity;
    }
    else {
        vec3 l = normalize(hitPoint - light.position);
        float cosAngle = dot(l, normalize(light.direction));
        if (cosAngle > light.angleCutOff) {
            diffColor += objectColor * dot(n, -l) * light.intensity;
        }
    }

    //diffColor = glm::min(glm::max(diffColor, 0.0f), 1.0f);
    return diffColor;
}

glm::vec3 specularRef(Object* o, Light light, vec3 hitPoint, vec3 currPoint) {
    vec3 n = o->getNormal(hitPoint);  // Normal at the hit point
    vec3 v = normalize(currPoint - hitPoint);  // View direction

    vec3 specularColor = vec3(0);

    if (light.flag == 0) { //directional
        vec3 l = normalize(light.direction);
        vec3 r = normalize(reflect(l, n));  // Reflected light direction
        float specularTerm = pow(glm::max(dot(r, v), 0.0f), o->shininess);
        specularColor += light.intensity * vec3(0.7, 0.7, 0.7) * specularTerm;
    }
    else {
        vec3 l = normalize(hitPoint - light.position);
        vec3 r = normalize(reflect(l, n));  // Reflected light direction
        float specularTerm = pow(glm::max(dot(r, v), 0.0f), o->shininess);
        float cosAngle = dot(l, light.direction);
        if (cosAngle > light.angleCutOff) {
            specularColor += light.intensity * vec3(0.7, 0.7, 0.7) * specularTerm;
        }
    }

    return specularColor;
}

glm::vec3 constructRayThroughPixel(glm::vec3 currPoint, glm::vec3 ray, int level) {
    if (level > 5) {
        return vec3(0);
    }

    double closestT = std::numeric_limits<double>::infinity();
    Object* closestObject = nullptr;
    glm::vec3 color;
    // Find closest intersection with objects
    for (Object* obj : objects) {
        double t = obj->findIntersect(ray, currPoint);
        if (t > 0 && t < closestT) {
            closestT = t;
            closestObject = obj;
        }
    }

    // If intersection found, calculate pixel color
    if (closestObject != nullptr) {
        if (!closestObject->reflective && !closestObject->transparent) {

            //  color = closestObject->rgb_color * ambientLight;


            glm::vec3 hitPoint = currPoint + (float)closestT * ray;


            //if its plane show chessboard: 
            if (closestObject->flag == 0) { //is plane
                glm::vec3 planeColor = closestObject->getColor(hitPoint);
                color = planeColor * ambientLight;
            }
            else { //its not plane, usual color
                color = closestObject->rgb_color * ambientLight;
            }


            for (Light li : lights) {

                // for shadowing:
                glm::vec3 shadowRayDir;
                glm::vec3 shadowRayDirNot;
                bool isInShadow = false;
                if (li.flag == 0) {
                    shadowRayDir = normalize(-li.direction); // Directional light
                }
                else {
                    shadowRayDir = normalize(li.position - hitPoint); // Spotlight
                    shadowRayDirNot = li.position - hitPoint;

                }
                // Offset the shadow ray origin slightly to avoid self-shadowing artifacts
                glm::vec3 shadowRayOrigin = hitPoint;
                double minShadowT = std::numeric_limits<double>::infinity();
                // Check for intersections with objects in the scene
                for (Object* obj : objects) {
                    double shadowT = obj->findIntersect(shadowRayDir, shadowRayOrigin);
                    // If there is an intersection along the shadow ray, the hit point is in shadow
                    if (shadowT > 0 && shadowT < std::numeric_limits<double>::infinity()) {
                        minShadowT = min(shadowT, minShadowT);

                    }
                }
                if (minShadowT < std::numeric_limits<double>::infinity()) {
                    if (li.flag == 0) {
                        isInShadow = true;
                    }
                    else {
                        if (glm::dot(shadowRayOrigin + ((float)minShadowT * shadowRayDir), shadowRayDirNot) < 0) {
                            isInShadow = true;
                        }
                    }
                }
                if (!isInShadow) {
                    // Calculate diffuse
                    vec3 diff = diffuseRef(closestObject, li, hitPoint);
                    diff = glm::min(glm::max(diff, 0.f), 1.f);
                    color += diff;
                    // Calculate specular reflection
                    glm::vec3 specularColor = specularRef(closestObject, li, hitPoint, currPoint);
                    color += specularColor;
                }
            }
            color = glm::min(glm::max(color, 0.f), 1.f);
        }
        else if (closestObject->reflective) {
            glm::vec3 hitPoint = currPoint + (float)closestT * ray;
            vec3 newRay = normalize(reflect(ray, closestObject->getNormal(hitPoint)));
            color = constructRayThroughPixel(hitPoint, newRay, level++);
        }
    }
    else {
        color = vec3(0);
    }
    return color;
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
    readScene("../res/txt_scenes/scene1.txt");
    //for scene 2:
 //   readScene("../res/txt_scenes/scene2.txt");
    //for scene 3:
  //   readScene("../res/txt_scenes/scene3.txt");
    //for scene 4:
   // readScene("../res/txt_scenes/scene4.txt");
    //for scene 5:
  //  readScene("../res/txt_scenes/scene5.txt");

    //now the the ambient, objects and lights vectors are set.

    unsigned char* data = (unsigned char*)malloc(DISPLAY_HEIGHT * DISPLAY_WIDTH * 4);
    double lengthPixel = (double)2 / (double)800;
    for (int i = 0; i < DISPLAY_HEIGHT; i++) {
        for (int j = 0; j < DISPLAY_WIDTH; j++) {
            glm::vec3 currPoint = calculate3Dcord(i, j, lengthPixel);
            glm::vec3 ray = glm::normalize(currPoint - camera);

            glm::vec3 color = constructRayThroughPixel(camera, ray, 0);

            data[(i + j * DISPLAY_WIDTH) * 4] = color.r * 255;
            data[(i + j * DISPLAY_WIDTH) * 4 + 1] = color.g * 255;
            data[(i + j * DISPLAY_WIDTH) * 4 + 2] = color.b * 255;
        }

    }

    scn->AddTexture(DISPLAY_WIDTH, DISPLAY_HEIGHT, data);
    scn->SetShapeTex(0, 0);

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