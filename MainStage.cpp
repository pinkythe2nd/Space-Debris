#include "MainStage.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "GL\freeglut.h"
#include "FreeImage.h"



std::random_device os_seed;
const uint_least32_t seed = os_seed();
std::default_random_engine generator(seed);

auto Foo() -> vector<array<float, 2>> {
	constexpr auto kRadius = 150.0F;
	constexpr auto kXMin = array<float, 2>{ {-23428, -13180}};
	constexpr auto kXMax = array<float, 2>{ {23428, 13180}};

	return thinks::PoissonDiskSampling(kRadius, kXMin, kXMax);
}

MainStage::MainStage() {
	player = Player(50.0f, 50.0f, 0.0f, 0.0f, 0.05f, 0.075f);
	background = Background(MAP_WIDTH / 2, MAP_HEIGHT / 2, 0.0f, 11714, 6590);
	ai = Ai(50.0f, 50.0f, -250.0f, 250.0f, 4);
	ai2 = Ai(50.0f, 50.0f, 250.0f, 250.0f, 4);
	load_percent = 0;
	dave = Foo();
	a = dave.size();
	percent = 100.0f / a;
}

double euclidan_distance(Sprite& sprite1, Sprite& sprite2) {
	return sqrt(pow(sprite2.pos.x - sprite1.pos.x, 2) + (sprite2.pos.y - sprite1.pos.y, 2));
}

float get_random(const float &high, const float &low) {

	std::uniform_real_distribution<double> dis(low, high);
	return dis(generator);
}

void MainStage::load_asteroids(int start, int end) {
	for (int i = start; i < end; i++) {
		auto& coord = dave.at(i);
		if ((coord.at(0) > game_width || coord.at(0) < -game_width) || (coord.at(1) > game_height || coord.at(1) < -game_height)) {
			Asteroid temp = Asteroid(coord.at(0), coord.at(1), 0, get_random(70.0f, 20.0f), asteroid_vector.size(), false, false);
			temp.init(green, asteroidShader);
			asteroid_vector.push_back(temp);
		}
		else {
			Asteroid temp = Asteroid(coord.at(0), coord.at(1), 0, get_random(70.0f, 20.0f), asteroid_vector.size(), true, false);
			temp.init(green, asteroidShader);
			asteroid_vector.push_back(temp);
		}
		load_percent += percent;
	}

}

void MainStage::placeAsteroids() {
	for (Asteroid& asteroid : asteroid_vector) {
		for (int i = 0; i < raceTrack.sides; i++) {
			if (sqrt(pow(raceTrack.vert[i].x - asteroid.pos.x, 2) +
				pow(raceTrack.vert[i].y - asteroid.pos.y, 2)) < 250) {
				asteroid.active = false;
				break;
			}
			else {
				asteroid.active = true;
			}
		}
	}

	vector<glm::vec4> temp;

	for (int i = raceTrack.waypoints.size() / 2; i > -1; i--) {
		temp.push_back(raceTrack.waypoints.at(i));
	}

	for (int i = raceTrack.waypoints.size() - 1; i > raceTrack.waypoints.size() / 2 - 1; i--) {
		temp.push_back(raceTrack.waypoints.at(i));
	}

	raceTrack.waypoints = temp;
	temp.clear();

	for (int i = raceTrack.bumpy_waypoints.size() / 2; i > -1; i--) {
		temp.push_back(raceTrack.bumpy_waypoints.at(i));
	}

	for (int i = raceTrack.bumpy_waypoints.size() - 1; i > raceTrack.bumpy_waypoints.size() / 2 - 1; i--) {
		temp.push_back(raceTrack.bumpy_waypoints.at(i));
	}

	raceTrack.bumpy_waypoints = temp;
	temp.clear();

	for (int i = raceTrack.bumpy_waypoints2.size() / 2; i > -1; i--) {
		temp.push_back(raceTrack.bumpy_waypoints2.at(i));
	}

	for (int i = raceTrack.bumpy_waypoints2.size() - 1; i > raceTrack.bumpy_waypoints2.size() / 2 - 1; i--) {
		temp.push_back(raceTrack.bumpy_waypoints2.at(i));
	}

	raceTrack.bumpy_waypoints2 = temp;
	temp.clear();

	player.prevNode = raceTrack.waypoints.at(0);
	player.nextNode = raceTrack.waypoints.at(1);

	ai.prevNode = raceTrack.bumpy_waypoints2.at(0);
	ai.nextNode = raceTrack.bumpy_waypoints2.at(1);

	ai2.prevNode = raceTrack.bumpy_waypoints.at(0);
	ai2.nextNode = raceTrack.bumpy_waypoints.at(1);

	vector<Asteroid> temp_vector;
	for (Asteroid& asteroid : asteroid_vector) {
		if (asteroid.active) {
			temp_vector.push_back(asteroid);
		}
	}
	asteroid_vector = temp_vector;
	temp_vector.clear();
	loading = false;
	loaded = true;


	for (int i = 0; i < raceTrack.vert.size(); i++) {
		cout << glm::to_string(raceTrack.vert[i]) << endl;
	}

}

