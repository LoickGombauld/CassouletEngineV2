#include <CoraEngineLibrarie/Game.h>
#include <CoraEngineLibrarie/ConvertMapSketch.h>
#include <CoraEngineLibrarie/AngleFunctions.h>
#include <CoraEngineLibrarie/SpriteManager.hpp>
#include <CoraEngineLibrarie/MapCollision.h>
#include <CoraEngineLibrarie/Astar.h>

entt::entity CreatePlayer(entt::registry& registry, sf::Vector2f& position);
void fill_map(gbl::MAP::Map<>& map, entt::registry& registry, entt::entity& entity);
//declare static fields
Game* Game::i;

Game* Game::I()
{
	return i;
}

Game::Game(entt::registry& registry) :
	show_map(1),
	hand_offset(-0.5f),
	window(sf::VideoMode(gbl::SCREEN::RESIZE* gbl::SCREEN::WIDTH, gbl::SCREEN::RESIZE* gbl::SCREEN::HEIGHT), "CoraEngine 1.3 (Ont fait du neuf avec du vieux) ", sf::Style::Close),
	fov_visualization(sf::TriangleFan, 1 + gbl::SCREEN::WIDTH),
	weapon_manager(sprite_manager),
	npc_manager(sprite_manager)
{
	i = this;
	playerEntity = CreatePlayer(registry, sf::Vector2f(0, 0));
	window.setMouseCursorVisible(0);
	window.setView(sf::View(sf::FloatRect(0, 0, gbl::SCREEN::WIDTH, gbl::SCREEN::HEIGHT)));
	window.setFramerateLimit(gbl::SCREEN::FPSMax);
	map = convert_map_sketch(*managerMap.GetLevel(0), playerEntity, registry, sprite_manager,
		Game::I()->GetNpcData()->get_Npc_data("SOLDIER"));
	auto& view = registry.view<gbl::NPC::NpcData>();
	for (auto entity : view) {
		fill_map(map, registry, entity);
	}
	for (Stripe& stripe : stripes)
	{
		stripe.set_sprite_manager(sprite_manager);
	}
}

entt::entity CreatePlayer(entt::registry& registry, sf::Vector2f& position) {
	entt::entity entity = registry.create();
	registry.emplace<gbl::Transform>(entity);
	auto& playerCollider = registry.emplace<gbl::Collider>(entity);
	registry.emplace<gbl::PLAYER::PlayerData>(entity);
	registry.emplace<gbl::Animation>(entity);
	registry.emplace <gbl::WEAPON::WeaponData>(entity, Game::I()->weapon_manager.get_weapon_data("WEAPON_2"));
	registry.emplace<gbl::SpriteData>(entity, Game::I()->sprite_manager.get_sprite_data("WEAPON_2"));
	auto& playerAnimation = registry.get<gbl::Animation>(entity);
	auto& playerSpriteData = registry.get<gbl::SpriteData>(entity);
	playerCollider.istrigger = true;
	playerCollider.range = 1;
	playerAnimation.animation_speed = gbl::SPRITES::FIRE_ANIMATION_SPEED;
	playerAnimation.ping_pong = 0;
	playerSpriteData.total_frames = playerSpriteData.total_frames;

	playerAnimation.animation_speed = std::fmod(playerAnimation.animation_speed, playerSpriteData.total_frames);
	playerSpriteData.frame = fmod(playerSpriteData.total_frames + fmod(playerSpriteData.frame, playerSpriteData.total_frames), playerSpriteData.total_frames);


	return entity;
}

int  Game::get_width(SpriteManager& sprite_manager, float distance, std::string name) const
{
	float sprite_height = sprite_manager.get_sprite_data(name).texture_box.height;
	float sprite_width = sprite_manager.get_sprite_data(name).texture_box.width;

	return round(gbl::SCREEN::HEIGHT * sprite_width / (distance * sprite_height * tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL))));
}

int Game::get_x( float distance, std::string name,gbl::NPC::NpcData& npcData) 
{
	return npcData.screen_x - round(0.5f * get_width(sprite_manager, npcData.distance, npcData.name));
}

int Game::get_y(gbl::NPC::NpcData& npcData)
{
	return round(0.5f * (gbl::SCREEN::HEIGHT - npcData.get_height()));
}


void Game::DrawAnimation(const sf::Vector2<short>& i_position, sf::RenderWindow& i_window, gbl::SpriteData& sprite_data, gbl::Animation& animation, const bool i_mirror_horizontal, const bool i_mirror_vertical, const float i_scale_x, const float i_scale_y, const sf::Color& i_color, const sf::Rect<unsigned short>& i_texture_box)
{
	unsigned short frame = floor(sprite_data.frame);

	//We're treating the ping pong animation as a regular one with twice as many frames to make things easier.
	if (1 == animation.ping_pong && frame >= sprite_data.total_frames)
	{
		frame -= 2 * (1 + frame - sprite_data.total_frames);
	}

	sprite_manager.draw_sprite(frame, sprite_data.name, i_position, i_window, i_mirror_horizontal, i_mirror_vertical, i_scale_x, i_scale_y, i_color, i_texture_box);
}


