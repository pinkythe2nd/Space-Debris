//--- OpenGL ---
#include "GL\glew.h"
#include "GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------
#include "GL\freeglut.h"
#include "FreeImage.h"

// ---- Sprite Code -----//
#include "Globals.h"
#include "MainStage.h"
#include "StartScreen.h"
#include "EndScreen.h"
#include "LoadingScreen.h"
// ------- global variables -------- //

glm::mat4 game_projection_matrix;
glm::mat4 screen_projection_matrix;
// ----- function declarations ------ //

void init();
void reshape(int width, int height);
void display();
void keyboard_func(unsigned char key, int x, int y);
void set_bullet_fire_true(int time);

bool startScreenBool = true;
bool inGame = false;
bool endGame = false;

int start = 0;
int endi = 100;


MainStage mainstage;

StartScreen startScreen;
EndScreen endScreen;
LoadingScreen loadingScreen;

// ----- function definitions ------ //

void reshape(int width, int height)		// Resize the OpenGL window
{
	screen_width = width;
	screen_height = height;

	glViewport(0, 0, width, height);						// set Viewport dimensions

	std::cout << width << " h " << height << endl;

	screen_projection_matrix = glm::ortho(-game_width * 1.77777778f, game_width * 1.77777778f, -game_height, game_height);
	game_projection_matrix = glm::ortho(-game_width, game_width, -game_height, game_height);
}

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}

void init(){
	FreeImage_Initialise();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	startScreen.init();
	mainstage.init();
	loadingScreen.init();
	glutTimerFunc(500, set_bullet_fire_true, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	timer(0);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (startScreenBool) {
		startScreen.render(screen_projection_matrix);
	}
	if (loading) {
		loadingScreen.render(screen_projection_matrix, load_percent);
		mainstage.load_asteroids(start, endi);
		start += 100;
		endi += 100;
		if (endi > mainstage.a - 1) {
			endi = mainstage.a % endi;
			mainstage.load_asteroids(start, endi);
			loadingScreen.render(screen_projection_matrix, load_percent);
			loaded = true;
			mainstage.placeAsteroids();
			loading = false;
		}
	}

	if (loaded) {
		if (ai.loops == 3 || ai2.loops == 3) {
			endScreen.init("AI");
			endGame = true;
			loaded = false;
		}if (player.loops == 3) {
			endScreen.init("Player");
			endGame = true;
			loaded = false;
		}
		mainstage.render(game_projection_matrix);
	}

	if (endGame) {
		endScreen.render(screen_projection_matrix);
	}

	glutSwapBuffers();
}


void set_bullet_fire_true(int time) {
	player.bullet_fire = true;
	glutTimerFunc(250, set_bullet_fire_true, 0);
}



void keyboard_func(unsigned char key, int x, int y) {
	switch (key) {
	case 'z':
		if (player.z_key) {
			player.z_key = false;
			break;
		}
		else {
			player.z_key = true;
			break;
		}
	case 'x':
		if (player.x_key) {
			player.x_key = false;
			break;
		}
		else {
			player.x_key = true;
			break;
		}
	case 'w':
		player.up = true;
		break;
	case 'a':
		player.left = true;
		player.shift_right = false;
		break;
	case 's':
		player.down = true;
		break;
	case 'd':
		player.right = true;
		player.shift_left = false;
		break;
	case ' ':
		if (startScreenBool) {
			startScreenBool = false;
			loading = true;
		}
		player.space = true;
		break;
	case 'W':
		player.up = true;
		break;
	case 'S':
		player.down = true;
		break;
	case 'A':
		player.shift_right = true;
		player.right = false;
		break;
	case 'D':
		player.shift_left = true;
		player.left = false;
		break;
	}
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		player.left = false;
		player.right = false;
	}
}

void keyboard_func_up(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		player.up = false;
		break;
	case 'a':
		player.left = false;
		break;
	case 's':
		player.down = false;
		break;
	case 'd':
		player.right = false;
		break;
	case ' ':
		player.space = false;
		break;
	case 'W':
		player.up = false;
		break;
	case 'S':
		player.down = false;
		break;
	case 'A':
		player.shift_right = false;
		break;
	case 'D':
		player.shift_left = false;
		break;
	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Space Game!");

	glutReshapeFunc(reshape);

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << " GLEW ERROR" << endl;
		return -1;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	std::cout << "OpenGL Version: " << OpenGLVersion[0] << "." << OpenGLVersion[1] << endl;

	init();
	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(keyboard_func_up);

	glutMainLoop();

	return 0;
}
