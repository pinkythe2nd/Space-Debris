#include "Asteroid.h"
#include "shaders\Shader.h"
#include "glm/gtx/string_cast.hpp"

#include <iostream>
#include <random>
#include <math.h>

Asteroid::Asteroid() {
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;

	angle = 0.0f;
	sides = 0;
	radius = 0;
	random_bool = false;
	mass = 0;

	active = false;

	pos = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

Asteroid::Asteroid(float x, float y, float ang, float radi, int i, bool act, bool randomp) {
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;

	angle = ang;
	sides = radi / 4;
	random_bool = randomp;
	radius = radi;

	mass = sides;
	active = act;

	health = sides;
	max_health = sides;

	dead = false;
	pos = glm::vec4(x, y, 0.0f, 0.0f);
}

float get_random(float high) {
	using engine = std::mt19937;
	std::random_device os_seed;
	const uint_least32_t seed = os_seed();
	engine generator(seed);

	std::uniform_real_distribution<> dis(-high, high); // range 0 - 1
	return dis(generator);
}

int Asteroid::inCircle(Asteroid& sprite) {
	float square_dist = pow((pos.x - sprite.pos.x), 2) + pow((pos.y - sprite.pos.y), 2);
	if (square_dist <= pow(radius, 2)) {
		return true;
	}
	if (square_dist <= pow(sprite.radius, 2)) {
		return true;
	}
	return false;
}


void Asteroid::update() {
	pos += velocity.x;
	pos.y += velocity.y;

}

void Asteroid::init(float colour[3], Shader* shade) {
	shader = shade;
	float* col = new float[sides * 9];	// colour array
	float x;
	float y;

	for (int i = 0; i < sides; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(sides);//get the current angle 
		x = radius * cosf(theta) + get_random(sides);//calculate the x component 
		y = radius * sinf(theta) + get_random(sides);//calculate the y component

		vectors.push_back(glm::vec3(x, y, 0.0f));
		col[i * 3] = colour[0]; col[i * 3 + 1] = colour[1]; col[i * 3 + 2] = colour[2];
	}
	og_vectors = vectors;
	//VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);

	glGenBuffers(2, m_vboID); // we need two VBOs - one for the vertices and one for the colours

	//Lets set up the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);

	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, vectors.size() * sizeof(glm::vec3), &vectors[0], GL_STATIC_DRAW);

	//set the position - linked to the position shader input
	GLint vertexLocation = glGetAttribLocation(shader->handle(), "in_Position");
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Now set up the colours
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, sides * 9 * sizeof(GLfloat), col, GL_STATIC_DRAW);

	//set the colour - linked to the colour shader input.
	GLint colorLocation = glGetAttribLocation(shader->handle(), "in_Color");
	glEnableVertexAttribArray(colorLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//good practice to bind to 0.
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glm::mat4 view_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	glm::mat4 player_transform = glm::translate(view_matrix, glm::vec3(0.0, 0, 1.0));
	player_transform = glm::rotate(player_transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
	transformPoints(player_transform);
}

void Asteroid::render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix) {
	transformPoints(ModelViewMatrix);
	glUseProgram(shader->handle());  // use the shader

	//set the uniform for the projectionmatrix
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//pass the uniform for the ModelView matrix to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	glLineWidth((float(health) / float(max_health)) * 3);

	if (health < max_health / 2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(m_vaoID);		// select first VAO

		glDrawArrays(GL_TRIANGLE_FAN, 0, sides);	// draw first object

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		//Draw the object
		glBindVertexArray(m_vaoID);		// select first VAO
		glDrawArrays(GL_LINE_LOOP, 0, sides);	// draw first object

	}

	glLineWidth(1.0f);
	glBindVertexArray(0); //unbind the vertex array object
	glUseProgram(0); //turn off the current shader
}
