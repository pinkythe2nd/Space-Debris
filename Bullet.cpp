#include "Bullet.h"
#include "shaders\Shader.h"

#include <iostream>

Bullet::Bullet() {
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	width = 0.0f;
	height = 0.0f;
}

Bullet::Bullet(float fangle, float fx_vel, float fy_vel, float fx, float fy, Shader *shade, int i) {
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	width = 10.f;
	height = 25.f;

	index = i;
	shader = shade;

	sides = 4;
	pos.x = fx;
	pos.y = fy;

	velocity.x = fx_vel;
	velocity.y = fy_vel;

	angle = fangle;
	mass = 0.25f;
	active = true;
}

void Bullet::update() {
	pos.x += velocity.x;
	pos.y += velocity.y;
}

void Bullet::init(float colour[3]) {
	//Load the GLSL program 
	//Create the geometry

	float halfBase = width / 2.0f;
	float halfHeight = height / 2.0f;

	vectors.push_back(glm::vec3(-halfBase, halfBase, 0.0));
	vectors.push_back(glm::vec3(-halfBase, -halfBase, 0.0));
	vectors.push_back(glm::vec3(halfBase, -halfBase, 0.0));

	vectors.push_back(glm::vec3(-halfBase, halfBase, 0.0));
	vectors.push_back(glm::vec3(halfBase, halfBase, 0.0));
	vectors.push_back(glm::vec3(halfBase, -halfBase, 0.0));

	og_vectors = vectors;

	float col[18];	// colour array
	col[0] = colour[0]; col[1] = colour[1]; col[2] = colour[2]; //r,g,b values for each vertex
	col[3] = colour[0]; col[4] = colour[1]; col[5] = colour[2]; //r,g,b values for each vertex
	col[6] = colour[0]; col[7] = colour[1]; col[8] = colour[2]; //r,g,b values for each vertex

	col[9] = colour[0]; col[10] = colour[1]; col[11] = colour[2]; //r,g,b values for each vertex
	col[12] = colour[0]; col[13] = colour[1]; col[14] = colour[2]; //r,g,b values for each vertex
	col[15] = colour[0]; col[16] = colour[1]; col[17] = colour[2]; //r,g,b values for each vertex

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
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), col, GL_STATIC_DRAW);

	//set the colour - linked to the colour shader input.
	GLint colorLocation = glGetAttribLocation(shader->handle(), "in_Color");
	glEnableVertexAttribArray(colorLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//good practice to bind to 0.
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Bullet::render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix){
	transformPoints(ModelViewMatrix);
	glUseProgram(shader->handle());  // use the shader

	//set the uniform for the projectionmatrix
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//pass the uniform for the ModelView matrix to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	//Draw the object
	glBindVertexArray(m_vaoID);		// select first VAO
	glDrawArrays(GL_TRIANGLES, 0, 6);	// draw first object

	glBindVertexArray(0); //unbind the vertex array object
	glUseProgram(0); //turn off the current shader
}
