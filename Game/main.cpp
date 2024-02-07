#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"


/*glm::vec3 showCheckerboard(glm::vec3 hitPoint)
{
	float scale_parameter = 0.5f;
	float chessboard = 0;
	if (hitPoint.x < 0) {
		chessboard += floor((0.5 – hitPoint.x) / scale_parameter);
	}
	else {
		chessboard += floor(hitPoint.x / scale_parameter);
	}
	if (hitPoint.y < 0) {
		chessboard += floor((0.5 - hitPoint.y) / scale_parameter);
	}
	else {
		chessboard += floor(hitPoint.y / scale_parameter);
	}
	chessboard = (chessboard * 0.5) - int(chessboard * 0.5);
	chessboard *= 2;
	if (chessboard > 0.5) {
		return 0.5f * this->rgbColors;
	}
	return this->rgb_color;
}*/

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 800;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;
	//const glm::vec3 hitPoint = (0,0,-1);

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display); //blah blah
	
	scn->Init();
	//showCheckerboard(hitPoint);
	display.SetScene(scn);
	int width = 256;
	int height = 256;



	//we added to display a black screen

	unsigned char* newData = (unsigned char*)malloc(width * height * 4);
	for (int y = 0; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			int pixelIndex = (y * width + x) * 4;
			newData[pixelIndex] = 0;
			newData[pixelIndex + 1] = 0;
			newData[pixelIndex + 2] = 0;

		}
	}
	scn->AddTexture(width, height, newData);
	//end of what we addwd
	while(!display.CloseWindow())
	{
		scn->Draw(1,0,scn->BACK,true,false);
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();	
			
	}
	delete scn;
	return 0;
}
