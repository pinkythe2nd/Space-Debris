#include "Sprite.h"


Sprite::Sprite() {
	shader = nullptr;

	angle = 0;
	sides = 0;

	mass = 0.0f;

	velocity = glm::vec2(0.0f, 0.0f);
	pos = glm::vec3(0.0f, 0.0f, 0.0f);

	active = false;
	renderme = false;
}

Sprite::Sprite(float fangle, float x, float y) {
	shader = nullptr;

	angle = fangle;
	sides = 0;

	mass = 0.0f;

	velocity = glm::vec2(x, y);
	pos = glm::vec4(x, y, 0.0f, 0.0f);

	active = false;
	renderme = true;
}

bool Sprite::operator == (Sprite& comparison) {
	if (pos.x == comparison.pos.x && pos.y == comparison.pos.y)return true;
	return false;
}

bool Sprite::operator != (Sprite& comparison) {
	if (pos.x == comparison.pos.x && pos.y == comparison.pos.y)return false;
	return true;
}

void Sprite::update(){
	pos.x -= velocity.x;
	pos.y -= velocity.y;

}

void Sprite::transformPoints(glm::mat4 ModelViewMatrix){
	for (int i = 0; i < og_vectors.size(); i++) {
		vectors[i].x = vectors[i].x + pos.x;
		vectors[i].y = vectors[i].y + pos.y;
		vectors[i] = glm::vec4(
			((og_vectors[i].x * cosf(angle)) - (og_vectors[i].y * sinf(angle)) + pos.x),
			((og_vectors[i].x * sinf(angle)) + (og_vectors[i].y * cosf(angle)) + pos.y), 0.0f, 0.0f);
	}
}

bool Sprite::ShapeOverlap_SAT_STATIC(Sprite& r1){
	float overlap = INFINITY;
	for (int a = 0; a < r1.vectors.size(); a++)
	{
		int b = (a + 1) % r1.vectors.size();
		glm::vec4 axisProj = glm::vec4(-(r1.vectors[b].y - r1.vectors[a].y), r1.vectors[b].x - r1.vectors[a].x, 0.0f, 0.0f );

		// Work out min and max 1D points for r1
		float min_r1 = INFINITY, max_r1 = -INFINITY;
		for (int p = 0; p < r1.vectors.size(); p++)
		{
			float q = (r1.vectors[p].x * axisProj.x + r1.vectors[p].y * axisProj.y);
			min_r1 = min(min_r1, q);
			max_r1 = max(max_r1, q);
		}

		// Work out min and max 1D points for r2
		float min_r2 = INFINITY, max_r2 = -INFINITY;
		for (int t = 0; t < vectors.size(); t++)
		{
			float q = (vectors[t].x * axisProj.x + vectors[t].y * axisProj.y);
			min_r2 = min(min_r2, q);
			max_r2 = max(max_r2, q);
		}

		// Calculate actual overlap along projected axis, and store the minimum
		overlap = min(min(max_r1, max_r2) - max(min_r1, min_r2), overlap);

		if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
			return false;
	}

	// If we got here, the objects have collided, we will displace r1
	// by overlap along the vector between the two object centers

	return true;
}