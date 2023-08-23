#include <CoraEngineLibrarie/Map.hpp>
#include  <CoraEngineLibrarie/Player.hpp>



Map::Map()
{
	GenerateMap();
}

Cell Map::GetCell(int y, int x)
{
	return m_map[y][x];
}



std::array<std::array<Cell, yCase>, xCase> Map::GetHandle()
{
	return m_map;
}
CellType Map::GetCellType(int x, int y)
{
	return m_map[y][x].GetCellType();
}

void Map::GenerateMap()
{
	sf::Image map_sketch;
	bool mapisinvalid = true;
	while (mapisinvalid)
	{
		std::string levelName; std::cout << "Level Name ? : ";
		std::cin >> levelName;
		std::cout << std::endl;
		map_sketch.loadFromFile("Resources/Level/" + levelName + ".png");
		mapisinvalid = map_sketch.getPixelsPtr() == nullptr;
	}

	for (unsigned char a = 0; a < xCase; a++)
	{
		for (unsigned char b = 0; b < yCase; b++)
		{
			sf::Color pixel = map_sketch.getPixel(a, b);

			auto cellPosition = sf::Vector2f(a * blockSize, b * blockSize);
			if (pixel == sf::Color(255, 255, 255))
			{
				sf::RectangleShape box(sf::Vector2f(blockSize, blockSize));
				box.setPosition(cellPosition);
				std::cout << "Wall Placed" << std::endl;
				m_map[a][b] = Cell(cellPosition, Wall, box);
				m_walls.push_back(m_map[a][b]);
			}
			else
			{
				m_map[a][b] = Empty;
			}

		}

		m_image = map_sketch;
	}
}

void Map::SetWallTexture(int index, sf::Texture* walltexture)
{
	m_walls[index].GetShape().setTexture(walltexture);
}

void Map::Draw(Window& renderWindow)
{
	for (unsigned char a = 0; a < xCase; a++)
	{
		for (unsigned char b = 0; b < yCase; b++)
		{
			Shape box(m_map[a][b].GetShape());
			renderWindow.DrawRectShape(box);
		}
	}
}

void Map::SpawnPlayerOnMap(Player& player)
{
	bool hasSpawned = false;
	for (unsigned char a = 0; a < xCase; a++)
	{
		for (unsigned char b = 0; b < yCase; b++)
		{
			sf::Color pixel = m_image.getPixel(a, b);

			if (pixel == sf::Color(255, 0, 0) && !hasSpawned)
			{
				player.SetPosition(static_cast<float>(blockSize * a), static_cast<float>(blockSize * b));
				std::cout << "Player Placed" << std::endl;
				hasSpawned = true;
			}
		}
	}
	if (!hasSpawned)
	{
		std::cout << "Aucun Spawner n'a été trouvé !";
	}
}


