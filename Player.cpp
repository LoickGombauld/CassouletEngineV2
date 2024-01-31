#include <CoraEngineLibrarie/Player.h>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/AngleFunctions.h>
#include <CoraEngineLibrarie/MapCollision.h>
#include <CoraEngineLibrarie/Npc.h>
#include <CoraEngineLibrarie/Raycast.h>
#include <CoraEngineLibrarie/Game.h>
#include <CoraEngineLibrarie/Events.h>


Player::Player(const float i_x, const float i_y) :
	maxSpeed(gbl::PLAYER::MOVEMENT_SPEED)
{
	playerdata.weapon.name = "WEAPON_2";
}




void Player::Update(Game* game, float deltaTime)
{
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
			speed += deltaTime /2;
		}
		else
		{
			speed -= deltaTime;
		}
		speed = std::clamp(speed, 0.f, maxSpeed);
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			step_x += speed * cos(deg_to_rad(90 + direction.x));
			step_y += -speed * sin(deg_to_rad(90 + direction.x));

		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			step_x += speed * cos(deg_to_rad(direction.x - 90));
			step_y += -speed * sin(deg_to_rad(direction.x - 90));

		}
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			step_x -= speed * cos(deg_to_rad(direction.x));
			step_y += speed * sin(deg_to_rad(direction.x));
		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			step_x += speed * cos(deg_to_rad(direction.x));
			step_y -= speed * sin(deg_to_rad(direction.x));
		}
		if(!hasMove)
		{
			velocity.x *= 0.90f - friction;
			velocity.y *= 0.90f - friction;
			step_x = velocity.x;
			step_y = velocity.y;
		}

		std::cout << "Velocity : " << step_x << ";" << step_y << std::endl;
		std::cout << "Speed : " << speed << std::endl;
		velocity = sf::Vector2f(step_x, step_y);
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
		lastfire += deltaTime;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && lastfire > playerdata.weapon.fireDelay)
		{
			std::cout << "fire !" << std::endl;
			Shoot();
			for (std::map<int, Npc>::iterator it = game->npc.begin(); it != game->npc.end(); ++it)
			{
				if (Raycast::Send(sf::Vector2f(position.x + 0.5f, position.y + 0.5f), direction.x, playerdata.weapon.range, it->second))
				{
					it->second.currentState = gbl::IA::StateMachine::FollowPlayer;
					break;
				}
			}
			lastfire = 0;
		}
		UpdateCollider(std::function<void(Entity&)>(PlayerTakeDamage),game,"Enemy");
		UpdateCollider(std::function<void(Entity&)>(PlayerTakeDamage),game,"Projectile_Enemy");
	}
}



void Player::Shoot()
{
	playerdata.weapon.fireSound.play();
}

sf::Vector2f Player::get_direction() const
{
	return direction;
}

sf::Vector2f Player::get_position() const
{
	return position;
}