void PlayerTakeDamage()
{
	std::cout << "TakeDamage" << std::endl;
}

void fill_map(gbl::MAP::Map<>& map, entt::registry& registry, entt::entity& entity)
{
	auto& npcAstar = registry.get<gbl::IA::Astar>(entity);
	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			if (gbl::MAP::Cell::Empty != map[a][b])
			{
				npcAstar.astar_map[a][b] = gbl::MAP::PathCell::Wall;
			}
			else
			{
				npcAstar.astar_map[a][b] = gbl::MAP::PathCell::Empty;
			}
		}
	}
}

void UpdateNpc(Game* game, entt::registry& registry, entt::entity& npc) {
	auto& player = game->playerEntity;
	auto& playerPosition = registry.get<gbl::Transform>(player).position;
	auto& playerdirection = registry.get<gbl::Transform>(player).direction;
	auto& playerCollider = registry.get<gbl::Collider>(player);
	auto& npcdata = registry.get<gbl::NPC::NpcData>(npc);
	auto& npcSprite = registry.get<gbl::SpriteData>(npc);
	auto& npcAstar = registry.get<gbl::IA::Astar>(npc);
	auto& npcTransform = registry.get<gbl::Transform>(npc);
	auto& npcCollider = registry.get<gbl::Collider>(npc);
	if (1 == game->window.hasFocus())
	{
		float angle = get_radians(atan2(playerPosition.y - npcTransform.position.y, npcTransform.position.x - playerPosition.x));
		float difference = deg_difference(playerdirection.x, rad_to_deg(angle));
		float frame_angle = 360.f / game->sprite_manager.get_sprite_data(npcdata.name).total_frames;
		float shifted_direction;
		float speed = 0;
		float step_x = 0;
		float step_y = 0;

		//We're taking the cells that are closest to Steven and the player as starting and finishing cells.
		sf::Vector2<unsigned short> finish_position(round(playerPosition.x), round(playerPosition.y));
		sf::Vector2<unsigned short> start_position(round(npcTransform.position.x), round(npcTransform.position.y));
		switch (npcdata.currentState)
		{
		case gbl::IA::Waiting:
			break;
		case gbl::IA::Partrol:
			break;
		case gbl::IA::FollowPlayer:

			if (finish_position != start_position)
			{
				astar_reset(npcAstar.astar_path_length, npcAstar.astar_previous_cell,
					npcAstar.astar_path_vector, npcAstar.astar_f_scores, npcAstar.astar_g_scores,
					npcAstar.astar_h_scores, finish_position, start_position, npcAstar.astar_map);
				astar_search(npcAstar.astar_path_length, npcAstar.astar_previous_cell,
					npcAstar.astar_path_vector, npcAstar.astar_f_scores,
					npcAstar.astar_g_scores, npcAstar.astar_h_scores, npcAstar.next_cell, finish_position,
					start_position, npcAstar.astar_map);

				npcTransform.direction.x = rad_to_deg(atan2(npcTransform.position.y - npcAstar.next_cell.y,
					npcAstar.next_cell.x - npcTransform.position.x));

				//The farther Steven is from the player, the faster he moves, and vice versa.
				if (gbl::NPC::MAX_MOVEMENT_DISTANCE < npcAstar.astar_path_length)
				{
					speed = gbl::NPC::MAX_MOVEMENT_SPEED;
				}
				else if (gbl::NPC::MIN_MOVEMENT_DISTANCE > npcAstar.astar_path_length)
				{
					speed = gbl::NPC::MIN_MOVEMENT_SPEED;
				}
				else
				{
					speed = gbl::NPC::MIN_MOVEMENT_SPEED + (gbl::NPC::MAX_MOVEMENT_SPEED - gbl::NPC::MIN_MOVEMENT_SPEED) *
						(npcAstar.astar_path_length - gbl::NPC::MIN_MOVEMENT_DISTANCE) /
						(gbl::NPC::MAX_MOVEMENT_DISTANCE - gbl::NPC::MIN_MOVEMENT_DISTANCE);
				}
			}
			break;
		default:
			break;
		}

		// si Npc rentre en contact avec le joueur
		if (npcCollider.OnCollision(playerPosition,playerCollider,npcTransform))
		{
			PlayerTakeDamage();
		}
		//Npc moves to the next cell in the A star path.
		if (npcAstar.next_cell.x < npcTransform.position.x)
		{
			step_x = std::max(-speed, npcAstar.next_cell.x - npcTransform.position.x);
		}
		else if (npcAstar.next_cell.x > npcTransform.position.x)
		{
			step_x = std::min(speed, npcAstar.next_cell.x - npcTransform.position.x);
		}

		if (npcAstar.next_cell.y < npcTransform.position.y)
		{
			step_y = std::max(-speed, npcAstar.next_cell.y - npcTransform.position.y);
		}
		else if (npcAstar.next_cell.y > npcTransform.position.y)
		{
			step_y = std::min(speed, npcAstar.next_cell.y - npcTransform.position.y);
		}

		if (0 == map_collision(step_x + npcTransform.position.x, step_y + npcTransform.position.y, game->map))
		{
			npcTransform.position.x += step_x;
			npcTransform.position.y += step_y;
		}
		else if (0 == map_collision(step_x + npcTransform.position.x, npcTransform.position.y, game->map))
		{
			npcTransform.position.x += step_x;
			npcTransform.position.y = round(npcTransform.position.y);
		}
		else if (0 == map_collision(npcTransform.position.x, step_y + npcTransform.position.y, game->map))
		{
			npcTransform.position.x = round(npcTransform.position.x);
			npcTransform.position.y += step_y;
		}
		else
		{
			npcTransform.position.x = round(npcTransform.position.x);
			npcTransform.position.y = round(npcTransform.position.y);
		}

		//This makes it so that the difference is between -180 to 180.
		if (deg_difference(playerdirection.x, difference + rad_to_deg(angle)) < deg_difference(playerdirection.x, rad_to_deg(angle) - difference))
		{
			difference *= -1;
		}

		shifted_direction = get_degrees(90 + get_degrees(npcTransform.direction.x + 0.5f * frame_angle) - difference - playerdirection.x);

		//Calculating the perpendicular distance from Steven to the player.
		npcdata.distance = abs(playerPosition.y - npcTransform.position.y - tan(deg_to_rad(playerdirection.x - 90))
			* (npcTransform.position.x - playerPosition.x)) / sqrt(1 + pow(tan(deg_to_rad(playerdirection.x - 90)), 2));

		npcdata.screen_x = round(0.5f * gbl::SCREEN::WIDTH * (1 - tan(deg_to_rad(difference)) / tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL))));

		npcdata.in_the_view = 90 > abs(difference);

		npcSprite.frame = floor(shifted_direction / frame_angle);
	}
}

