
#include <CoraEngineLibrarie/Npc.h>
#include <CoraEngineLibrarie/AngleFunctions.h>
#include <CoraEngineLibrarie/Astar.h>
#include <CoraEngineLibrarie/MapCollision.h>
#include <CoraEngineLibrarie/NpcManager.hpp>

Npc::Npc(SpriteManager& i_sprite_manager, float i_x, float i_y) :
	currentState(gbl::IA::StateMachine::Waiting),
	in_the_view(0),
	distance(0),
	screen_x(0),
	current_frame(0),
	sprite_manager(&i_sprite_manager),
	astar_path_length(0),
	next_cell(transform.position),
	astar_map({}),
	astar_f_scores({}),
	astar_g_scores({}),
	astar_h_scores({})

{
	data = Game::I()->GetNpcData()->get_Npc_data("SOLDIER");
	//Entity construction
	transform.direction=sf::Vector2f( 0,0);
	transform.position = sf::Vector2f(i_x, i_y);
}

int Npc::get_height() const
{
	return round(gbl::SCREEN::HEIGHT / (distance * tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_VERTICAL))));
}

int Npc::get_width() const
{
	float sprite_height = sprite_manager->get_sprite_data(data.name).texture_box.height;
	float sprite_width = sprite_manager->get_sprite_data(data.name).texture_box.width;

	return round(gbl::SCREEN::HEIGHT * sprite_width / (distance * sprite_height * tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL))));
}

int Npc::get_x() const
{
	return screen_x - round(0.5f * get_width());
}

int Npc::get_y() const
{
	return round(0.5f * (gbl::SCREEN::HEIGHT - get_height()));
}

void Npc::draw(const short i_pitch, sf::RenderWindow& i_window)
{
	float sprite_height = sprite_manager->get_sprite_data(data.name).texture_box.height;
	float sprite_width = sprite_manager->get_sprite_data(data.name).texture_box.width;

	unsigned char shade = 255 * std::clamp<float>(1 - distance / gbl::RAYCASTING::RENDER_DISTANCE, 0, 1);

	in_the_view &= gbl::SCREEN::HEIGHT > i_pitch + get_y() && gbl::SCREEN::WIDTH > get_x() && get_x() > -1 * get_width() && i_pitch + get_y() > -1 * get_height();

	if (0 < shade && 1 == in_the_view)
	{
		sprite_manager->draw_sprite(current_frame, data.name, sf::Vector2<short>(get_x(), i_pitch + get_y()), i_window, 0, 0, get_width() / sprite_width, get_height() / sprite_height, sf::Color(shade, shade, shade));
	}
}

void Npc::fill_map(const gbl::MAP::Map<>& i_map)
{
	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			if (gbl::MAP::Cell::Empty != i_map[a][b])
			{
				astar_map[a][b] = gbl::MAP::PathCell::Wall;
			}
			else
			{
				astar_map[a][b] = gbl::MAP::PathCell::Empty;
			}
		}
	}
}

void Npc::set_position(const float i_x, const float i_y)
{
	transform.position.x = i_x;
	transform.position.y = i_y;

	next_cell.x = transform.position.x;
	next_cell.y = transform.position.y;
}

