#include <array>
#include <CoraEngineLibrarie/ConvertMapSketch.h>


//Someday I'll use "Tiled Map Editor".
//But not today.
gbl::MAP::Map<> convert_map_sketch(sf::Image& map_sketch, entt::entity& i_playerEntity, entt::registry& registry,
	SpriteManager& i_sprite_manager, const  gbl::NPC::NpcData& soldier)
{
	auto& i_player = registry.get<gbl::Transform>(i_playerEntity);
	gbl::MAP::Map<> output_map{};
	for (unsigned char a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned char b = 0; b < gbl::MAP::ROWS; b++)
		{
			sf::Color pixel = map_sketch.getPixel(a, b);

			if (pixel != sf::Color::White && pixel != sf::Color(255, 0, 0))
			{
				const auto entity = registry.create();
				registry.emplace<gbl::Transform>(entity, sf::Vector2f(0, 0), sf::Vector2f(a, b), "Props");
				auto& collider = registry.emplace<gbl::Collider>(entity);
				registry.emplace<gbl::Animation>(entity);
					collider.range = 1;
				if (pixel == sf::Color(0, 0, 255))
				{
					collider.istrigger = true;
					registry.get<gbl::Transform>(entity).tag = "Enemy";
					registry.emplace<gbl::NPC::NpcData>(entity, soldier);
					registry.emplace<gbl::IA::Astar>(entity);
					registry.emplace<gbl::SpriteData>(entity, i_sprite_manager.get_sprite_data(soldier.name));
				}
				else if (pixel == sf::Color(182, 0, 0))
				{
					registry.emplace<gbl::PropData>(entity);
					auto& prop = registry.get<gbl::PropData>(entity);
					prop.is_animation = 0;
					prop.name = "BARREL";
					registry.emplace<gbl::SpriteData>(entity, i_sprite_manager.get_sprite_data(prop.name));
				}
				else if (pixel == sf::Color(255, 146, 0))
				{
					registry.emplace<gbl::PropData>(entity);
					auto& prop = registry.get<gbl::PropData>(entity);
					prop.is_animation = 1;
					prop.name = "FIRE_CAULDRON";
					registry.emplace<gbl::SpriteData>(entity, i_sprite_manager.get_sprite_data(prop.name));
				}
			}
			else if (pixel == sf::Color(255, 0, 0))
			{
				i_player.position = sf::Vector2f(a, b);
			}
			else if (pixel == sf::Color::White)
			{
				output_map[a][b] = gbl::MAP::Cell::Wall;
			}
			else
			{
				output_map[a][b] = gbl::MAP::Cell::Empty;
			}
		}
	}

	return output_map;
}