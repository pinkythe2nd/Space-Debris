#pragma once

class Shader;

#include "Sprite.h"
#include "Bullet.h"


class Asteroid : public Sprite {
private:
	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[2];	// ids for Vertex Buffer Objects

	Shader* shader;  ///shader object 

public:
	Asteroid();
	Asteroid(float x, float y, float angle, float radius, int index, bool active, bool random_bool);

	void init(float colour[3], Shader* shader);
	void update();
	void render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);
	virtual int inCircle(Asteroid& sprite);

	float radius;
	bool random_bool, dead;
	int sides, max_health, health;
};