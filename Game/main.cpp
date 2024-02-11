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

std::vector<Light> lights;
std::vector<Object> objects;
Camera camera;

void readScene(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char type;
        iss >> type;

        //for objects:
        glm::vec3 colors;
        float shininess;
        glm::vec3 normal; //for plane
        float d; //for plane
        glm::vec3 centerPoint; //for sphere
        float radius; //for sphere

        //for lights:
        glm::vec4 direction;
        glm::vec4 position;
        glm::vec4 intensity;

        if (type == 'e') {
            // Camera coordinates
            float eye[4];
            iss >> eye[0] >> eye[1] >> eye[2] >> eye[3];
            camera = Camera(eye[0], eye[1], eye[2], eye[3]); //Camera(float fov, float relathionWH, float zNear, float zFar);
        }
        else if (type == 'a') {
            // Ambient light
            float ambientLight[4];
            iss >> ambientLight[0] >> ambientLight[1] >> ambientLight[2] >> ambientLight[3];
            //  to set AmbientLight
        }
        else if (type == 'd' || type == 'p') {
            // Handle directional or spotlight
            iss >> direction.x >> direction.y >> direction.z >> direction.w;
            if (type == 'p') {
                // Handle spotlight position
                iss >> position.x >> position.y >> position.z >> position.w;
            }
            iss >> intensity.x >> intensity.y >> intensity.z >> intensity.w;
            lights.push_back(Light(type, direction, position, intensity));
        }
        else if (type == 'o') {
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            if (cord4 > 0) { //sphere
                centerPoint.x = cord1;
                centerPoint.y = cord2;
                centerPoint.z = cord3;
                radius = cord4;
                objects.push_back(Sphere(radius, centerPoint, colors, shininess));
            }
            else { //plane
                normal.x = cord1;
                normal.y = cord2;
                normal.z = cord3;
                d = cord4;
                objects.push_back(Plane(d, normal, colors, shininess));
            }
        }
        else if (type == 'r') {
            //is reflective
        }
        else if (type == 't') {
            // trasparent
        }
        else if (type == 'c') { 
            float cord1, cord2, cord3, cord4;
            iss >> cord1 >> cord2 >> cord3 >> cord4;
            colors.x = cord1;
            colors.y = cord2;
            colors.z = cord3;
            shininess = cord4;
            objects.push_back(Object(colors, shininess)); // TODO:  add it to the compatible existing object and not push new one
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

    // Creating the chessboard pattern
    const int boardSize = 8; // Size of the chessboard (8x8)
    const int squareSize = DISPLAY_WIDTH / boardSize; // Size of each square

    unsigned char* newData = (unsigned char*)malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * 4);

    // Loop through each pixel and assign color based on the position on the chessboard
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int squareX = x / squareSize;
            int squareY = y / squareSize;
            bool isWhite = (squareX + squareY) % 2 == 0; // Check if the square is white or black
            int pixelIndex = (y * DISPLAY_WIDTH + x) * 4;
            newData[pixelIndex] = isWhite ? 255 : 0; // Red component
            newData[pixelIndex + 1] = isWhite ? 255 : 0; // Green component
            newData[pixelIndex + 2] = isWhite ? 255 : 0; // Blue component
            newData[pixelIndex + 3] = 255; // Alpha component (fully opaque)
        }
    }

    scn->AddTexture(DISPLAY_WIDTH, DISPLAY_HEIGHT, newData);

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