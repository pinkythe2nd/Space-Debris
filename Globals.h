#pragma once
#include <string>

#include "Player.h"
#include "Asteroid.h"
#include "Ai.h"
#include "RaceTrack.h"

extern float load_percent;
extern bool loaded;
extern bool loading;
extern bool asteroidsInit;

extern Player player;
extern Ai ai;
extern Ai ai2;
extern RaceTrack raceTrack;
extern vector<Asteroid> asteroid_vector;

extern float red[3];
extern float blue[3];
extern float green[3];

extern const float game_width;
extern const float game_height;

extern int screen_width;
extern int screen_height;

extern const float MAP_WIDTH;
extern const float MAP_HEIGHT;