void UpdateProp(Game* game, entt::registry& registry, entt::entity& npc) {
	auto& player = game->playerEntity;
	auto& playerPosition = registry.get<gbl::Transform>(player).position;
	auto& playerdirection = registry.get<gbl::Transform>(player).direction;
	auto& playerdata = registry.get<gbl::NPC::NpcData>(player);
	auto& playerSprite = registry.get<gbl::SpriteData>(player);
	auto& npcTransform = registry.get<gbl::Transform>(npc);
}

void UpdatePlayer(float deltaTime, Game* game, entt::registry& registry) {

	auto& playerdata = registry.get<gbl::PLAYER::PlayerData>(game->playerEntity);
	auto& direction = registry.get<gbl::Transform>(game->playerEntity).direction;
	auto& position = registry.get<gbl::Transform>(game->playerEntity).position;
	if (1 == game->window.hasFocus())
	{
		float rotation_horizontal = 0;
		float rotation_vertical = 0;
		float step_x = 0;
		float step_y = 0;
		unsigned short window_center_x = static_cast<unsigned short>(round(0.5f * game->window.getSize().x));
		unsigned short window_center_y = static_cast<unsigned short>(round(0.5f * game->window.getSize().y));

		rotation_horizontal = gbl::RAYCASTING::FOV_HORIZONTAL * (window_center_x - sf::Mouse::getPosition(game->window).x) / game->window.getSize().x;
		rotation_vertical = gbl::RAYCASTING::FOV_VERTICAL * (window_center_y - sf::Mouse::getPosition(game->window).y) / game->window.getSize().y;

		direction.x = get_degrees(direction.x + rotation_horizontal);
		direction.y = std::clamp<float>(direction.y + rotation_vertical, -gbl::RAYCASTING::MAX_VERTICAL_DIRECTION, gbl::RAYCASTING::MAX_VERTICAL_DIRECTION);

		sf::Mouse::setPosition(sf::Vector2i(window_center_x, window_center_y), game->window);
		bool hasMove = sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);

		if (hasMove)
		{
			playerdata.speed += deltaTime;
		}
		else
		{
			playerdata.speed -= deltaTime;
		}
		playerdata.speed = std::clamp(playerdata.speed, 0.f, playerdata.speedMax);
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			step_x += playerdata.speed * cos(deg_to_rad(90 + direction.x));
			step_y += -playerdata.speed * sin(deg_to_rad(90 + direction.x));

		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			step_x += playerdata.speed * cos(deg_to_rad(direction.x - 90));
			step_y += -playerdata.speed * sin(deg_to_rad(direction.x - 90));

		}
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			step_x -= playerdata.speed * cos(deg_to_rad(direction.x));
			step_y += playerdata.speed * sin(deg_to_rad(direction.x));
		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			step_x += playerdata.speed * cos(deg_to_rad(direction.x));
			step_y -= playerdata.speed * sin(deg_to_rad(direction.x));
		}
		if (!hasMove)
		{
			playerdata.velocity.x *= 0.90f - playerdata.friction;
			playerdata.velocity.y *= 0.90f - playerdata.friction;
			playerdata.speed *= 0.90f - playerdata.friction;
			step_x = playerdata.velocity.x;
			step_y = playerdata.velocity.y;
		}

		std::cout << "playerdata.velocity : " << step_x << ";" << step_y << std::endl;
		std::cout << "Speed : " << playerdata.speed << std::endl;
		playerdata.velocity = sf::Vector2f(step_x, step_y);
		if (0 == map_collision(step_x + position.x, step_y + position.y, game->map))
		{
			position.x += step_x;
			position.y += step_y;
		}
		else if (0 == map_collision(step_x + position.x, position.y, game->map))
		{
			position.x += step_x;
			position.y = round(position.y);
		}
		else if (0 == map_collision(position.x, step_y + position.y, game->map))
		{
			position.x = round(position.x);
			position.y += step_y;
		}
		else
		{
			position.x = round(position.x);
			position.y = round(position.y);
		}
		playerdata.lastfire += deltaTime;

		auto& weapon = registry.get < gbl::WEAPON::WeaponData >(game->playerEntity);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && playerdata.lastfire > weapon.fireDelay)
		{
			weapon.fireSound.play();
			playerdata.lastfire = 0;
			std::cout << "fire !" << std::endl;
		}
	}
}

