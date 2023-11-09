#include <CoraEngineLibrarie/TextureManager.hpp>
#include <iostream>

std::unordered_map<std::string, sf::Texture> TextureManager::textureIds;


void TextureManager::LoadTexturesFolders(const std::string& filePath)
{
	for (const auto& entry : std::filesystem::directory_iterator("Resources\\"+ filePath)) {
		if (".png" == entry.path().extension()) {
			sf::Texture sfmliD;
			sfmliD.loadFromFile(entry.path().string());
			//Debug::Log(entry.path().filename().string() + " was added ");
			textureIds.insert(std::pair(entry.path().filename().string(), sfmliD));
		}
	}
}

void TextureManager::Init()
{
	LoadTexturesFolders("Textures");
}

sf::Texture* TextureManager::GetTexture(const std::string& name)
{
	std::string textureName =  name + ".png";
	//check if the texture has been added
	if (textureIds.count(textureName) == 0)
	{
		//Debug::Error("Trying to get a texture that has not been added. Name: " + name);
		return  &textureIds["Default_Wall.png"];
	}

	//return the texture with that name
	return &textureIds.find(textureName)->second;
}

