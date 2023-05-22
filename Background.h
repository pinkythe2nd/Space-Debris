#pragma once

class Shader;

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "gl\glew.h"
#include "Sprite.h"


#include <vector>

class Background : public Sprite{
private:
	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[3];	// ids for Vertex Buffer Objects

	Shader* shader;  ///shader object 

	GLuint m_TexName; //identifier for the texture

public:
	Background();					//default constructor
	Background(float angle, float x, float y, int width, int height);
	void init(float colour[3]);
	void render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);

	float angle, x_velocity, y_velocity, mass;
	int sides, width, height;
};