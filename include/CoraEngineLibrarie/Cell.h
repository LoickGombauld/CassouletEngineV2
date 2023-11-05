#pragma once
#include <SFML/Graphics.hpp>

enum CellType
{
	Empty,
	Wall,
	Random,
	Enemy,
	Win
};

class Cell {
public:
	Cell(sf::Vector2f& position, CellType type, sf::RectangleShape& box, sf::Texture* texture) : cellType(type), cellPosition(position), cellTexture(texture), cellBody(box)
	{
		cellBody.setTexture(texture);
	}
	Cell() : cellType(CellType::Empty), cellPosition(sf::Vector2f(0, 0)), cellTexture(nullptr)
	{
	}
	CellType cellType;
	sf::RectangleShape cellBody;
	sf::Vector2f cellPosition;
	sf::Texture* cellTexture;
};