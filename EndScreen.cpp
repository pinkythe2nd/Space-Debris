
#include "EndScreen.h"

EndScreen::EndScreen() {

}


void EndScreen::init(string whoWon) {
	winner = whoWon.append(" Won!");
	our_font.init("font.ttf", 50, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	our_font_shadow.init("font.ttf", 50, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	big_font.init("font.ttf", 120, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	big_font_shadow.init("font.ttf", 120, glm::vec4(0.8f, 0.0f, 0.0f, 0.5f));
}

void EndScreen::render(glm::mat4& projection_matrix) {
	big_font.print(projection_matrix, big_font, -800, 800, winner.c_str());
	big_font_shadow.print(projection_matrix, big_font_shadow, -810, 800, winner.c_str());
}