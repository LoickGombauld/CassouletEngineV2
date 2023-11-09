#pragma once
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Export.hpp>
#include <CoraEngineLibrarie/Constant.h>

class CASSOULET_DLL Player {
	sf::Vector2f direction;
	sf::Vector2f veloctity;
	sf::Vector2f position;
    float MaxSpeed;
	float Acceleration;
	float Deceleration;
	float speed;
public:
	Player(const float i_x = 0, const float i_y = 0);

	void set_position(const float i_x, const float i_y);
	void update(const sf::RenderWindow& i_window, const gbl::MAP::Map<>& i_map);

	sf::Vector2f get_direction() const;
	sf::Vector2f get_position() const;
};