void DrawNpc(entt::registry& registry, entt::entity& npc, Game* game, const short i_pitch, sf::RenderWindow& i_window) {
	auto& npcData = registry.get<gbl::NPC::NpcData>(npc);

	auto& npcSpriteData = registry.get<gbl::SpriteData>(npc);
	float sprite_height = game->sprite_manager.get_sprite_data(npcData.name).texture_box.height;
	float sprite_width = game->sprite_manager.get_sprite_data(npcData.name).texture_box.width;

	unsigned char shade = 255 * std::clamp<float>(1 - npcData.distance / gbl::RAYCASTING::RENDER_DISTANCE, 0, 1);

	npcData.in_the_view &= gbl::SCREEN::HEIGHT > i_pitch + game->get_y(npcData) &&
		gbl::SCREEN::WIDTH > game->get_x(npcData.distance,npcData.name,npcData) && game->get_x(npcData.distance, npcData.name, npcData) > -1 * game->get_width(game->sprite_manager, npcData.distance, npcData.name)
		&& i_pitch + game->get_y(npcData) > -1 * npcData.get_height();

	if (0 < shade && 1 == npcData.in_the_view)
	{
		game->sprite_manager.draw_sprite(npcSpriteData.frame, npcData.name,
			sf::Vector2<short>(game->get_x(npcData.distance, npcData.name, npcData), i_pitch + game->get_y(npcData)),
			i_window, 0, 0, game->get_width(game->sprite_manager, npcData.distance, npcData.name) / sprite_width,
			npcData.get_height() / sprite_height, sf::Color(shade, shade, shade));
	}
}

void DrawProp(entt::registry& registry, entt::entity& prop, Game* game, const short i_pitch, sf::RenderWindow& i_window)
{
	auto& propData = registry.get<gbl::PropData>(prop);
	auto& propTransform = registry.get<gbl::Transform>(prop);
	auto& propSpriteData = registry.get<gbl::SpriteData>(prop);
	auto& propAnimation = registry.get<gbl::Animation>(prop);
	float sprite_height = game->sprite_manager.get_sprite_data(propData.name).texture_box.height;
	float sprite_width = game->sprite_manager.get_sprite_data(propData.name).texture_box.width;

	unsigned char shade = 255 * std::clamp<float>(1 - propData.distance / gbl::RAYCASTING::RENDER_DISTANCE, 0, 1);

	propData.in_the_view &= gbl::SCREEN::HEIGHT > i_pitch + propTransform.position.y && gbl::SCREEN::WIDTH > propTransform.position.x && propTransform.position.x > -1 * Game::I()->get_width(game->sprite_manager, propData.distance, propData.name)
		&& i_pitch + propTransform.position.y > -1 * propData.get_height();
	if (0 < shade && 1 == propData.in_the_view)
	{
		if (0 == propData.is_animation)
		{
			game->sprite_manager.draw_sprite(0, propData.name, sf::Vector2<short>(propTransform.position.x,
				i_pitch + propTransform.position.y), i_window, 0, 0, Game::I()->get_width(game->sprite_manager,
					propData.distance, propData.name) / sprite_width, propData.get_height() / sprite_height,
				sf::Color(shade, shade, shade));
		}
		else
		{
			game->DrawAnimation(sf::Vector2<short>(propTransform.position.x, i_pitch + propTransform.position.y), i_window, propSpriteData, propAnimation, 0, 0, Game::I()->get_width(game->sprite_manager, propData.distance, propData.name) / sprite_width, propData.get_height() / sprite_height,
				sf::Color(shade, shade, shade));
		}
	}
}



void Game::GameOver()
{
}

bool Game::is_open() const
{
	return window.isOpen();
}

void Game::calculate_fov_visualization(entt::registry& registry)
{
	auto& player = registry.get<gbl::Transform>(playerEntity);
	float start_x = 0.5f + player.position.x;
	float start_y = 0.5f + player.position.y;

	fov_visualization[0].position = sf::Vector2f(gbl::MAP::CELL_SIZE * start_x, gbl::MAP::CELL_SIZE * start_y);

	for (unsigned short a = 0; a < gbl::SCREEN::WIDTH; a++)
	{
		fov_visualization[1 + a].position = sf::Vector2f(gbl::MAP::CELL_SIZE * (start_x + stripes[a].get_true_distance() * cos(stripes[a].get_angle())), gbl::MAP::CELL_SIZE * (start_y - stripes[a].get_true_distance() * sin(stripes[a].get_angle())));
	}
}

