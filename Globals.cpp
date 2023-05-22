#include "Globals.h"

float load_percent = 0;

bool loaded = false;
bool loading = false;
bool asteroidsInit = false;

Player player;
Ai ai;
Ai ai2;
RaceTrack raceTrack;
vector<Asteroid> asteroid_vector;

float red[3] = { 1, 0, 0 };
float blue[3] = { 0, 0, 1 };
float green[3] = { 0, 1, 0 };

const float game_width = 1000.0f;
const float game_height = 1000.0f;

int screen_width = 450, screen_height = 450;

const float MAP_WIDTH = 11714 * 2;
const float MAP_HEIGHT = 6590 * 2;