void MainStage::init() {
	player.init(red);

	ai.init(blue);
	ai2.init(blue);

	background.init(red);

	raceTrack = RaceTrack(4000.0f, 0, 0, 4000.0f);
	raceTrack.init();

	our_font.init("font.ttf", 50, glm::vec4(0.9f, 0.9f, 0.2f, 1.0f));
	our_font_shadow.init("font.ttf", 50, glm::vec4(0.9f, 0.9f, 0.2f, 0.5f));

	asteroidShader = new Shader;
	if (!asteroidShader->load("Asteroid Shader", "./shaders/basicTexture.vert", "./shaders/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}
}

void MainStage::collision() {
	process_keys();
	update_sprites();
	for (Asteroid& asteroid : asteroid_vector) {
		if (!asteroid.active) {
			continue;
		}

		if (asteroid.dead) {
			continue;
		}

		for (Bullet& bullet : player.bullet_list) {
			if ((bullet.pos.x > game_width || bullet.pos.x < -game_width) || (bullet.pos.y > game_height || bullet.pos.y < -game_height)) {
				bullet.active = false;
				continue;
			}

			if (!bullet.active) continue;
			
			if (euclidan_distance(asteroid, bullet) < 200) {
				if (bullet.ShapeOverlap_SAT_STATIC(asteroid)) {
					asteroid.velocity.x += bullet.velocity.x / (asteroid.mass * 4);
					asteroid.velocity.y += bullet.velocity.y / (asteroid.mass * 4);

					asteroid.health -= 1;
					if (asteroid.health <= 0) {
						asteroid.dead = true;
					}
					bullet.active = false;
				}
			}
			if (bullet.ShapeOverlap_SAT_STATIC(ai)) {
				ai.pos.x += bullet.velocity.x;
				ai.pos.y += bullet.velocity.y;

				ai.theoryPos.x += bullet.velocity.x;
				ai.theoryPos.y += bullet.velocity.y;

				bullet.active = false;
			}

			if (bullet.ShapeOverlap_SAT_STATIC(ai2)) {
				ai2.pos.x += bullet.velocity.x;
				ai2.pos.y += bullet.velocity.y;

				ai2.theoryPos.x += bullet.velocity.x;
				ai2.theoryPos.y += bullet.velocity.y;

				bullet.active = false;
			}
		}
		for (Asteroid& asteroid2 : asteroid_vector) {
			if (!asteroid2.active) continue;
			if (asteroid2 == asteroid) continue;
			if (euclidan_distance(asteroid, asteroid2) > 200) continue;

			if (asteroid.ShapeOverlap_SAT_STATIC(asteroid2)) {
				if (abs(asteroid2.velocity.x) + abs(asteroid2.velocity.y) <
					abs(asteroid.velocity.x) + abs(asteroid.velocity.y)) {

					asteroid2.velocity.x += asteroid.velocity.x / asteroid2.mass;
					asteroid2.velocity.y += asteroid.velocity.y / asteroid2.mass;

					asteroid.velocity.x -= asteroid.velocity.x / asteroid2.mass;
					asteroid.velocity.y -= asteroid.velocity.y / asteroid2.mass;

				}
				else {
					asteroid.velocity.x += asteroid2.velocity.x / asteroid.mass;
					asteroid.velocity.y += asteroid2.velocity.y / asteroid.mass;

					asteroid2.velocity.x -= asteroid2.velocity.x / asteroid.mass;
					asteroid2.velocity.y -= asteroid2.velocity.y / asteroid.mass;
				}
			}
		}

		if (euclidan_distance(asteroid, player) < 200) {
			if (player.ShapeOverlap_SAT_STATIC(asteroid)) {
				asteroid.velocity.x += player.velocity.x / asteroid.mass;
				asteroid.velocity.y += player.velocity.y / asteroid.mass;

				player.velocity.x = asteroid.velocity.x / asteroid.mass;
				player.velocity.y = asteroid.velocity.y / asteroid.mass;

			}
		}

		if (euclidan_distance(asteroid, ai) < 200) {
			if (ai.ShapeOverlap_SAT_STATIC(asteroid)) {
				asteroid.velocity.x += ai.velocity.x / asteroid.mass;
				asteroid.velocity.y += ai.velocity.y / asteroid.mass;

				ai.thrust = ai.thrust / 2;
			}
		}
		if (euclidan_distance(asteroid, ai2) < 200) {
			if (ai2.ShapeOverlap_SAT_STATIC(asteroid)) {
				asteroid.velocity.x += ai2.velocity.x / asteroid.mass;
				asteroid.velocity.y += ai2.velocity.y / asteroid.mass;

				ai2.thrust = ai2.thrust / 2;
			}
		}

		if (ai.ShapeOverlap_SAT_STATIC(player)) {
			ai.thrust += 0.02;

			player.velocity.x = 0;
			player.velocity.y = 0;

		}
		if (ai2.ShapeOverlap_SAT_STATIC(player)) {
			ai2.thrust += 0.02;

			player.velocity.x = 0;
			player.velocity.y = 0;

		}

		if (ai.ShapeOverlap_SAT_STATIC(ai2)) {
			ai2.velocity.x += ai.velocity.x / 2;
			ai2.velocity.y += ai.velocity.y / 2;

			ai.velocity.x = ai2.velocity.x / 2;
			ai.velocity.y = ai2.velocity.y / 2;
		}

		if (ai2.ShapeOverlap_SAT_STATIC(ai)) {
			ai.velocity.x += ai2.velocity.x / 2;
			ai.velocity.y += ai2.velocity.y / 2;


			ai2.velocity.x = ai.velocity.x / 2;
			ai2.velocity.y = ai.velocity.y / 2;
		}
	}


	float a_distance = sqrt(pow(player.prevNode.x - player.theoryPos.x, 2) + pow(player.prevNode.y - player.theoryPos.y, 2));
	float b_distance = sqrt(pow(player.nextNode.x - player.theoryPos.x, 2) + pow(player.nextNode.y - player.theoryPos.y, 2));

	if (b_distance < a_distance) {
		if (player.positionInNodes == raceTrack.waypoints.size() - 1) {
			player.loops += 1;
		}
		player.positionInNodes = (player.positionInNodes % (raceTrack.waypoints.size() - 1)) + 1;

		player.prevNode = player.nextNode;
		player.nextNode = raceTrack.waypoints.at(player.positionInNodes);
	}

	a_distance = sqrt(pow(ai.prevNode.x - ai.theoryPos.x, 2) + pow(ai.prevNode.y - ai.theoryPos.y, 2));
	b_distance = sqrt(pow(ai.nextNode.x - ai.theoryPos.x, 2) + pow(ai.nextNode.y - ai.theoryPos.y, 2));

	if (b_distance < a_distance) {
		if (ai.positionInNodes == raceTrack.bumpy_waypoints.size() - 1) {
			ai.loops += 1;
		}

		ai.positionInNodes = (ai.positionInNodes % (raceTrack.bumpy_waypoints.size() - 1)) + 1;
		ai.prevNode = ai.nextNode;
		ai.nextNode = raceTrack.bumpy_waypoints2.at(ai.positionInNodes);
	}

	a_distance = sqrt(pow(ai2.prevNode.x - ai2.theoryPos.x, 2) + pow(ai2.prevNode.y - ai2.theoryPos.y, 2));
	b_distance = sqrt(pow(ai2.nextNode.x - ai2.theoryPos.x, 2) + pow(ai2.nextNode.y - ai2.theoryPos.y, 2));

	if (b_distance < a_distance) {
		if (ai2.positionInNodes == raceTrack.bumpy_waypoints2.size() - 1) {
			ai2.loops += 1;
		}
		ai2.positionInNodes = (ai2.positionInNodes % (raceTrack.bumpy_waypoints2.size() - 1)) + 1;
		ai2.prevNode = ai2.nextNode;
		ai2.nextNode = raceTrack.bumpy_waypoints.at(ai2.positionInNodes);
	}
}

void MainStage::process_keys() {
	bool moving = false;
	if (player.shift_left) {
		player.velocity.x += player.thrust * sin(glm::radians(90 - player.angle));
		player.velocity.y += player.thrust * sin(glm::radians(player.angle));
		moving = true;
	}if (player.left) {
		player.angle += 1.5f;
	}if (player.shift_right) {
		player.velocity.x -= player.thrust * sin(glm::radians(90 - player.angle));
		player.velocity.y -= player.thrust * sin(glm::radians(player.angle));
		moving = true;
	}if (player.right) {
		player.angle -= 1.5f;
	}if (player.up) {
		player.velocity.x -= player.thrust * sin(glm::radians(player.angle));
		player.velocity.y += player.thrust * sin(glm::radians(90 - player.angle));
		moving = true;
	}if (player.down) {
		player.velocity.x += player.thrust * sin(glm::radians(player.angle));
		player.velocity.y -= player.thrust * sin(glm::radians(90 - player.angle));
		moving = true;
	}if (player.space) {
		if (player.bullet_fire) {
			player.bullet_list.push_back(Bullet(player.angle,
				(player.bullet_speed + abs(player.velocity.x)) * -sin(glm::radians(player.angle)),
				(player.bullet_speed + abs(player.velocity.y)) * sin(glm::radians(90 - player.angle)),
				0, 0, asteroidShader, player.bullet_list.size()));
			player.bullet_list.back().init(red);
			player.bullet_fire = false;
		}
	}if (player.z_key && !moving) {
		if (player.velocity.x != 0) {
			if (player.velocity.x > 0) {
				player.velocity.x -= player.interia_dampners;
			}
			else {
				player.velocity.x += player.interia_dampners;
			}
		}if (player.velocity.y != 0) {
			if (player.velocity.y > 0) {
				player.velocity.y -= player.interia_dampners;
			}
			else {
				player.velocity.y += player.interia_dampners;
			}
		}
		player.velocity.y = ceil(player.velocity.y * 100.0) / 100.0;
		player.velocity.x = ceil(player.velocity.x * 100.0) / 100.0;
	}
}


void MainStage::update_sprites() {
	for (Bullet& bullet : player.bullet_list) {
		bullet.update();
	}


	for (Asteroid& asteroid : asteroid_vector) {
		if ((asteroid.pos.x > game_width + 100 || asteroid.pos.x < -game_width - 100) || (asteroid.pos.y > game_height + 100 || asteroid.pos.y < -game_height - 100)) {
			asteroid.active = false;
		}
		else {
			asteroid.active = true;
			asteroid.update();
		}


		asteroid.pos.x -= player.velocity.x;
		asteroid.pos.y -= player.velocity.y;
	}

	player.update();

	background.pos.x -= player.velocity.x;
	background.pos.y -= player.velocity.y;

	//ai 1
	double result = glm::degrees(atan2f(ai.nextNode.y - ai.theoryPos.y, ai.nextNode.x - ai.theoryPos.x)) - 90;
	double angle_to_player = glm::degrees(atan2(player.pos.y - ai.pos.y, player.pos.x - ai.pos.x)) - 90;
	
	ai.angle = result;

	//------------ ai catchup mechanic ----------
	double speed_scale = euclidan_distance(ai, player) / 500 / ai.thrust;
	glm::vec3 hello = glm::normalize(ai.nextNode - ai.theoryPos);
	glm::vec3 playerhello = glm::normalize(player.pos - ai.pos);

	double a = glm::dot(hello, playerhello);


	if (a > 0) {
		if (ai.thrust < 10) {
			ai.thrust += speed_scale;
		}
	}
	else {
		if (round(ai.thrust) == ai.constThrust) {
			ai.thrust = ai.constThrust;
		}
		else {
			ai.thrust -= ai.thrust / 200;
		}
	}


	if (ai.thrust < ai.constThrust) {
		ai.thrust += 0.5;
	}

	//ai 2
	result = glm::degrees(atan2(ai2.nextNode.y - ai2.theoryPos.y, ai2.nextNode.x - ai2.theoryPos.x)) - 90;
	angle_to_player = glm::degrees(atan2(player.pos.y - ai2.pos.y, player.pos.x - ai2.pos.x)) - 90;

	ai2.angle = result;

	//------------ ai2 catchup mechanic ----------
	speed_scale = euclidan_distance(ai2, player) / 500 / ai2.thrust;
	hello = glm::normalize(ai2.nextNode - ai2.theoryPos);
	playerhello = glm::normalize(player.pos - ai2.pos);

	a = glm::dot(hello, playerhello);


	if (a > 0) {
		if (ai2.thrust < 10) {
			ai2.thrust += speed_scale;
		}
	}
	else {
		if (round(ai2.thrust) == ai2.constThrust) {
			ai2.thrust = ai2.constThrust;
		}
		else {
			ai2.thrust -= ai2.thrust / 200;
		}
	}

	if (ai2.thrust < ai2.constThrust) {
		ai2.thrust += 0.5;
	}

	//-------------ai update-----------

	ai.pos.x -= player.velocity.x;
	ai.pos.y -= player.velocity.y;

	ai.update();

	//ai 2
	ai2.pos.x -= player.velocity.x;
	ai2.pos.y -= player.velocity.y;

	ai2.update();
}


void MainStage::render_target() {
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return false;
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
}

void MainStage::render(glm::mat4& projection_matrix) {
	process_keys();
	update_sprites();
	collision();

	view_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

	glEnable(GL_BLEND);
	glm::mat4 player_transform = glm::translate(view_matrix, glm::vec3(background.pos.x, background.pos.y, 0.0));
	background.render(player_transform, projection_matrix);
	glDisable(GL_BLEND);

	player_transform = glm::translate(view_matrix, glm::vec3(0.0, 0, 0.0));
	player_transform = glm::rotate(player_transform, glm::radians(player.angle), glm::vec3(0.0, 0.0, 1.0));
	player.render(player_transform, projection_matrix);

	//ai 1
	player_transform = glm::translate(view_matrix, glm::vec3(ai.pos.x, ai.pos.y, 1.0));
	player_transform = glm::rotate(player_transform, glm::radians(ai.angle), glm::vec3(0, 0, 1.0));
	ai.render(player_transform, projection_matrix);

	//ai 2
	player_transform = glm::translate(view_matrix, glm::vec3(ai2.pos.x, ai2.pos.y, 1.0));
	player_transform = glm::rotate(player_transform, glm::radians(ai2.angle), glm::vec3(0, 0, 1.0));
	ai2.render(player_transform, projection_matrix);

	for (Bullet& bullet : player.bullet_list) {
		if (!bullet.active) continue;

		player_transform = glm::translate(view_matrix, glm::vec3(bullet.pos.x, bullet.pos.y, 1.0));
		player_transform = glm::rotate(player_transform, glm::radians(bullet.angle), glm::vec3(0.0, 0.0, 1.0));
		bullet.render(player_transform, projection_matrix);
	}

	for (Asteroid& asteroid : asteroid_vector) {
		if (!asteroid.active) {
			continue;
		}
		if (asteroid.dead) {
			continue;
		}
		player_transform = glm::translate(view_matrix, glm::vec3(asteroid.pos.x, asteroid.pos.y, 1.0));
		player_transform = glm::rotate(player_transform, glm::radians(asteroid.angle), glm::vec3(0.0, 0.0, 1.0));
		asteroid.render(player_transform, projection_matrix);
	}

	our_font.print(projection_matrix, our_font, -800.0f, 800.0f, ("Lap: " + to_string(player.loops) + " out of: 3").c_str());
	our_font_shadow.print(projection_matrix, our_font, -805.0f, 800.0f, ("Lap: " + to_string(player.loops) + " out of: 3").c_str());
}