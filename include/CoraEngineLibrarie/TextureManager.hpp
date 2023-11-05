#pragma once
#include <CoraEngineLibrarie/Export.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <unordered_map>

class CASSOULET_DLL TextureManager
{
public:
	//add a new texture
	static void Init(std::string path);
	static sf::Texture* TextureManager::GetTexture(std::string name);

private:
	//list of loaded textures
	static std::unordered_map<std::string, sf::Texture> textureIds;
};
