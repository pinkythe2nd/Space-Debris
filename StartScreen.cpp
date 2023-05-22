#include "StartScreen.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <GL/freeglut_std.h>

StartScreen::StartScreen() {
}


void StartScreen::init() {
	our_font.init("font.ttf", 50, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	our_font_shadow.init("font.ttf", 50, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	big_font.init("font.ttf", 120, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	big_font_shadow.init("font.ttf", 120, glm::vec4(0.8f, 0.0f, 0.0f, 0.5f));


}

void StartScreen::update() {
}


void StartScreen::render(glm::mat4& projection_matrix) {
	update();
	big_font.print(projection_matrix, big_font, -800, 800, "S p a c e  D e b r i s");
	big_font.print(projection_matrix, big_font_shadow, -810, 800, "S p a c e  D e b r i s");

	our_font.print(projection_matrix, our_font, -500, 400, "Controls are as followed:");
	our_font.print(projection_matrix, our_font, -800, 300, "W=Forward,  A=Left,  S=Backwards,  D=Right, ");
	our_font.print(projection_matrix, our_font, -500, 200, "Z=Toggle inertia dampners, ");
	our_font.print(projection_matrix, our_font, -700, 100, "Shift=Lock heading angle, Space=Fire!");

	our_font.print(projection_matrix, our_font, -400, -200, "Press Space to Start!!!");

	our_font_shadow.print(projection_matrix, our_font_shadow, -502, 400, "Controls are as followed:");
	our_font_shadow.print(projection_matrix, our_font_shadow, -802, 300, "W=Forward,  A=Left,  S=Backwards,  D=Right, ");
	our_font_shadow.print(projection_matrix, our_font_shadow, -502, 200, "Z=Toggle inertia dampners, ");
	our_font_shadow.print(projection_matrix, our_font_shadow, -702, 100, "Shift=Lock heading angle, Space=Fire!");

	our_font_shadow.print(projection_matrix, our_font_shadow, -402, -200, "Press Space to Start!!!");

}