#pragma once
const int width = 600;
const int height = 600;
const float rayon = 5;
const int vitesse = 1; // Speed player
const int v_angle = 10; // Speed angle
const int angle_vision = 64; // FOV
const int precision_angle = 3; //equals to number of values in 1 degre


sf::ContextSettings options;

const int fps = 144;

const float rate_map = 5;

enum CellType
{
	Empty,
	Wall,
	Random,
	Enemy,
	Win
};