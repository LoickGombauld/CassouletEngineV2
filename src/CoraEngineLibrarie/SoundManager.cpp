#include <CoraEngineLibrarie/SoundManager.hpp>
#include <iostream>

std::unordered_map<std::string, sf::SoundBuffer> SoundManager::soundIds;


void SoundManager::LoadAudioFolders(const std::string& filePath)
{
	for (const auto& entry : std::filesystem::directory_iterator("Resources\\"+ filePath)) {
		if (".wav" == entry.path().extension()) {
			sf::SoundBuffer sfmliD;
			sfmliD.loadFromFile(entry.path().string());
			//Debug::Log(entry.path().filename().string() + " was added ");
			soundIds.insert({ entry.path().filename().string(), sfmliD });
		}
	}
}

void SoundManager::Init()
{
	LoadAudioFolders("Audio");
}

sf::Sound SoundManager::GetSound(const std::string& name)
{
	std::string SoundName =  name + ".wav";
	//check if the texture has been added
	if (soundIds.count(SoundName) == 0)
	{
		//Debug::Error("Trying to get a texture that has not been added. Name: " + name);
		return  sf::Sound( soundIds["Default_fx.wav"]);
	}

	//return the texture with that name
	return sf::Sound(soundIds.find(SoundName)->second);
}

