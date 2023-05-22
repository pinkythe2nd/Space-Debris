#pragma once

class Shader;

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "Sprite.h"
#include "Bullet.h"

class Ai : public Sprite{
private:
	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[2];	// ids for Vertex Buffer Objects

	Shader* shader;  ///shader object 

public:
	Ai();
	Ai(float width, float height, float x, float y, float thrust);

	void init(float colour[3]);
	void render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);
	void update() override;

	//controls variables
	int health;
	//physics variables

	float thrust;
	float constThrust;

	float width, height;

	//bullet variables
	bool bullet_fire;
	float bullet_speed;

	std::vector<Bullet> bullet_list;


	glm::vec4 prevNode;
	glm::vec4 nextNode;
	int positionInNodes;

	glm::vec4 theoryPos;
	int loops;

};