
class Shader;

#include "Sprite.h"
#include <iostream>

class Stage {
private:
	Shader* shader;  ///shader object 
public:
	Stage();					//default constructor#
	void init();
	void update();
	void render();

	std::vector<Sprite> spritelist;

};