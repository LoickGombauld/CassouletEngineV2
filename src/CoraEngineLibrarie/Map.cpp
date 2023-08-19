#include <CoraEngineLibrarie/Map.hpp>
#include  <CoraEngineLibrarie/Player.hpp>



Map::Map(Window& window) : m_renderWindow(window)
{
	GenerateMap();
}

Map::Map(Window& window, TextureManager wallTextures, TextureManager floorTextures) : m_renderWindow(window),
m_wallTexures(wallTextures), m_floorTexures(floorTextures)
{
	GenerateMap();
}

Map::Map(Window& window, TextureManager textures, TextureType textureType) : m_renderWindow(window)
{
	switch (textureType) {
	case WallTexture:
		m_wallTexures = textures;
		break;
	case FloorTexture:
		m_floorTexures = textures;
		break;
	default:;
	}
	GenerateMap();
}

TextureManager Map::GetTextureManager(TextureType Texuretype)
{
	switch (Texuretype) {
	case WallTexture:
		return m_wallTexures;
	case FloorTexture:
		return m_floorTexures;
	default:;

	}
}

void Map::GenerateMap()
{
	sf::Image map_sketch;

	while (map_sketch.getPixelsPtr() == nullptr)
	{
		std::string levelName; std::cout << "Level Name ? : ";
		std::cin >> levelName;
		std::cout << std::endl;
		map_sketch.loadFromFile("Resources/Level/" + levelName + ".png");
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
				if (HasTexures(WallTexture))
				{
					SetRandomShapeTexture(box, m_wallTexures);
				}
				box.setPosition(cellPosition);
				m_map[a][b] = Cell(cellPosition, CellType::Wall, box);
				std::cout << "Wall Placed" << std::endl;
			}
			//else if (pixel == sf::Color(0, 0, 255))
			//{
			//	i_steven.set_position(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
			//}
			else
			{
				sf::RectangleShape box(sf::Vector2f(blockSize, blockSize));
				if (HasTexures(FloorTexture))
				{
					SetRandomShapeTexture(box, m_floorTexures);
				}
				box.setFillColor(sf::Color::Blue);
				box.setPosition(cellPosition);
				m_map[a][b] = Cell(cellPosition, CellType::Empty, box);
			}

		}

		m_image = map_sketch;
	}
}

void Map::SetRandomShapeTexture(sf::Shape& box, TextureManager& textures)
{
	box.setTexture(textures.GetRandomTexture());
	box.setTextureRect(sf::IntRect({ 0,0 }, { 16 ,16 }));
}

bool Map::HasTexures(TextureType type)
{
	switch (type) {
	case WallTexture:
		if (m_wallTexures.TextureCount() == 0)
		{
			return false;
		}
		return  true;
	case FloorTexture:
		if (m_floorTexures.TextureCount() == 0)
		{
			return false;
		}
		return  true;
	default:;
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

void Map::Draw()
{
	for (unsigned char a = 0; a < xCase; a++)
	{
		for (unsigned char b = 0; b < yCase; b++)
		{
			Shape box(m_map[a][b].GetShape());
			m_renderWindow.DrawRectShape(box);
		}
	}
}

CellType Map::GetCellType(int x, int y)
{
	CheckXY(x, y);
	return m_map[y][x].GetCellType();
}


void Map::CheckXY(int& x, int& y)
{
	while (x > xCase || y > yCase)
	{
		if (x > xCase)
		{
			x /= blockSize;
		}
		if (y > yCase)
		{
			y /= blockSize;
		}
	}
}

Cell Map::GetCell(int x, int y)
{
	CheckXY(x, y);
	return m_map[y][x];
}

bool Map::ContainPoint(int x, int y, sf::Vector2f point)
{
	CheckXY(x, y);
	return  m_map[y][x].CellBounds().contains(point);
}


bool Map::CheckMapCase(unsigned int dx, unsigned int dy)
{
	return  m_map[dy][dx].GetCellType() == CellType::Wall;
}
