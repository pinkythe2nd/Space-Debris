#pragma once

#include "FreeType.h"
using namespace freetype;

class StartScreen{
private:
	float red[3] = { 1, 0, 0 };
	float blue[3] = { 0, 0, 1 };
	float green[3] = { 0, 1, 0 };

public:
	StartScreen();

	void render(glm::mat4& projection_matrix);
	void init();
	void update();

	Font our_font;
	Font our_font_shadow;
	Font big_font;
	Font big_font_shadow;
	Font pur_font;
};