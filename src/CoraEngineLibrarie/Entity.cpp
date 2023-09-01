#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Entity.hpp>


Entity::Entity(sf::Texture& entityTexture, Map& map) : m_entitySprite(sf::Sprite(entityTexture)), m_map(map)
{

	info.m_speedMove = SpeedMove / 10;
	info.m_speedAngle = SpeedAngle;
	info.m_position = sf::Vector2f{ 0,0 };
	info.m_rayLength = RENDER_DISTANCE /2 ;
	m_entitySprite.setOrigin(10, 10);
	m_originSize = m_entitySprite.getScale();

}

sf::Vector2f Entity::GetPos()
{
	return info.m_position;
}

float Entity::get_center_x() const
{
	return info.m_position.x + 0.5f * blockSize;
}

float Entity::get_center_y() const
{
	return info.m_position.y + 0.5f * blockSize;
}


float Entity::GetDirection() const
{
	return info.m_angleX;
}

void Entity::DrawOnMap(Window& i_window)
{
	i_window.DrawSprite(m_entitySprite);
}

void Entity::SetPosition(float i_x, float i_y)
{
	info.m_position.x = i_x;
	info.m_position.y = i_y;
}

void Entity::SetOriginPosition(float i_x, float i_y)
{
	m_originPos = { i_x,i_y };
}
void Entity::SetOriginPosition(sf::Vector2f pos)
{
	m_originPos = pos;
}


void Entity::Update()
{
	m_entitySprite.setPosition(m_originPos);
}


void Entity::CheckCollisionWithWalls(sf::Vector2f& movement)
{

	if (0 == map_collision(movement.x + info.m_position.x, movement.y + info.m_position.y))
	{
		info.m_position.x += movement.x;
		info.m_position.y += movement.y;
	}
	else if (0 == map_collision(movement.x + info.m_position.x, info.m_position.y))
	{
		info.m_position.x += movement.x;
	}
	else if (0 == map_collision(info.m_position.x, movement.y + info.m_position.y))
	{
		info.m_position.y += movement.y;
	}
	else
	{
		//We're placing the player on the nearest cell so that it touches the wall.
		info.m_position.x = blockSize * round(info.m_position.x / blockSize);
		info.m_position.y = blockSize * round(info.m_position.y / blockSize);
	}


}

bool Entity::map_collision(float i_x, float i_y)
{
	float cell_x = i_x / blockSize;
	float cell_y = i_y / blockSize;

	if (Wall == m_map.GetCellType(cell_x, cell_y))
	{
		//std::cout << "Collision" << std::endl;
		return 1;
	}

	return 0;
}

sf::Sprite Entity::GetSprite()
{
	return m_entitySprite;
}

void Entity::SetScale(sf::Vector2f& scale)
{
	m_entitySprite.setScale(scale);
}

void Entity::Reset()
{
	if (info.m_position != m_originPos)
	info.m_position = m_originPos;
	if (m_entitySprite.getScale() != m_originSize)
		m_entitySprite.setScale(m_originSize);
}
