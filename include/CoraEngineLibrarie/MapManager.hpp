#pragma once
#include <CoraEngineLibrarie/Export.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>

class CASSOULET_DLL MapManager
{
public:

	MapManager();
	MapManager(std::string filePath);

	sf::Image* GetLevel(int index);
	sf::Image* GetRandomLevel();


private:

	std::vector<sf::Image> m_maps;
};
