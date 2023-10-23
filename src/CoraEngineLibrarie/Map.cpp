#include <CoraEngineLibrarie/Map.hpp>
#include  <CoraEngineLibrarie/Player.hpp>
#include  <CoraEngineLibrarie/Entity.hpp>


Map::Map()
{
	GenerateMap();
}

Cell Map::GetCell(int y, int x)
{
	return m_map[x][y];
}


std::array<std::array<Cell, yCase>, xCase> Map::GetHandle()
{
	return m_map;
}
CellType Map::GetCellType(int x, int y)
{
	return m_map[x][y].GetCellType();
}

CellType Map::GetCellType(sf::Vector2f& pos)
{
	return m_map[pos.x][pos.y].GetCellType();
}

bool Map::ContainPoint(int x, int y, sf::Vector2f& point)
{
	if (&m_map[x][y].CellBounds() == nullptr)
	{
		return false;
	}
	return  m_map[x][y].CellBounds().contains(point);
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
			sf::RectangleShape box(sf::Vector2f(blockSize, blockSize));

			box.setPosition(cellPosition);
			if (pixel == sf::Color(255, 255, 255))
			{
				std::cout << "Wall Placed" << std::endl;
				float height = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / HEIGHTMAX));;
				m_map[a][b] = Cell(cellPosition, Wall, box,height);
				m_walls.push_back(m_map[a][b]);
			}
			else
			{
				m_map[a][b] = Cell(cellPosition, Empty, box,0);
				if (pixel == sf::Color::Blue)
				{
					m_EntitySpawner.push_back(m_map[a][b]);
				}
			}
		}


	}
	m_image = map_sketch;
}

Cell Map::GetSpawnerCell(int index)
{
	if (index< 0)
	{
		index = 0;
	}
	else if(index> m_EntitySpawner.size())
	{
		index = m_EntitySpawner.size();
	}
	return m_EntitySpawner[index];
}


void Map::SetWallTexture(int index, sf::Texture* walltexture)
{
	m_walls[index].GetShape().setTexture(walltexture);
}



bool Map::CheckEntityOnCell(sf::Vector2i& pos , Entity& entity)
{
		int x =  entity.GetOriginPosition().x/ blockSize;
		int y=  entity.GetOriginPosition().y / blockSize;

		if (pos == sf::Vector2i(x,y))
		{
			return true;
		}
	
	return false;
}

void Map::Draw(Window& renderWindow, sf::Texture* walltexture, sf::Texture* floortexture)
{
	for (unsigned char a = 0; a < xCase; a++)
	{
		for (unsigned char b = 0; b < yCase; b++)
		{
			auto shape = m_map[a][b].GetShape();
			switch (m_map[a][b].GetCellType()) {
			case Empty:
				shape.setTexture(floortexture);

				break;
			case Wall:
				shape.setTexture(walltexture);
				break;
			case Win:

				break;
			default:;
			}
			renderWindow.GetHandle()->draw(shape);
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
				break;
			}
		}
	}
	if (!hasSpawned)
	{
		std::cout << "Aucun Spawner n'a �t� trouv� !";
	}
}



