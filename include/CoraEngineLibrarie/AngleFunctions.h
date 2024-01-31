#pragma once
#include <SFML/Graphics.hpp>

float deg_difference(const float i_angle_0, const float i_angle_1);
float deg_to_rad(const float i_degrees);
float get_degrees(const float i_degrees);
float get_radians(const float i_radians);
float rad_difference(const float i_angle_0, const float i_angle_1);
float rad_to_deg(const float i_radians);
float Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
float DotProduct(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
float Length(const sf::Vector2f& vec);
sf::Vector2f Normalize(const sf::Vector2f& vec);
sf::Vector2f TruncateLength(const sf::Vector2f& vec, float maxLength);