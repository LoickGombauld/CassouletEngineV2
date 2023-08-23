#pragma once
#include <array>

#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Window.hpp>
#include  <CoraEngineLibrarie/Export.hpp>
#include  <CoraEngineLibrarie/Global.hpp>
#include  <CoraEngineLibrarie/TextureManager.hpp>
#include  <iostream>

class CASSOULET_DLL Player;

class CASSOULET_DLL Map
{
protected:
	std::array<std::array<Cell, yCase>, xCase> m_map;

public:
	Map();
	Cell GetCell(int y,int x);
	std::array<std::array<Cell, yCase>, xCase> GetHandle();
	CellType GetCellType(int x, int y);
	std::vector<Cell> m_walls;
	void SpawnPlayerOnMap(Player& player);
	void Draw(Window& renderWindow);
	void SetWallTexture(int index, sf::Texture* walltexture);

private:
	void GenerateMap();
	sf::Image m_image;
};