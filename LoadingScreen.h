#pragma once

#include <iomanip>
#include <sstream>

#include "FreeType.h"
using namespace freetype;


class LoadingScreen {
public:
	LoadingScreen();

	void render(glm::mat4& projection_matrix, float percent);
	void init();

	float time_passed;

	Font our_font;
	Font our_font_shadow;
	Font big_font;
	Font big_font_shadow;
};
