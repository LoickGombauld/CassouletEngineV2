#pragma once
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Constant.h>

#include <CoraEngineLibrarie/Export.hpp>
#include <CoraEngineLibrarie/SpriteManager.hpp>


class CASSOULET_DLL Animation
{
	//Does the animation go back and forth?

public:
	Animation(const std::string& i_sprite_name, const bool i_ping_pong = 0, const float i_animation_speed = 1, const float i_current_frame = 0);
	Animation() {};

	bool Update(const char i_animation_end = 0);

	float get_animation_speed() const;
	float get_current_frame() const;

	void draw(const sf::Vector2<short>& i_position, sf::RenderWindow& i_window, const bool i_mirror_horizontal = 0, const bool i_mirror_vertical = 0, const float i_scale_x = 1, const float i_scale_y = 1, const sf::Color& i_color = sf::Color(255, 255, 255), const sf::Rect<unsigned short>& i_texture_box = sf::Rect<unsigned short>(0, 0, USHRT_MAX, USHRT_MAX)) const;
	void set_animation_speed(const float i_animation_speed);
	void set_current_frame(const float i_current_frame);
};