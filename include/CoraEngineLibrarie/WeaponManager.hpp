#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/SpriteManager.hpp>
#include <CoraEngineLibrarie/Export.hpp>

class CASSOULET_DLL WeaponManager
{
	std::unordered_map<std::string, gbl::WEAPON::WeaponData> weapon_data;
	SpriteManager spriteManager;
public:
	WeaponManager(SpriteManager& spritemanager);

	void load_weapon(const std::string& i_file_name);

gbl::WEAPON::WeaponData& get_weapon_data(const std::string& i_sprite_name);
};