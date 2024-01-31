#include <CoraEngineLibrarie/Prop.h>
#include <CoraEngineLibrarie/AngleFunctions.h>

Prop::Prop(const bool i_is_animation, const std::string& i_sprite_name, SpriteManager& i_sprite_manager, const float i_x, const float i_y) :
	in_the_view(0),
	is_animation(i_is_animation),
	distance(0),
	screen_x(0),
	sprite_name(i_sprite_name),

	sprite_manager(&i_sprite_manager),
	animation(i_sprite_name, i_sprite_manager)
{
	position = sf::Vector2f(i_x, i_y);
	if ("FIRE_CAULDRON" == i_sprite_name)
	{
		animation.set_animation_speed(gbl::SPRITES::FIRE_ANIMATION_SPEED);
	}
}

bool Prop::operator>(const Prop& i_Prop) const
{
	return distance > i_Prop.get_distance();
}

bool Prop::operator<(const Prop& i_Prop) const
{
	return distance < i_Prop.get_distance();
}

float Prop::get_distance() const
{
	return distance;
}

int Prop::get_height() const
{
	return round(gbl::SCREEN::HEIGHT / (distance * tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_VERTICAL))));
}

int Prop::get_width() const
{
	float sprite_height = sprite_manager->get_sprite_data(sprite_name).texture_box.height;
	float sprite_width = sprite_manager->get_sprite_data(sprite_name).texture_box.width;

	return round(gbl::SCREEN::HEIGHT * sprite_width / (distance * sprite_height * tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL))));
}

int Prop::get_x() const
{
	return screen_x - round(0.5f * get_width());
}

int Prop::get_y() const
{
	return round(0.5f * (gbl::SCREEN::HEIGHT - get_height()));
}

void Prop::draw(const short i_pitch, sf::RenderWindow& i_window)
{
	float sprite_height = sprite_manager->get_sprite_data(sprite_name).texture_box.height;
	float sprite_width = sprite_manager->get_sprite_data(sprite_name).texture_box.width;

	unsigned char shade = 255 * std::clamp<float>(1 - distance / gbl::RAYCASTING::RENDER_DISTANCE, 0, 1);

	in_the_view &= gbl::SCREEN::HEIGHT > i_pitch + get_y() && gbl::SCREEN::WIDTH > get_x() && get_x() > -1 * get_width() && i_pitch + get_y() > -1 * get_height();

	if (0 < shade && 1 == in_the_view)
	{
		if (0 == is_animation)
		{
			sprite_manager->draw_sprite(0, sprite_name, sf::Vector2<short>(get_x(), i_pitch + get_y()), i_window, 0, 0, get_width() / sprite_width, get_height() / sprite_height, sf::Color(shade, shade, shade));
		}
		else
		{
			animation.draw(sf::Vector2<short>(get_x(), i_pitch + get_y()), i_window, 0, 0, get_width() / sprite_width, get_height() / sprite_height, sf::Color(shade, shade, shade));
		}
	}
}

void Prop::Update(const sf::Vector2f& i_player_direction, const sf::Vector2f& i_player_position)
{
	float angle = get_radians(atan2(i_player_position.y - position.y, position.x - i_player_position.x));
	float difference = deg_difference(i_player_direction.x, rad_to_deg(angle));

	//This makes it so that the difference is between -180 to 180.
	if (deg_difference(i_player_direction.x, difference + rad_to_deg(angle)) < deg_difference(i_player_direction.x, rad_to_deg(angle) - difference))
	{
		difference *= -1;
	}

	//We're calculating the perpendicular distance, not the Euqalifalistadalidian one.
	distance = abs(i_player_position.y - position.y - tan(deg_to_rad(i_player_direction.x - 90)) * (position.x - i_player_position.x)) / sqrt(1 + pow(tan(deg_to_rad(i_player_direction.x - 90)), 2));

	screen_x = round(0.5f * gbl::SCREEN::WIDTH * (1 - tan(deg_to_rad(difference)) / tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL))));

	in_the_view = 90 > abs(difference);

	if (1 == is_animation)
	{
		animation.Update();
	}
}