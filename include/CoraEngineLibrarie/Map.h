#pragma once
#include <array>
#include  <iostream> 

#include <SFML/Graphics.hpp>
#include  <CoraEngineLibrarie/Export.hpp>
#include  <CoraEngineLibrarie/TextureManager.hpp>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/Cell.h>

class CASSOULET_DLL Player;

class CASSOULET_DLL Map
{

public:
	sf::Vector2f playerStarter;
	std::array<std::array<Cell, sizemaxY>, sizemaxX> GetHandle();
	Map(sf::Image&);
	Cell GetCell(int y, int x);
	CellType GetCellType(int x, int y);
	CellType GetCellType(sf::Vector2f& pos);
	bool ContainPoint(int x, int y, sf::Vector2f& point);
	std::vector<Cell> m_walls;
	void SpawnPlayerOnMap(Player& player);
	void Draw(sf::RenderWindow& renderWindow);
private:
	sf::Image mapImage;
	std::array<std::array<Cell, sizemaxY >, sizemaxX> mapArray;
};