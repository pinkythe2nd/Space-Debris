#pragma once

class Shader;

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <vector>

using namespace std;

class RaceTrack{
private:
	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[2];	// ids for Vertex Buffer Objects

	unsigned int m_vaoIDtwo;		// id for Vertex Array Object
	unsigned int m_vboIDtwo[2];	// ids for Vertex Buffer Objects
	Shader* shader;  ///shader object 

public:
	RaceTrack();
	RaceTrack(float x, float y, float angle, float radius);

	void init();
	void render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);
	void rendertwo(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);
	float x, y, angle, radius;
	int sides;
	vector <glm::vec4> vert;
	vector <glm::vec4> waypoints;
	vector <glm::vec4> bumpy_waypoints;
	vector <glm::vec4> bumpy_waypoints2;
};