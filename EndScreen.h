#pragma once

#include "FreeType.h"
using namespace freetype;

class EndScreen {
private:
	float red[3] = { 1, 0, 0 };
	float blue[3] = { 0, 0, 1 };
	float green[3] = { 0, 1, 0 };

public:
	EndScreen();

	void render(glm::mat4& projection_matrix);
	void init(string whoWon);
	void update();

	int width;
	int height;

	int screen_width;
	int screen_height;

	bool space;
	string winner;

	Font our_font;
	Font our_font_shadow;
	Font big_font;
	Font big_font_shadow;
};