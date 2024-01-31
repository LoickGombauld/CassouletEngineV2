#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Export.hpp>
#include <CoraEngineLibrarie/Animation.h>
#include <CoraEngineLibrarie/SpriteManager.hpp>

class CASSOULET_DLL Prop 
{
	bool in_the_view;
	//I didn't wanna make another class for animated Props.
	bool is_animation;

	float distance;

	int screen_x;

	std::string sprite_name;

	SpriteManager* sprite_manager;
	Animation animation;
public:
	Prop(const bool i_is_animation, const std::string& i_sprite_name, SpriteManager& i_sprite_manager, const float i_x = 0, const float i_y = 0);
	bool operator>(const Prop& i_Prop) const;
	bool operator<(const Prop& i_Prop) const;

	float get_distance() const;

	int get_height() const;
	int get_width() const;
	int get_x() const;
	int get_y() const;

	void draw(const short i_pitch, sf::RenderWindow& i_window);
	void Update(const sf::Vector2f& i_player_direction, const sf::Vector2f& i_player_position);
};