#pragma once
#include <array>

#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Window.hpp>
#include  <CoraEngineLibrarie/Export.hpp>
#include  <CoraEngineLibrarie/Global.hpp>
#include  <CoraEngineLibrarie/TextureManager.hpp>
#include  <iostream>

#include <CoraEngineLibrarie/Fonts.hpp>

class CASSOULET_DLL Player;
class CASSOULET_DLL Entity;

class CASSOULET_DLL Map
{
protected:
	std::array<std::array<Cell, yCase>, xCase> m_map;
public:
	Map();
	Cell GetCell(int y,int x);
	std::array<std::array<Cell, yCase>, xCase> GetHandle();
	CellType GetCellType(int x, int y);
	CellType GetCellType(sf::Vector2f& pos);
	bool ContainPoint(int x, int y, sf::Vector2f& point);
	void SpawnPlayerOnMap(Player& player);
	void Draw(Window& renderWindow, sf::Texture* walltexture, sf::Texture* floortexture);
	void SetWallTexture(int index, sf::Texture* walltexture);
	Cell GetSpawnerCell(int index);
private:
	void GenerateMap();
	std::vector<Cell> m_walls;
	std::vector<Cell> m_EntitySpawner;
 	sf::Image m_image;

};