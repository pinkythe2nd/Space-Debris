#include "RaceTrack.h"
#include "shaders\Shader.h"

#include <random>
#include <iostream>


RaceTrack::RaceTrack() {
	angle = 0.0f;
	x = 0.0f;
	y = 0.0f;
	sides = 0;
	radius = 0;
}
RaceTrack::RaceTrack(float xp, float yp, float ang, float radi) {
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;

	angle = ang;
	x = xp;
	y = yp;
	sides = 300;
	radius = radi;
}

float get_random_two(float high) {
	using engine = std::mt19937;
	std::random_device os_seed;
	const uint_least32_t seed = os_seed();
	engine generator(seed);

	std::uniform_real_distribution<> dis(-high, high); // range 0 - 1
	return dis(generator);
}

void RaceTrack::init() {
	float colour[3] = { 1, 0, 0 };
	float* vert2 = new float[sides * 9];
	float* col = new float[sides * 9];	// colour array
	float* col2 = new float[sides * 9];	// colour array
	float temp_x;
	float temp_y;

	for (int i = 0; i < sides; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(sides);//get the current 
		temp_x = radius * cosf(theta);
		temp_y = radius * sinf(theta);

		temp_x += x;

		waypoints.push_back(glm::vec4(temp_x, temp_y, 0, 0));
		bumpy_waypoints.push_back(glm::vec4(temp_x, temp_y, 0, 0));

		temp_x += get_random_two(sides * 2) + 100;//calculate the x component
		temp_y += get_random_two(sides) + 100;//calculate the y component


		vert.push_back(glm::vec4(temp_x, temp_y, 0.0f, 0.0f));
		col[i * 3] = colour[0]; col[i * 3 + 1] = colour[1]; col[i * 3 + 2] = colour[2];
	}

	radius += 200;
	for (int i = 0; i < sides; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(sides);//get the current 
		temp_x = radius * cosf(theta);
		temp_x += x;
		temp_y = radius * sinf(theta);
		bumpy_waypoints2.push_back(glm::vec4(temp_x, temp_y, 0, 0));
	}

}