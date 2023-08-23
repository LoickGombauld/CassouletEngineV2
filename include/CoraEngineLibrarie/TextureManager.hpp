#pragma once
#include <CoraEngineLibrarie/Export.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>

enum CASSOULET_DLL TextureType { WallTexture, FloorTexture };


class CASSOULET_DLL TextureManager
{
public:

	TextureManager();
	TextureManager(std::string filePath);

	sf::Texture* GetTexture(int index);
	int TextureCount();
	sf::Texture* GetRandomTexture();


private:

	std::vector<sf::Texture> m_textures;
};
