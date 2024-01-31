#include <filesystem>
#include <array>
#include <chrono>
#include <fstream>
#include <unordered_map>
#include <CoraEngineLibrarie/NpcManager.hpp>
#include <CoraEngineLibrarie/SoundManager.hpp>

NpcManager::NpcManager(SpriteManager& spritemanager)
{
	unsigned short current_texture = 0;

	//Adding new elements to the vector changes its memory address so we're resizing it then changing the existing ones.
	for (const std::filesystem::directory_entry file : std::filesystem::directory_iterator("Resources\\Data\\"))
	{
		if (".npc" == file.path().extension())
		{
			load_Npc(file.path().stem().string());
			break;
		}
	}
}

gbl::NPC::NpcData& NpcManager::get_Npc_data(const std::string& i_sprite_name)
{
	return npc_data.at(i_sprite_name);
}

void NpcManager::load_Npc(const std::string& i_file_name)
{
	unsigned char line_counter = 0;

	std::ifstream data_file("Resources\\Data\\" + i_file_name + ".npc");

	std::string line;
	std::string name;

	while (std::getline(data_file, line))
	{
		if (0 == line.size())
		{
			line_counter = 0;
		}
		else
		{
			switch (line_counter)
			{
			case 0:
			{
				name = line;

				npc_data[name].name = name;
				break;
			}
			case 1:
			{
				npc_data.at(name).damage = std::stoi(line);

				break;
			}
			case 2:
			{
				npc_data.at(name).fireDelay = std::stoi(line);

				break;
			}
			case 3:
			{
				npc_data.at(name).range = std::stoi(line);

				break;
			}
			case 4:
			{
				npc_data.at(name).knockBack = std::stoi(line);

				break;
			}
			case 5:
			{
				npc_data.at(name).fireSound = SoundManager::GetSound(line);
				break;
			}
			case 6: {
				npc_data.at(name).lifeMax = std::stoi(line);
				npc_data.at(name).life = std::stoi(line);
				break;
			}
			}
			line_counter++;
		}
	}
}
