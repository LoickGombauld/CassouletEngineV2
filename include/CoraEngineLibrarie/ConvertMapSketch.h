#pragma once
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/SpriteManager.hpp>

gbl::MAP::Map<> convert_map_sketch(sf::Image& map_sketch, entt::entity& i_playerEntity, entt::registry& registry,
	SpriteManager& i_sprite_manager, const gbl::NPC::NpcData& soldier);