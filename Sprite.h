#pragma once

class Shader;

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <vector>

#include "shaders\Shader.h"
#include "glm/gtx/string_cast.hpp"

#include <iostream>
#include <math.h>

class Sprite
{
private:
	Shader* shader;  ///shader object 
public:
	Sprite();					//default constructor
	Sprite(float angle, float x, float y);
	virtual bool ShapeOverlap_SAT_STATIC(Sprite& r1);
	void transformPoints(glm::mat4 ModelViewMatrix);
	virtual void update();

	bool operator == (Sprite& comparison);
	bool operator != (Sprite& comparison);

	std::vector <glm::vec3> vectors;
	std::vector <glm::vec3> og_vectors;
	glm::vec3 pos;	
	glm::vec2 velocity;

	float angle, mass;
	int sides;

	bool active;
	bool renderme;
};