void Npc::Update(Game* game)
{
	if (1 == game->window.hasFocus())
	{
		float angle = get_radians(atan2(game->player.get_position().y - transform.position.y, transform.position.x - game->player.get_position().x));
		float difference = deg_difference(game->player.get_direction().x, rad_to_deg(angle));
		float frame_angle = 360.f / sprite_manager->get_sprite_data(data.name).total_frames;
		float shifted_direction;
		float speed = 0;
		float step_x = 0;
		float step_y = 0;

		//We're taking the cells that are closest to Steven and the player as starting and finishing cells.
		sf::Vector2<unsigned short> finish_position(round(game->player.get_position().x), round(game->player.get_position().y));
		sf::Vector2<unsigned short> start_position(round(transform.position.x), round(transform.position.y));
		switch (currentState)
		{
		case gbl::IA::Waiting:
			break;
		case gbl::IA::Partrol:
			break;
		case gbl::IA::FollowPlayer:

		if (finish_position != start_position)
		{
			astar_reset(astar_path_length, astar_previous_cell, astar_path_vector, astar_f_scores, astar_g_scores, astar_h_scores, finish_position, start_position, astar_map);
			astar_search(astar_path_length, astar_previous_cell, astar_path_vector, astar_f_scores, astar_g_scores, astar_h_scores, next_cell, finish_position, start_position, astar_map);

			transform.direction.x = rad_to_deg(atan2(transform.position.y - next_cell.y, next_cell.x - transform.position.x));

			//The farther Steven is from the player, the faster he moves, and vice versa.
			if (gbl::NPC::MAX_MOVEMENT_DISTANCE < astar_path_length)
			{
				speed = gbl::NPC::MAX_MOVEMENT_SPEED;
			}
			else if (gbl::NPC::MIN_MOVEMENT_DISTANCE > astar_path_length)
			{
				speed = gbl::NPC::MIN_MOVEMENT_SPEED;
			}
			else
			{
				speed = gbl::NPC::MIN_MOVEMENT_SPEED + (gbl::NPC::MAX_MOVEMENT_SPEED - gbl::NPC::MIN_MOVEMENT_SPEED) * (astar_path_length - gbl::NPC::MIN_MOVEMENT_DISTANCE) / (gbl::NPC::MAX_MOVEMENT_DISTANCE - gbl::NPC::MIN_MOVEMENT_DISTANCE);
			}
		}
			break;
		default:
			break;
		}
		
		
		// si Npc rentre en contact avec le joueur
		if (1 > sqrt(pow(transform.position.x - game->player.get_position().x, 2) + pow(transform.position.y - game->player.get_position().y, 2)))
		{
		}

		//Steven moves to the next cell in the A star path.
		if (next_cell.x < transform.position.x)
		{
			step_x = std::max(-speed, next_cell.x - transform.position.x);
		}
		else if (next_cell.x > transform.position.x)
		{
			step_x = std::min(speed, next_cell.x - transform.position.x);
		}

		if (next_cell.y < transform.position.y)
		{
			step_y = std::max(-speed, next_cell.y - transform.position.y);
		}
		else if (next_cell.y > transform.position.y)
		{
			step_y = std::min(speed, next_cell.y - transform.position.y);
		}

		if (0 == map_collision(step_x + transform.position.x, step_y + transform.position.y, game->map))
		{
			transform.position.x += step_x;
			transform.position.y += step_y;
		}
		else if (0 == map_collision(step_x + transform.position.x, transform.position.y, game->map))
		{
			transform.position.x += step_x;
			transform.position.y = round(transform.position.y);
		}
		else if (0 == map_collision(transform.position.x, step_y + transform.position.y, game->map))
		{
			transform.position.x = round(transform.position.x);
			transform.position.y += step_y;
		}
		else
		{
			transform.position.x = round(transform.position.x);
			transform.position.y = round(transform.position.y);
		}

		//This makes it so that the difference is between -180 to 180.
		if (deg_difference(game->player.get_direction().x, difference + rad_to_deg(angle)) < deg_difference(game->player.get_direction().x, rad_to_deg(angle) - difference))
		{
			difference *= -1;
		}

		shifted_direction = get_degrees(90 + get_degrees(transform.direction.x + 0.5f * frame_angle) - difference - game->player.get_direction().x);

		//Calculating the perpendicular distance from Steven to the player.
		distance = abs(game->player.get_position().y - transform.position.y - tan(deg_to_rad(game->player.get_direction().x - 90)) * (transform.position.x - game->player.get_position().x)) / sqrt(1 + pow(tan(deg_to_rad(game->player.get_direction().x - 90)), 2));

		screen_x = round(0.5f * gbl::SCREEN::WIDTH * (1 - tan(deg_to_rad(difference)) / tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL))));

		in_the_view = 90 > abs(difference);

		current_frame = floor(shifted_direction / frame_angle);
	}
}

sf::Vector2f Npc::get_position() const
{
	return transform.position;
}