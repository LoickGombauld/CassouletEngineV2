#include <CoraEngineLibrarie/Map.h>

Cell Map::GetCell(int y, int x)
{
	return mapArray[x][y];
}


std::array<std::array<Cell, sizemaxY>, sizemaxX> Map::GetHandle()
{
	return mapArray;
}
CellType Map::GetCellType(int x, int y)
{
	return mapArray[x][y].cellType;
}

CellType Map::GetCellType(sf::Vector2f& pos)
{
	return mapArray[pos.x][pos.y].cellType;
}

bool Map::ContainPoint(int x, int y, sf::Vector2f& point)
{
	if (&mapArray[x][y].cellBody.getGlobalBounds() == nullptr)
	{
		return false;
	}
	else
	{
		return  mapArray[x][y].cellBody.getGlobalBounds().contains(point);
	}
}


Map::Map(sf::Image& map_sketch) : mapImage(map_sketch)
{
	float width_map = width / rate_map;
	float height_map = height / rate_map;
	for (unsigned char a = 0; a < map_sketch.getSize().x; a++)
	{
		for (unsigned char b = 0; b < map_sketch.getSize().y; b++)
		{
			sf::Color pixel = map_sketch.getPixel(a, b);

			auto cellPosition = sf::Vector2f(a * rate_map, b * rate_map);
			sf::RectangleShape box(sf::Vector2f(width_map / map_sketch.getSize().x, height_map / map_sketch.getSize().y));

			box.setPosition(cellPosition);
			if (pixel == sf::Color::White)
			{
				std::cout << "Wall Placed" << std::endl;
				mapArray[a][b] = Cell(cellPosition, Wall, box, TextureManager::GetTexture("Wall (1)"));
				m_walls.push_back(mapArray[a][b]);
			}
			else
			{
				mapArray[a][b] = Cell(cellPosition, Empty, box, TextureManager::GetTexture("dirtpres_0"));
			}
			if (pixel == sf::Color::Blue) {
				playerStarter = sf::Vector2f(a * rate_map, b * rate_map);
			}
		}
	}
}

void Map::Draw(sf::RenderWindow& renderWindow)
{
	for (unsigned char a = 0; a < mapImage.getSize().x; a++)
	{
		for (unsigned char b = 0; b < mapImage.getSize().y; b++)
		{
			auto shape = mapArray[a][b].cellBody;
			renderWindow.draw(shape);
		}
	}
}