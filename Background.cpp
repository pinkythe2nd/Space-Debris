#include "Background.h"
#include "shaders/Shader.h"

#include "ImageLoading.h"

#include "glm/gtx/string_cast.hpp"

#include <iostream>
#include <math.h>

Background::Background() {
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;

	angle = 0;
	sides = 0;

	mass = 0.0f;
	x_velocity = 0.0f;
	y_velocity = 0.0f;

	pos = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	width = 0;
	height = 0;
}

Background::Background(float ang, float x, float y, int widthp, int heightp){
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;

	angle = ang;
	sides = 0;

	mass = 0.0f;
	x_velocity = 0.0f;
	y_velocity = 0.0f;

	pos = glm::vec4(x, y, 0.0f, 0.0f);
	width = widthp;
	height = heightp;
}


void Background::init(float colour[3]) {
	shader = new Shader;
	if (!shader->load("Basic", "./shaders/basicTexture2.vert", "./shaders/basicTexture2.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}


	int imageHeight = 0;
	int imageWidth = 0;
	GLubyte* pngImageData = nullptr;

	//create the texture on the GPU
	glGenTextures(1, &m_TexName);
	glBindTexture(GL_TEXTURE_2D, m_TexName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  //or use GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	bool success = ImageLoading::loadImage("textures/SCALE.png");
	if (!success) {
		std::cout << "Unable to load image file" << std::endl;
		glDeleteTextures(1, &m_TexName);
		return;
	}
	else
	{
		std::cout << "Image loaded " << std::endl;
	}



	float vert[18];	// create a vertex array

	float halfBase = width / 2.0f;
	float halfHeight = height / 2.0f;

	vert[0] = -halfBase; vert[1] = halfHeight; vert[2] = 0.0; //x,y,z values for each vertex
	vert[3] = -halfBase; vert[4] = -halfHeight; vert[5] = 0.0;
	vert[6] = halfBase; vert[7] = -halfHeight; vert[8] = 0.0;

	vert[9] = -halfBase; vert[10] = halfHeight; vert[11] = 0.0;
	vert[12] = halfBase; vert[13] = halfHeight; vert[14] = 0.0;
	vert[15] = halfBase; vert[16] = -halfHeight; vert[17] = 0.0;

	float tex[12];
	tex[0] = 0.0f;	 tex[1] = 1.0;
	tex[2] = 0.0f;	 tex[3] = 0.0;
	tex[4] = 1.0f;	 tex[5] = 0.0;

	tex[6] = 0.0f;	 tex[7] = 1.0;
	tex[8] = 1.0f;	 tex[9] = 1.0;
	tex[10] = 1.0f;	 tex[11] = 0.0;

	float col[18];	// colour array

	col[0] = colour[0]; col[1] = colour[1]; col[2] = colour[2]; //r,g,b values for each vertex
	col[3] = colour[0]; col[4] = colour[1]; col[5] = colour[2]; //r,g,b values for each vertex
	col[6] = colour[0]; col[7] = colour[1]; col[8] = colour[2]; //r,g,b values for each vertex

	col[9] = colour[0]; col[10] = colour[1]; col[11] = colour[2]; //r,g,b values for each vertex
	col[12] = colour[0]; col[13] = colour[1]; col[14] = colour[2]; //r,g,b values for each vertex
	col[15] = colour[0]; col[16] = colour[1]; col[17] = colour[2]; //r,g,b values for each vertex

	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);

	glGenBuffers(3, m_vboID); // we need three VBOs - one for the vertices and one for the colours
	//and an extra one for the texture coordinates

	//Lets set up the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);

	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), vert, GL_STATIC_DRAW);

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

	//Now set up the texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), tex, GL_STATIC_DRAW);

	//set the texture coords - linked to the texcoord shader input.
	GLint texLocation = glGetAttribLocation(shader->handle(), "in_TexCoord");
	glEnableVertexAttribArray(texLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//good practice to bind to 0.
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Background::render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix) {
	glUseProgram(shader->handle());  // use the shader

	glUniform1i(glGetUniformLocation(shader->handle(), "DiffuseMap"), 0);

	glBindTexture(GL_TEXTURE_2D, m_TexName);

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
