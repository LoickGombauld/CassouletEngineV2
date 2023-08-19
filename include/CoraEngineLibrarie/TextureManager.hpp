#pragma once
#include <CoraEngineLibrarie/Export.hpp>
#include <CoraEngineLibrarie/Global.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <filesystem>

class CASSOULET_DLL TextureManager
{
public:
	TextureManager(std::string filePath);

	sf::Texture* GetTexture(int index);
	int TextureCount();
	sf::Texture* GetRandomTexture();


private:

	std::vector<sf::Texture> m_textures;
};
