#include "Shield.h"
#include "shaders\Shader.h"

#include <iostream>

Shield::Shield() {
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;

	cx = 0.0f;
	cy = 0.0f;

	angle = 0.0f;
	m_NumOfVerts = 0;
	radius = 0.0f;
	num_segments = 0.0f;
	shield_health = 0;
}

Shield::Shield(float a, float xp, float yp, float radi, float num_seg) {
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;

	cx = xp;
	cy = yp;

	radius = radi;
	angle = a;
	m_NumOfVerts = 0.0f;
	num_segments = num_seg;
	shield_health = 100;
}

void Shield::init(float colour[3])
{
	//Load the GLSL program 
	shader = new Shader;
	if (!shader->load("Basic", "./shaders/basicTexture.vert", "./shaders/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}
}


int Shield::inCircle(Sprite& sprite) {
	for (glm::vec4& point : sprite.p) {
		float square_dist = pow((pos.x - point.x), 2) + pow((pos.y - point.y), 2);
		if (square_dist <= pow(radius, 2)) {
			return true;
		}
	}
	return false;
}

void Shield::render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix) {

	glUseProgram(shader->handle());  // use the shader

	//set the uniform for the projectionmatrix
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//pass the uniform for the ModelView matrix to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);


	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++) {
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
		float x = radius * cosf(theta);//calculate the x component 
		float y = radius * sinf(theta);//calculate the y component 
		glVertex2f(x + cx, y + cy);//output vertex 
	}
	glEnd();
	
	glUseProgram(0); //turn off the current shader
}
