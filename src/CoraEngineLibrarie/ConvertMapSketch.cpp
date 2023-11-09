#include <array>
#include <CoraEngineLibrarie/ConvertMapSketch.h>


//Someday I'll use "Tiled Map Editor".
//But not today.
gbl::MAP::Map<> convert_map_sketch(sf::Image& map_sketch,std::vector<Prop>& i_Props, Player& i_player, std::vector<Npc>& i_npc, SpriteManager& i_sprite_manager)
{

	gbl::MAP::Map<> output_map{};

	for (unsigned char a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned char b = 0; b < gbl::MAP::ROWS; b++)
		{
			sf::Color pixel = map_sketch.getPixel(a, b);

			if (pixel == sf::Color::White)
			{
				output_map[a][b] = gbl::MAP::Cell::Wall;
			}
			else if (pixel == sf::Color(0, 0, 255))
			{
				i_npc.push_back(Npc(i_sprite_manager, a, b));
			}
			else if (pixel == sf::Color(182, 0, 0))
			{
				i_Props.push_back(Prop(0, "BARREL", i_sprite_manager, a, b));
			}
			else if (pixel == sf::Color(255, 0, 0))
			{
				i_player.set_position(a, b);
			}
			else if (pixel == sf::Color(255, 146, 0))
			{
				i_Props.push_back(Prop(1, "FIRE_CAULDRON", i_sprite_manager, a, b));
			}
			else
			{
				output_map[a][b] = gbl::MAP::Cell::Empty;
			}
		}
	}

	return output_map;
}