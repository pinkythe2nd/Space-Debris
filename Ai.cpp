#include "Ai.h"

#define PI 3.1415926535897932384626433832795

Ai::Ai() {
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	width = 0.0f;
	height = 0.0f;

	thrust = 0.0f;
	angle = 0.0f;

	bullet_list;
	bullet_fire = false;
	bullet_speed = 0.0f;

	health = 0;

	prevNode = glm::vec4(0, 0, 0, 0);
	nextNode = glm::vec4(0, 0, 0, 0);
	positionInNodes = 0;

	loops = 0;
	theoryPos = glm::vec4(0, 0, 0, 0);
}

Ai::Ai(float w, float h, float x, float y, float t) {
	shader = nullptr;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	width = w;
	height = h;

	thrust = t;
	constThrust = t;
	angle = 0.0f;
	mass = 1;
	health = 3;

	bullet_list;
	bullet_fire = false;
	bullet_speed = 10.0f;

	pos = glm::vec4(x, y, 0.0f, 0.0f);

	prevNode = glm::vec4(0, 0, 0, 0);
	nextNode = glm::vec4(0, 0, 0, 0);
	positionInNodes = 1;

	theoryPos = glm::vec4(x, y, 0, 0);
	loops = 0;
}


void Ai::init(float colour[3]) {
	//Load the GLSL program 
	shader = new Shader;
	if (!shader->load("Basic", "./shaders/basicTexture.vert", "./shaders/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}

	//Create the geometry
	float halfBase = width / 2.0f;
	float halfHeight = height / 2.0f;

	vectors.push_back(glm::vec3(-halfBase, -halfHeight, 0));
	vectors.push_back(glm::vec3(0.0, halfHeight, 0));
	vectors.push_back(glm::vec3(halfBase, -halfHeight, 0));

	vectors.push_back(glm::vec3(-halfBase, -halfHeight, 0));
	vectors.push_back(glm::vec3(-halfBase, -halfHeight - 20.0f, 0));
	vectors.push_back(glm::vec3(halfHeight, -halfBase, 0));

	vectors.push_back(glm::vec3(halfBase, -halfHeight, 0));
	vectors.push_back(glm::vec3(halfBase, -halfHeight - 20.0f, 0));
	vectors.push_back(glm::vec3(-halfHeight, -halfBase, 0));

	og_vectors = vectors;

	float col[27];	// colour array
	col[0] = colour[0]; col[1] = colour[1]; col[2] = colour[2]; //r,g,b values for each vertex
	col[3] = colour[0]; col[4] = colour[1]; col[5] = colour[2]; //r,g,b values for each vertex
	col[6] = colour[0]; col[7] = colour[1]; col[8] = colour[2]; //r,g,b values for each vertex

	col[9] = colour[0]; col[10] = colour[1]; col[11] = colour[2]; //r,g,b values for each vertex
	col[12] = colour[0]; col[13] = colour[1]; col[14] = colour[2]; //r,g,b values for each vertex
	col[15] = colour[0]; col[16] = colour[1]; col[17] = colour[2]; //r,g,b values for each vertex

	col[18] = colour[0]; col[19] = colour[1]; col[20] = colour[2]; //r,g,b values for each vertex
	col[21] = colour[0]; col[22] = colour[1]; col[23] = colour[2]; //r,g,b values for each vertex
	col[24] = colour[0]; col[25] = colour[1]; col[26] = colour[2]; //r,g,b values for each vertex
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
	glBufferData(GL_ARRAY_BUFFER, 9 * 3 * sizeof(GLfloat), col, GL_STATIC_DRAW);

	//set the colour - linked to the colour shader input.
	GLint colorLocation = glGetAttribLocation(shader->handle(), "in_Color");
	glEnableVertexAttribArray(colorLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//good practice to bind to 0.
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Ai::update() {
	velocity.x = thrust * cos((90 + angle) * (PI / 180.0f));
	velocity.y = thrust * sin((90 + angle) * (PI / 180.0f));

	pos.x += velocity.x;
	pos.y += velocity.y;

	theoryPos.x += velocity.x;
	theoryPos.y += velocity.y;
}

void Ai::render(glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix) {
	transformPoints(ModelViewMatrix);
	glUseProgram(shader->handle());  // use the shader

	//set the uniform for the projectionmatrix
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//pass the uniform for the ModelView matrix to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	//Draw the object
	glBindVertexArray(m_vaoID);		// select first VAO
	glDrawArrays(GL_TRIANGLES, 0, 9);	// draw first object

	glBindVertexArray(0); //unbind the vertex array object
	glUseProgram(0); //turn off the current shader
}