void Game::Render(entt::registry& registry)
{
	std::vector<bool> npc_is_drawn;
	auto& viewNpc = registry.view<gbl::NPC::NpcData>();
	auto& viewProp = registry.view<gbl::PropData>();
	auto& player = registry.get<gbl::Transform>(playerEntity);
	for (auto id : viewNpc) {
		npc_is_drawn.push_back(0);
	}
	//I believe that by changing this, you can change your height in the game.
	//You can use that if you wanna add jumping or crouching.

	float camera_z = 0.5f * gbl::SCREEN::HEIGHT;
	float end_stripe_x = tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL)) * (1 - 2.f / gbl::SCREEN::WIDTH);
	float hand_offset_x = gbl::SPRITES::HAND_OFFSET_MAX * cos(gbl::RAYCASTING::PI * hand_offset);
	float hand_offset_y = 0.5f * gbl::SPRITES::HAND_OFFSET_MAX * sin(gbl::RAYCASTING::PI * abs(hand_offset));
	float ray_direction_end_x;
	float ray_direction_end_y;
	float ray_direction_start_x;
	float ray_direction_start_y;
	float start_stripe_x = -tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL));
	float vertical_fov_ratio = tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_VERTICAL));

	short pitch = round(0.5f * gbl::SCREEN::HEIGHT * tan(deg_to_rad(player.direction.y)));

	unsigned short floor_start_y = std::clamp<float>(pitch + 0.5f * gbl::SCREEN::HEIGHT, 0, gbl::SCREEN::HEIGHT);
	unsigned short hand_texture_height = gbl::SPRITES::HAND_SCALE * sprite_manager.get_sprite_data("WEAPON_1").texture_box.height;
	unsigned short hand_texture_width = gbl::SPRITES::HAND_SCALE * sprite_manager.get_sprite_data("WEAPON_1").texture_box.width;

	gbl::SpriteData floor_sprite_data = sprite_manager.get_sprite_data("WALL_DESTROY_1");

	sf::Image floor_image;
	floor_image.loadFromFile(floor_sprite_data.image_location);

	//We're gonna draw the floor in this buffer image because the floor is drawn pixel by pixel.
	//I tried drawing directly on the screen. I stopped when my PC started cooking eggs.
	sf::Image floor_buffer_image;
	floor_buffer_image.create(gbl::SCREEN::WIDTH, gbl::SCREEN::HEIGHT - floor_start_y);

	sf::Sprite floor_sprite;
	floor_sprite.setPosition(0, floor_start_y);

	sf::Texture floor_texture;

	ray_direction_end_x = cos(deg_to_rad(player.direction.x)) + end_stripe_x * cos(deg_to_rad(player.direction.x - 90));
	ray_direction_end_y = -sin(deg_to_rad(player.direction.x)) - end_stripe_x * sin(deg_to_rad(player.direction.x - 90));
	ray_direction_start_x = cos(deg_to_rad(player.direction.x)) + start_stripe_x * cos(deg_to_rad(player.direction.x - 90));
	ray_direction_start_y = -sin(deg_to_rad(player.direction.x)) - start_stripe_x * sin(deg_to_rad(player.direction.x - 90));

	window.clear();

	//Drawing the floor.
	//For some reason, this part hits heavily on the performance.
	//I couldn't find a way to optimize it, though.
	for (unsigned short a = floor_start_y; a < gbl::SCREEN::HEIGHT; a++)
	{
		float floor_step_x;
		float floor_step_y;
		float floor_x;
		float floor_y;
		float row_distance;

		//We're drawing the floor row by row from top to bottom.
		short row_y = a - pitch - 0.5f * gbl::SCREEN::HEIGHT;

		unsigned char shade;

		//Distance from the player to the current row we're drawing.
		row_distance = (0 == row_y) ? FLT_MAX : camera_z / (row_y * vertical_fov_ratio);

		shade = 255 * std::clamp<float>(1 - row_distance / gbl::RAYCASTING::RENDER_DISTANCE, 0, 1);

		if (0 < shade)
		{
			floor_step_x = row_distance * (ray_direction_end_x - ray_direction_start_x) / gbl::SCREEN::WIDTH;
			floor_step_y = row_distance * (ray_direction_end_y - ray_direction_start_y) / gbl::SCREEN::WIDTH;
			//You can get the position of the current floor cell we're drawing using these variables (floor(floor_x) and floor(floor_y)).
			//Then you'll be able to draw different floor textures.
			floor_x = 0.5f + player.position.x + ray_direction_start_x * row_distance;
			floor_y = 0.5f + player.position.y + ray_direction_start_y * row_distance;

			for (unsigned short b = 0; b < gbl::SCREEN::WIDTH; b++)
			{
				sf::Color floor_image_pixel = floor_image.getPixel(floor(floor_sprite_data.texture_box.width * (floor_sprite_data.frame + floor_x - floor(floor_x))), floor(floor_sprite_data.texture_box.height * (floor_y - floor(floor_y))));

				floor_x += floor_step_x;
				floor_y += floor_step_y;

				floor_image_pixel *= sf::Color(shade, shade, shade);

				floor_buffer_image.setPixel(b, a - floor_start_y, floor_image_pixel);
			}
		}
	}

	floor_texture.loadFromImage(floor_buffer_image);

	floor_sprite.setTexture(floor_texture);
	window.draw(floor_sprite);

	calculate_fov_visualization(registry);

	//std::sort(props.begin(), props.end(), std::greater());
	std::sort(stripes.begin(), stripes.end(), std::greater());


	//Drawing things layer by layer.
	for (Stripe& stripe : stripes)
	{
				int index = 0;
		for (auto propEntity : viewProp)
		{
			auto& currentProp = viewProp.get<gbl::PropData>(propEntity);

			if (stripe.get_distance() < currentProp.distance)
			{
				//I'm repeating the same code 3 times.
				//I had 2 options:
				//1) Spending hours trying to figure out the efficient way of doing this.
				//2) Writing the same code 3 times and tolerating people making fun of me.
				for (auto
 npcEntity : viewNpc) {

					if (0 == npc_is_drawn[index])
					{

						npc_is_drawn[index] = 1;
						DrawNpc(registry, npcEntity, this, pitch, window);
					}
					index++;
				}

				DrawProp(registry, propEntity, this, pitch, window);

			}
		}
		for (auto npcEntity : viewNpc)
		{
			auto& npcdata = registry.get<gbl::NPC::NpcData>(npcEntity);
			if (0 == npc_is_drawn[index])
			{
				if (npcdata.distance > stripe.get_distance())
				{
					npc_is_drawn[index] = 1;

					DrawNpc(registry, npcEntity, this, pitch, window);
				}
			}
			index++;
		}
			stripe.set_height(hand_offset);
			stripe.draw(pitch, window);
	}

	for (auto propEntity : viewProp)
	{
		auto& currentProp = viewProp.get<gbl::PropData>(propEntity);
		int index = 0;
		for (auto npcEntity : viewNpc) {
			auto& npcdata = registry.get<gbl::NPC::NpcData>(npcEntity);
			if (0 == npc_is_drawn[index])
			{
				if (npcdata.distance > currentProp.distance)
				{
					npc_is_drawn[index] = 1;

					DrawNpc(registry, npcEntity, this, pitch, window);
				}
			}
			index++;
		}

		DrawProp(registry, propEntity, this, pitch, window);
	}
	int index = 0;
	for (auto npcEntity : viewNpc) {

		if (0 == npc_is_drawn[index])
		{
			DrawNpc(registry, npcEntity, this, pitch, window);
		}
		index++;
	}

	if (1 == show_map)
	{
		draw_map(registry);
	}

	DrawAnimation(sf::Vector2<short>((gbl::SCREEN::WIDTH + hand_offset_x - hand_texture_width) / 2, (gbl::SCREEN::HEIGHT + hand_offset_y - hand_texture_height) * 2.75f),
		window, registry.get<gbl::SpriteData>(playerEntity),
		hand_animation, 0, 0, gbl::SPRITES::HAND_SCALE,
		gbl::SPRITES::HAND_SCALE);

	window.display();

	//else
	//{
	//	short screamer_x = 0.5f * (gbl::SCREEN::WIDTH - gbl::STEVEN::SCREAMER_RESIZE * sprite_manager.get_sprite_data("STEVEN_SCREAMER").texture_box.width);
	//	short screamer_y = gbl::STEVEN::SCREAMER_Y;

	//	//By the end of the project I became so lazy I didn't wanna bother working with the random library so I used rand().
	//	screamer_x += rand() % (1 + 2 * gbl::STEVEN::SCREAMER_MAX_OFFSET) - gbl::STEVEN::SCREAMER_MAX_OFFSET;
	//	screamer_y += rand() % (1 + 2 * gbl::STEVEN::SCREAMER_MAX_OFFSET) - gbl::STEVEN::SCREAMER_MAX_OFFSET;

	//	window.clear();

	//	sprite_manager.draw_sprite(0, "STEVEN_SCREAMER", sf::Vector2<short>(screamer_x, screamer_y), window, 0, 0, gbl::STEVEN::SCREAMER_RESIZE, gbl::STEVEN::SCREAMER_RESIZE);

	//	window.display();
	//}
}

