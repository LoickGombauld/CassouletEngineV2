#pragma once
#include <array>

#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Window.hpp>
#include  <CoraEngineLibrarie/Export.hpp>
#include  <CoraEngineLibrarie/Global.hpp>
#include  <CoraEngineLibrarie/TextureManager.hpp>
#include  <iostream>

class CASSOULET_DLL Player;

enum CASSOULET_DLL TextureType { WallTexture, FloorTexture };

class CASSOULET_DLL Map
{
protected:
	Window& m_renderWindow;
	std::array<std::array<Cell, yCase>, xCase> m_map;

public:
	Map(Window& window);
	Map(Window& window, TextureManager wallTextures, TextureManager floorTextures);
	Map(Window& window, TextureManager textures,TextureType );
	void GenerateMap();
	TextureManager GetTextureManager(TextureType Texuretype);
	void SpawnPlayerOnMap(Player& player);
	void Draw();
	bool HasTexures(TextureType type);
	void SetRandomShapeTexture(sf::Shape& box, TextureManager& textures);
	CellType GetCellType(int x, int y);
	void CheckXY(int& x, int& y);
	Cell GetCell(int x, int y);
	bool ContainPoint(int x, int y, sf::Vector2f point);
	bool CheckMapCase(unsigned int dy, unsigned int dx);

private:
	sf::Image m_image;
	TextureManager m_floorTexures;
	TextureManager m_wallTexures ;
};