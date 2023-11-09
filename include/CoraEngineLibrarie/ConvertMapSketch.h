#pragma once
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/Player.h>
#include <CoraEngineLibrarie/Prop.h>
#include <CoraEngineLibrarie/Npc.h>

gbl::MAP::Map<> convert_map_sketch(sf::Image& map_sketch, std::vector<Prop>& i_Props, Player& i_player, std::vector<Npc>& i_steven, SpriteManager& i_sprite_manager);