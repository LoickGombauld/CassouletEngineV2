#pragma once
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <CoraEngineLibrarie/Constant.h>
#include <filesystem>
#include <CoraEngineLibrarie/Export.hpp>

class CASSOULET_DLL SoundManager
{
public:
	//add a new texture
	static void Init();
	static sf::Sound SoundManager::GetSound(const std::string& name);

private:
	//list of loaded textures
	static void LoadAudioFolders(const std::string& path);
	static std::unordered_map<std::string, sf::SoundBuffer> soundIds;
};
