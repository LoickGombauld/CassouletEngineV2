#include <CoraEngineLibrarie/TextureManager.hpp>
#include <iostream>

std::unordered_map<std::string, sf::Texture> TextureManager::textureIds;


void TextureManager::Init(std::string filePath)
{
	for (const auto& entry : std::filesystem::directory_iterator(filePath)) {

		sf::Texture sfmliD;
		sfmliD.loadFromFile(entry.path().string());
		//Debug::Log(entry.path().filename().string() + " was added ");
		textureIds.insert(std::pair(entry.path().filename().string(), sfmliD));
	}
}

sf::Texture* TextureManager::GetTexture(std::string name)
{
	name += ".png";
	//check if the texture has been added
	if (textureIds.count(name) == 0)
	{
		//Debug::Error("Trying to get a texture that has not been added. Name: " + name);
		return  &textureIds["Default_Wall.png"];
	}

	//return the texture with that name
	return &textureIds.find(name)->second;
}

