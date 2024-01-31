#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/AngleFunctions.h>

float deg_difference(const float i_angle_0, const float i_angle_1)
{
	//This returns an angle between 0 and 180.
	return 180 - abs(abs(get_degrees(i_angle_0) - get_degrees(i_angle_1)) - 180);
}

float deg_to_rad(const float i_degrees)
{
	return gbl::RAYCASTING::PI * get_degrees(i_degrees) / 180;
}

float get_degrees(const float i_degrees)
{
	//Give this function any angle (in degrees) and it converts it to an angle between 0 and 360.
	return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
}

float get_radians(const float i_radians)
{
	//Same as above but in radians.
	return static_cast<float>(fmod(2 * gbl::RAYCASTING::PI + fmod(i_radians, 2 * gbl::RAYCASTING::PI), 2 * gbl::RAYCASTING::PI));
}

float rad_difference(const float i_angle_0, const float i_angle_1)
{
	//This returns an angle between 0 and PI (I think, I don't know how radians work).
	return gbl::RAYCASTING::PI - abs(abs(get_radians(i_angle_0) - get_radians(i_angle_1)) - gbl::RAYCASTING::PI);
}

float rad_to_deg(const float i_radians)
{
	return 180 * get_radians(i_radians) / gbl::RAYCASTING::PI;
}

float Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
{
	sf::Vector2f diff = pos2 - pos1;
	return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

float DotProduct(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
{
	return vec1.x * vec2.x + vec2.y * vec2.y;
}

float Length(const sf::Vector2f& vec)
{
	return std::sqrt(DotProduct(vec, vec));
}
sf::Vector2f Normalize(const sf::Vector2f& vec)
{
	return vec / Length(vec);
}

sf::Vector2f TruncateLength(const sf::Vector2f& vec, float maxLength)
{
	float length = Length(vec);
	if (length > maxLength)
		return vec / length * maxLength;
	else
		return vec;
}