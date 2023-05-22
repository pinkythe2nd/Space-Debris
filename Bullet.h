#pragma once

class Shader;

#include "Sprite.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class Bullet: public Sprite {
private:
	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[2];	// ids for Vertex Buffer Objects

	Shader* shader;  ///shader object 

public:
	Bullet();
	Bullet(float angle, float x_vel, float y_vel, float x, float y, Shader* shader, int i);

	void init(float colour[3]);
	void render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);
	void update();
	float width, height;
	int index;
	bool active;
};