void Game::draw_map(entt::registry& registry)
{
	auto& player = registry.get<gbl::Transform>(playerEntity);
	float frame_angle = 360.f / sprite_manager.get_sprite_data("MAP_PLAYER").total_frames;
	float shifted_direction = get_degrees(player.direction.x + 0.5f * frame_angle);
	auto& viewNpc = registry.view<gbl::NPC::NpcData>();
	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			switch (map[a][b])
			{
			case gbl::MAP::Cell::Empty:
			{
				sprite_manager.draw_sprite(0, "MAP_CELL", sf::Vector2<short>(gbl::MAP::CELL_SIZE * a, gbl::MAP::CELL_SIZE * b), window);

				break;
			}
			default:
			{
				sprite_manager.draw_sprite(0, "MAP_WALL", sf::Vector2<short>(gbl::MAP::CELL_SIZE * a, gbl::MAP::CELL_SIZE * b), window);
			}
			}
		}
	}

	window.draw(fov_visualization);

	for (auto npcEntity : viewNpc) {
		auto& npcTransform = registry.get<gbl::Transform>(npcEntity);
		float steven_shifted_direction = get_degrees(npcTransform.direction.x + 0.5f * frame_angle);
		sprite_manager.draw_sprite(floor(steven_shifted_direction / frame_angle), "MAP_NPC", sf::Vector2<short>(round(gbl::MAP::CELL_SIZE * npcTransform.position.x), round(gbl::MAP::CELL_SIZE * npcTransform.position.y)), window);
	}
	sprite_manager.draw_sprite(floor(shifted_direction / frame_angle), "MAP_PLAYER", sf::Vector2<short>(round(gbl::MAP::CELL_SIZE * player.position.x), round(gbl::MAP::CELL_SIZE * player.position.y)), window);
}

