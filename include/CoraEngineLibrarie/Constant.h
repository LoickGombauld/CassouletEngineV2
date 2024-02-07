#pragma once
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <entt/entt.hpp>
#include <CoraEngineLibrarie/AngleFunctions.h>

namespace gbl
{
	struct SpriteData
	{
		unsigned short total_frames;

		int frame;

		std::string image_location;
		std::string name;

		sf::Rect<unsigned short> texture_box;

		sf::Sprite* sprite;

		sf::Vector2<short> origin;
	};

	struct Animation {
		bool ping_pong;

		float animation_speed;
	

		bool Animation::Update(const SpriteData& sprite_data, int current_frame,const char i_animation_end = 0)
		{
			//We're gonna return whether or not the animation ended.
			//If the animation is going backwards and it reached the first frame, the left_end will be 1.
			bool left_end = 0;
			//If the animation is going forward and it reached the last frame, the right_end will be 1.
			bool right_end = 0;

			if (0 == ping_pong)
			{
				left_end = 0 > animation_speed + current_frame;
				right_end = sprite_data.total_frames <= animation_speed + current_frame;
			}
			else
			{
				//You have NO IDEA how long it took me to figure this out.
				left_end = 0 > animation_speed + current_frame || sprite_data.total_frames <= animation_speed + current_frame;
				right_end = current_frame < sprite_data.total_frames&& sprite_data.total_frames <= animation_speed + current_frame;

				if (0 == right_end)
				{
					right_end = current_frame >= sprite_data.total_frames && sprite_data.total_frames > animation_speed + current_frame;
				}
			}

			current_frame = std::fmod(animation_speed + current_frame + sprite_data.total_frames, sprite_data.total_frames);

			switch (i_animation_end)
			{
			case -1:
			{
				return left_end;
			}
			case 0:
			{
				return left_end || right_end;
			}
			case 1:
			{
				return right_end;
			}
			}
		}
	};

	struct EntityData {
		int life = 1;
		int lifeMax = 3;
		bool isDead() { return life >= 0; }
		bool in_the_view;
		int screen_x;
	};

	struct Transform {
		sf::Vector2f direction;
		sf::Vector2f position;
		std::string  tag;
	};

	struct Collider {
		float range;
		bool istrigger = true;
		bool OnCollision(sf::Vector2f& targetPosition, Collider& tragetCollider,Transform& entityPosition) {
			return (range > sqrt(pow(targetPosition.x - entityPosition.position.x, tragetCollider.range) + pow(targetPosition.y - entityPosition.position.y, tragetCollider.range)));		
		}
	};

	namespace WEAPON {

		struct WeaponData {

			int index;
			std::string name;
			int damage;
			float fireDelay;
			float range;
			float knockBack;
			sf::Sound fireSound;
		};
	}
	namespace MAP
	{
		constexpr unsigned char CELL_SIZE = 2;
		constexpr unsigned char COLUMNS = 64;
		constexpr unsigned char ROWS = 64;

		//This cell is used for the map.
		enum class Cell
		{
			Empty,
			Wall
		};

		//This cell is used for pathfinding.
		enum class PathCell
		{
			Empty,
			Invalid,
			Visited,
			Wall
		};

		template <typename value_type = Cell>
		using Map = std::array<std::array<value_type, ROWS>, COLUMNS>;
	}

	namespace PLAYER
	{
		constexpr float MOVEMENT_SPEED = 0.125;
		constexpr float COLLIDER_WIDTH = 10;
		constexpr float COLLIDER_HEIGHT = 10;
		struct PlayerData : public EntityData {
			float speed = 0;
			float acceleration;
			float friction = 0.0121f;
			sf::Vector2f velocity;
			float lastfire;
			float speedMax = 0.1f;
			int credit = 0;
			int bombs = 0;
			int keys = 0;
		};
	}

	namespace RAYCASTING
	{
		constexpr float FOV_HORIZONTAL = 90;
		constexpr float FOV_VERTICAL = 90;
		constexpr float MAX_VERTICAL_DIRECTION = 60;
		//Can someone give me the closest value of PI we can store in a float?
		constexpr float PI = 3.141592653589793116f;
		constexpr float RENDER_DISTANCE = 16;
	}

	namespace SCREEN
	{
		constexpr unsigned char RESIZE = 2;

		constexpr unsigned short HEIGHT = 360;
		constexpr unsigned short WIDTH = 640;

		//60 fps now
		constexpr float FPSMax = 144;
	}

	namespace SPRITES
	{
		constexpr float FIRE_ANIMATION_SPEED = 0.5f;
		constexpr float HAND_BOBBING_SPEED = 0.0425f;
		constexpr float HAND_SCALE = 3;

		constexpr unsigned short HAND_OFFSET_MAX = 32;
	}


	struct PropData : public EntityData {
		bool in_the_view;

		bool is_animation;

		float distance;

		int screen_x;

		std::string name;

		int get_height() const
		{
			return round(SCREEN::HEIGHT / (distance * tan(deg_to_rad(0.5f * RAYCASTING::FOV_VERTICAL))));
		}

	};
	template <typename value_type = unsigned short>
	using Position = std::pair<value_type, value_type>;
	namespace IA {

		struct Astar
		{
			//----------------< A star algorithm start >----------------
			unsigned short astar_path_length = 0;

			std::map<Position<>, Position<>> astar_previous_cell;

			std::vector<sf::Vector2<unsigned short>> astar_path_vector;

			sf::Vector2<unsigned short> next_cell;

			MAP::Map<MAP::PathCell> astar_map = {};

			MAP::Map<float> astar_f_scores = {};
			MAP::Map<float> astar_g_scores = {};
			MAP::Map<float> astar_h_scores = {};
			//----------------< A star algorithm end >----------------
		};

		enum StateMachine {
			Waiting,
			Partrol,
			FollowPlayer
		};

		constexpr float RANGE = 1.f;
	}

	namespace NPC
	{
		constexpr float MAX_MOVEMENT_DISTANCE = 32;
		constexpr float MAX_MOVEMENT_SPEED = 0.25f;
		constexpr float MIN_MOVEMENT_DISTANCE = 4;
		constexpr float MIN_MOVEMENT_SPEED = 0.10f;
		struct NpcData : public EntityData {
			IA::StateMachine currentState = IA::Waiting;
			std::string name;
			int damage;
			float fireDelay;
			float range;
			float knockBack;
			float distance;
			int get_height() const
			{
				return round(SCREEN::HEIGHT / (distance * tan(deg_to_rad(0.5f * RAYCASTING::FOV_VERTICAL))));
			}
			sf::Sound fireSound;
		};
	}

}