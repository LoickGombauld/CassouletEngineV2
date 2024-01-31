#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/SpriteManager.hpp>
#include <CoraEngineLibrarie/Export.hpp>

class CASSOULET_DLL NpcManager
{
	std::unordered_map<std::string, gbl::NPC::NpcData> npc_data;
	SpriteManager spriteManager;
public:
	NpcManager(SpriteManager& spritemanager);

	void load_Npc(const std::string& i_file_name);

	gbl::NPC::NpcData& get_Npc_data(const std::string& i_sprite_name);
};