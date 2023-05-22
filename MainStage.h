#pragma once

#include "shaders\Shader.h"

#include "Globals.h"
#include "poisson_disk_sampling.h"
#include "Background.h"

#include <random>
#include <array>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "FreeType.h"
using namespace freetype;

class MainStage{
private:
	Shader* asteroidShader;
public:
	MainStage();
	void init();

	void process_keys();
	void collision();
	void update_sprites();
	void placeAsteroids();
	void load_asteroids(int start, int end);
	void render_target();


	void render(glm::mat4& projection_matrix);

	Background background;
	vector<array<float, 2>> dave;
	int a;
	float percent;
	glm::mat4 view_matrix;

	Font our_font;
	Font our_font_shadow;
};