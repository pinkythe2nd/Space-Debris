
#include "LoadingScreen.h"

LoadingScreen::LoadingScreen() {
}


void LoadingScreen::init() {
	our_font.init("font.ttf", 50, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	our_font_shadow.init("font.ttf", 50, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	big_font.init("font.ttf", 120, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	big_font_shadow.init("font.ttf", 120, glm::vec4(0.8f, 0.0f, 0.0f, 0.5f));

	time_passed = 0.0f;
}

void LoadingScreen::render(glm::mat4& projection_matrix, float percent) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << percent;
	big_font.print(projection_matrix, big_font, -900, 800, ("Placing Asteroids " + stream.str()).c_str(), big_font.color);
	big_font_shadow.print(projection_matrix, big_font_shadow, -910, 800, ("Placing Asteroids " + stream.str()).c_str(), big_font.color);
	big_font.color = glm::vec4(sinf(time_passed) * 0.5f + 0.5f, cosf(time_passed) * 0.5f + 0.5f, sinf(time_passed) * 0.3f + 0.5f, 1.0f);
	time_passed += 0.05f;

	our_font.print(projection_matrix, our_font, -500, 400, "Controls are as followed:");
	our_font.print(projection_matrix, our_font, -800, 300, "W=Forward,  A=Left,  S=Backwards,  D=Right, ");
	our_font.print(projection_matrix, our_font, -500, 200, "Z=Toggle inertia dampners, ");
	our_font.print(projection_matrix, our_font, -700, 100, "Shift=Lock heading angle, Space=Fire!");

	our_font_shadow.print(projection_matrix, our_font_shadow, -502, 400, "Controls are as followed:");
	our_font_shadow.print(projection_matrix, our_font_shadow, -802, 300, "W=Forward,  A=Left,  S=Backwards,  D=Right, ");
	our_font_shadow.print(projection_matrix, our_font_shadow, -502, 200, "Z=Toggle inertia dampners, ");
	our_font_shadow.print(projection_matrix, our_font_shadow, -702, 100, "Shift=Lock heading angle, Space=Fire!");
}