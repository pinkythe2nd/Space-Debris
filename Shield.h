#pragma once

class Shader;

#include "Sprite.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

using namespace std;

class Shield : public Sprite {
private:
	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[2];	// ids for Vertex Buffer Objects

	int m_NumOfVerts;

	Shader* shader;  ///shader object 

public:
	Shield();
	Shield(float angle, float x, float y, float radius, float num_segments);

	void init(float colour[3]);
	void render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);
	int inCircle(Sprite& sprite);

	int shield_health;
	float radius, num_segments, cx, cy;

};