void Game::handle_events()
{
	while (1 == window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			window.close();

			break;
		}
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::M:
			{
				//I hate using boolean operators but if you like them, uncomment the next line.
				//show_map = !show_map;
				show_map = 1 - show_map;
			}
			}
		}
		}
	}
}

void Game::GameRaycast(entt::registry& registry)
{
	auto& player = registry.get<gbl::Transform>(playerEntity);
	float ray_start_x = 0.5f + player.position.x;
	float ray_start_y = 0.5f + player.position.y;

	//It's almost impossible to explain everything here without visuals.
	//But I'll try my best.
	for (unsigned short a = 0; a < gbl::SCREEN::WIDTH; a++)
	{
		char cell_step_x = 0;
		char cell_step_y = 0;

		//Current stripe's x-coordinate relative to the direction of the player.
		float current_stripe_x = tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL)) * (2 * a / static_cast<float>(gbl::SCREEN::WIDTH) - 1);
		//The angle between the player's direction and the direction to the current stripe.
		float ray_angle;
		//Ray's direction represented in coordinates (or distances. I don't know how to say it correctly).
		//I wanna say "vector" but I know some people will say "ACKULUQIDARTUALLY!!!!! A vector has a DIRECTION and a MAGNITUDE!" so I won't.
		float ray_direction_x = cos(deg_to_rad(player.direction.x)) + current_stripe_x * cos(deg_to_rad(player.direction.x - 90));
		float ray_direction_y = -sin(deg_to_rad(player.direction.x)) - current_stripe_x * sin(deg_to_rad(player.direction.x - 90));
		float side_x;
		//This ray checks for horizontal collisions.
		float x_ray_length = 0;
		float x_ray_unit_length = (0 == ray_direction_x) ? FLT_MAX : std::abs(1 / ray_direction_x);
		//This ray checks for vertical collisions.
		float y_ray_length = 0;
		float y_ray_unit_length = (0 == ray_direction_y) ? FLT_MAX : std::abs(1 / ray_direction_y);

		unsigned char current_cell_x = floor(ray_start_x);
		unsigned char current_cell_y = floor(ray_start_y);
		//The side of the wall.
		//When you're looking at the map from above.
		//0 - Left side.
		//1 - Top side.
		//2 - Right side.
		//3 - Bottom side.
		unsigned char side = 0;

		gbl::MAP::Cell collision_cell;

		//In order for the algorithm to work, the ray must start at the cell borders.
		//So if the starting position of the ray is not a cell border (which is very likely), we'll stretch it to the closest one.
		if (0 > ray_direction_x)
		{
			cell_step_x = -1;

			x_ray_length = x_ray_unit_length * (ray_start_x - current_cell_x);
		}
		else if (0 < ray_direction_x)
		{
			cell_step_x = 1;

			x_ray_length = x_ray_unit_length * (1 + current_cell_x - ray_start_x);
		}

		if (0 > ray_direction_y)
		{
			cell_step_y = -1;

			y_ray_length = y_ray_unit_length * (ray_start_y - current_cell_y);
		}
		else if (0 < ray_direction_y)
		{
			cell_step_y = 1;

			y_ray_length = y_ray_unit_length * (1 + current_cell_y - ray_start_y);
		}

		while (1)
		{
			//In case the ray hits a corner.
			//I'm a perfectionist (i. e. stupid).
			bool corner_collision = 0;

			//We stretch the shortest ray.
			if (x_ray_length < y_ray_length)
			{
				stripes[a].set_distance(x_ray_length);

				x_ray_length += x_ray_unit_length;

				current_cell_x += cell_step_x;

				if (1 == cell_step_x)
				{
					side = 2;
				}
				else
				{
					side = 0;
				}
			}
			else if (x_ray_length > y_ray_length)
			{
				stripes[a].set_distance(y_ray_length);

				y_ray_length += y_ray_unit_length;

				current_cell_y += cell_step_y;

				if (-1 == cell_step_y)
				{
					side = 3;
				}
				else
				{
					side = 1;
				}
			}
			else //If the rays are equal, that means we hit the corner, so we stretch both rays.
			{
				corner_collision = 1;

				stripes[a].set_distance(x_ray_length);

				x_ray_length += x_ray_unit_length;
				y_ray_length += y_ray_unit_length;

				current_cell_x += cell_step_x;
				current_cell_y += cell_step_y;

				if (1 == cell_step_x)
				{
					side = 2;
				}
				else
				{
					side = 0;
				}
			}

			//Making sure the current cell we're checking is inside our map.
			if (0 <= current_cell_x && 0 <= current_cell_y && gbl::MAP::COLUMNS > current_cell_x && gbl::MAP::ROWS > current_cell_y)
			{
				if (gbl::MAP::Cell::Empty != map[current_cell_x][current_cell_y])
				{
					collision_cell = map[current_cell_x][current_cell_y];

					break;
				}
				else if (1 == corner_collision)
				{
					//The ray can't go through 2 walls standing diagonally.
					if (gbl::MAP::Cell::Empty != map[current_cell_x - cell_step_x][current_cell_y] && gbl::MAP::Cell::Empty != map[current_cell_x][current_cell_y - cell_step_y])
					{
						collision_cell = map[current_cell_x - cell_step_x][current_cell_y];

						break;
					}
				}
			}
			else
			{
				break;
			}
		}

		//Once we hit the wall, we need to find where exactly we hit it to draw the texture correctly.
		if (0 == side % 2)
		{
			side_x = ray_start_y + ray_direction_y * stripes[a].get_distance();
		}
		else
		{
			side_x = ray_start_x + ray_direction_x * stripes[a].get_distance();
		}

		if (2 > side)
		{
			side_x = ceil(side_x) - side_x;
		}
		else
		{
			side_x -= floor(side_x);
		}

		stripes[a].set_angle(get_radians(atan2(-ray_direction_y, ray_direction_x)));
		stripes[a].set_side(side);
		stripes[a].set_side_x(side_x);
		stripes[a].set_x(a);

		ray_angle = stripes[a].get_angle() - deg_to_rad(player.direction.x);

		//We're calculating the perpendicular distance when casting rays. But we also need the Eukacfiragridalidian distance to visualize the FOV on the map.
		stripes[a].set_true_distance(stripes[a].get_distance() / abs(cos(ray_angle)));

		switch (collision_cell)
		{
		case gbl::MAP::Cell::Wall:
		{
			stripes[a].set_sprite_name("WALL_DESTROY_2");
		}
		}
	}
}

