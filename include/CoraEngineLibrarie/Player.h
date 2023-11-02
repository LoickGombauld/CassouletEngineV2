#pragma once
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Export.hpp>
#include <CoraEngineLibrarie/Constant.h>
class CASSOULET_DLL Player {
private:
public:
	sf::CircleShape	 body;
	Player();
	void SetPosition(float x, float y);
	float angleX = 0;
	float angleY = 0;
	float Posx = 150;
	float Posy = 150;

	void UpdatePostion();

};