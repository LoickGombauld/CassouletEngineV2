#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Constant.h>

gbl::MAP::PathCell get_cell(const sf::Vector2<short>& i_cell, const gbl::MAP::Map<gbl::MAP::PathCell>& i_map);