NpcManager* Game::GetNpcData()
{
	return &npc_manager;
}


void Game::set_title(const std::string& i_title)
{
	window.setTitle(i_title);
}

void Game::Update(float deltaTime, entt::registry& registry)
{
	auto & player = registry.get<gbl::Transform>(playerEntity);
	auto & viewNpc = registry.view<gbl::NPC::NpcData>();
	auto & viewProp = registry.view<gbl::PropData>();

	float player_movement_distance;

	sf::Vector2f player_position = player.position;

	UpdatePlayer(deltaTime, this, registry);
	for (auto npcEntity : viewNpc) {
		UpdateNpc(this, registry, npcEntity);
	}

	player_movement_distance = sqrt(pow(player_position.x - player.position.x, 2) + pow(player_position.y - player.position.y, 2));

	GameRaycast(registry);
	for (auto propEntity : viewProp)
	{
		auto& currentProp = viewProp.get<gbl::PropData>(propEntity);
		UpdateProp(this, registry, propEntity);
	}

	hand_animation.Update(registry.get<gbl::SpriteData>(playerEntity), registry.get<gbl::SpriteData>(playerEntity).frame);

	//So this is what they mean when they say "Drawing hands is hard".
	if (0 < player_movement_distance)
	{
		hand_offset += gbl::SPRITES::HAND_BOBBING_SPEED * player_movement_distance / gbl::PLAYER::MOVEMENT_SPEED;

		while (1 <= hand_offset)
		{
			hand_offset -= 2;
		}
	}
	else
	{
		if (-0.5f > hand_offset)
		{
			hand_offset += gbl::SPRITES::HAND_BOBBING_SPEED;

			if (-0.5f < hand_offset)
			{
				hand_offset = -0.5f;
			}
		}
		else if (-0.5f < hand_offset && 0.5f > hand_offset)
		{
			if (0 > hand_offset)
			{
				hand_offset -= gbl::SPRITES::HAND_BOBBING_SPEED;

				if (-0.5f > hand_offset)
				{
					hand_offset = -0.5f;
				}
			}
			else
			{
				hand_offset += gbl::SPRITES::HAND_BOBBING_SPEED;

				if (0.5f < hand_offset)
				{
					hand_offset = 0.5f;
				}
			}
		}
		else if (0.5f < hand_offset)
		{
			hand_offset -= gbl::SPRITES::HAND_BOBBING_SPEED;

			if (0.5f > hand_offset)
			{
				hand_offset = 0.5f;
			}
		}

		hand_offset = -abs(hand_offset);
	}

}