#include <iostream>
#include <CoraEngineLibrarie/Player.hpp>
#include <CoraEngineLibrarie/Entity.hpp>

Player::Player(Window& window, TextureManager& texture_manager, TextureType type, Map& map)
	: m_renderWindow(&window), m_map(map)
{
	SetupPlayer();

	switch (type) {
	case WallTexture:
		m_WallTexureManager = texture_manager;
		m_wallSprite = sf::Sprite(*m_FloorTextureManager.GetTexture(0));
		break;
	case FloorTexture:
		m_FloorTextureManager = texture_manager;
		m_floorSprite = sf::Sprite(*m_FloorTextureManager.GetTexture(0));
		break;
	default:;
	}
}

Player::Player(Window& window, TextureManager& wallTexture, TextureManager& floorTexure, Map& map)
	: m_map(map), m_renderWindow(&window), m_WallTexureManager(wallTexture), m_FloorTextureManager(floorTexure)
{
	m_wallSprite = sf::Sprite(*m_WallTexureManager.GetTexture(0));
	m_floorSprite = sf::Sprite(*m_FloorTextureManager.GetTexture(0));
	SetupPlayer();

}

void Player::SetupPlayer()
{
	m_camera.SetRadius(10);
	m_camera.SetFillColor(sf::Color(0, 255, 0));
	m_camera.SetOrigin(sf::Vector2f(10, 10));
	info.m_speedMove = SpeedMove / 10;
	info.m_speedAngle = SpeedAngle;
	info.m_position = sf::Vector2f{ 0,0 };
	info.m_rayLength = RENDER_DISTANCE;

	for (unsigned int i = 0; i < WIDTHSCREEN; i++)
	{
		sf::VertexArray ray(sf::Lines, 2);
		ray[0].color = sf::Color::Red;
		ray[1].color = sf::Color::Red;
		m_vecRays.push_back(ray);
	}
}
void Player::Draw(Window& mapWindow, Entity& entity)
{
	bool draw_entity = 0;
	for (auto& vec : m_vecRays)
		mapWindow.DrawVertex(vec);
	mapWindow.DrawCircleShape(m_camera);

	float ray_start_x = info.m_position.x + 0.5f * blockSize;
	float ray_start_y = info.m_position.y + 0.5f * blockSize;
	float projection_distance = 0.5f * blockSize / tan(Math::deg_to_rad(0.5f * FOV));
	float floor_level = round(0.5f * HEIGHTSCREEN * (1 + tan(Math::deg_to_rad(info.m_angleY)) / tan(Math::deg_to_rad(0.5f * FOV))));

	float entity_direction  = Math::get_degrees(Math::rad_to_deg(atan2(ray_start_y - entity.get_center_y(), entity.get_center_x() - ray_start_x))) + info.m_angleX;
	//My man Pythagoras is saving the day once again!

	float entity_distance = Math::CalculateDistance(info.m_position , entity.GetPos());

	short previous_column = SHRT_MIN;

	sf::RectangleShape floor_shape(sf::Vector2f(WIDTHSCREEN, HEIGHTSCREEN - floor_level));
	floor_shape.setFillColor(sf::Color(36, 54, 0));
	floor_shape.setPosition(0, floor_level);



	draw_entity = RENDER_DISTANCE >= entity_distance;

	m_renderWindow->DrawRectShape(Shape(floor_shape));
	for (unsigned short a = 0; a < WIDTHSCREEN; a++)
	{
		if (0 == (1 == draw_entity && entity_distance > info.view_rays[a]))
		{

			entity.Reset();
			float ray_direction = FOV * (floor(0.5f * WIDTHSCREEN) - a) / (WIDTHSCREEN - 1);

			float ray_projection_position = 0.5f * dTan(ray_direction) / dTan(0.5f * FOV);

			//Current column's position on the screen.
			short current_column = static_cast<short>(round(WIDTHSCREEN * (0.5f - ray_projection_position)));
			short next_column = WIDTHSCREEN;

			if (a < WIDTHSCREEN - 1)
			{
				float next_ray_direction = FOV * (floor(0.5f * WIDTHSCREEN) - 1 - a) / (WIDTHSCREEN - 1);

				ray_projection_position = 0.5f * tan(Math::deg_to_rad(next_ray_direction)) / tan(Math::deg_to_rad(0.5f * FOV));

				next_column = static_cast<short>(round(WIDTHSCREEN * (0.5f - ray_projection_position)));
			}
			if (previous_column < current_column)
			{
				float ray_end_x = m_vecRays[a][1].position.x;
				float ray_end_y = m_vecRays[a][1].position.y;
				float wall_texture_column_x = 0;

				unsigned char brightness = static_cast<unsigned char>(round(255 * std::max<float>(0, 2 * info.view_rays[a] / info.m_rayLength - 1)));

				unsigned short column_height = static_cast<unsigned short>(HEIGHTSCREEN * projection_distance / (info.view_rays[a] * dCos(ray_direction)));

				sf::RectangleShape shape(sf::Vector2f(std::max(1, next_column - current_column), column_height));
				shape.setFillColor(sf::Color(73, 255, 255, brightness));
				shape.setPosition(current_column, round(floor_level - 0.5f * column_height));

				previous_column = current_column;

				if (abs(ray_end_x - blockSize * round(ray_end_x / blockSize)) < abs(ray_end_y - blockSize * round(ray_end_y / blockSize)))
				{
					wall_texture_column_x = ray_end_y - blockSize * floor(ray_end_y / blockSize);
				}
				else
				{
					wall_texture_column_x = blockSize * ceil(ray_end_x / blockSize) - ray_end_x;
				}

				m_wallSprite.setPosition(current_column, round(floor_level - 0.5f * column_height));
				m_wallSprite.setTextureRect(sf::IntRect(static_cast<unsigned short>(round(wall_texture_column_x)), 0, 1, blockSize));
				m_wallSprite.setScale(std::max(1, next_column - current_column), column_height / static_cast<float>(blockSize));

				m_renderWindow->GetHandle()->draw(m_wallSprite);
				m_renderWindow->GetHandle()->draw(shape);
			}
		}
	}
	if (1 == draw_entity)
	{
		float entity_projection_position = 0.5f * dTan(entity_direction) / dTan(0.5f * FOV);

		short entity_screen_x = static_cast<short>(round(WIDTHSCREEN * (0.5f - entity_projection_position)));

		unsigned short entity_size = static_cast<unsigned short>(HEIGHTSCREEN * projection_distance/ (entity_distance * dCos(entity_direction)));

		previous_column = SHRT_MIN;

		entity.GetSprite().setColor(sf::Color(255, 255, 255, static_cast<unsigned char>(round(255 * std::min<float>(1, 2 * (1 - entity_distance / RENDER_DISTANCE))))));
		entity.SetPosition(round( entity_screen_x - 0.5f * entity_size),round( floor_level - 0.5f * entity_size));
		entity.SetScale(sf::Vector2f( entity_size / static_cast<float>(blockSize), entity_size / static_cast<float>(blockSize)));
		m_renderWindow->GetHandle()->draw(entity.GetSprite());
		for (unsigned short a = 0; a < WIDTHSCREEN; a++)
		{
			if (entity_distance > info.view_rays[a])
			{
				float ray_direction = FOV * (floor(0.5f * WIDTHSCREEN) - a) / (WIDTHSCREEN - 1);

				float ray_projection_position = 0.5f * dTan(ray_direction) / dTan(0.5f * FOV);

				//Current column's position on the screen.
				short current_column = static_cast<short>(round(WIDTHSCREEN * (0.5f - ray_projection_position)));
				short next_column = WIDTHSCREEN;

				if (a < WIDTHSCREEN - 1)
				{
					float next_ray_direction = FOV * (floor(0.5f * WIDTHSCREEN) - 1 - a) / (WIDTHSCREEN - 1);

					ray_projection_position = 0.5f * dTan(next_ray_direction) / dTan(0.5f * FOV);

					next_column = static_cast<short>(round(WIDTHSCREEN * (0.5f - ray_projection_position)));
				}
				if (previous_column < current_column)
				{
					float ray_end_x = m_vecRays[a][1].position.x;
					float ray_end_y = m_vecRays[a][1].position.y;
					float wall_texture_column_x = 0;

					unsigned char brightness = static_cast<unsigned char>(round(255 * std::max<float>(0, 2 * info.view_rays[a] / info.m_rayLength - 1)));

					unsigned short column_height = static_cast<unsigned short>(HEIGHTSCREEN * projection_distance / (info.view_rays[a] * dCos(ray_direction)));

					sf::RectangleShape shape(sf::Vector2f(std::max(1, next_column - current_column), column_height));
					shape.setFillColor(sf::Color(73, 255, 255, brightness));
					shape.setPosition(current_column, round(floor_level - 0.5f * column_height));

					previous_column = current_column;

					if (abs(ray_end_x - blockSize * round(ray_end_x / blockSize)) < abs(ray_end_y - blockSize * round(ray_end_y / blockSize)))
					{
						wall_texture_column_x = ray_end_y - blockSize * floor(ray_end_y / blockSize);
					}
					else
					{
						wall_texture_column_x = blockSize * ceil(ray_end_x / blockSize) - ray_end_x;
					}

					m_wallSprite.setPosition(current_column, round(floor_level - 0.5f * column_height));
					m_wallSprite.setTextureRect(sf::IntRect(static_cast<unsigned short>(round(wall_texture_column_x)), 0, 1, blockSize));
					m_wallSprite.setScale(std::max(1, next_column - current_column), column_height / static_cast<float>(blockSize));

					m_renderWindow->GetHandle()->draw(m_wallSprite);
					m_renderWindow->GetHandle()->draw(shape);
				}
			}
		}
	}
}


void Player::Update()
{
	UpdateKeyboardHit();
	Projection();
}

void Player::SetMouse(bool isUnlocked)
{
	mouseIslocked = isUnlocked;
	m_renderWindow->SetMouseCursorVisible(!isUnlocked);
}

void Player::UpdateKeyboardHit() {

	//Mouse control!
	//By the way, do I need to write comments? Can't you just watch my video? I've already explained everything there.
	if (mouseIslocked)
	{
		unsigned short window_center_x = static_cast<unsigned short>(round(0.5f * m_renderWindow->GetSize().x));
		unsigned short window_center_y = static_cast<unsigned short>(round(0.5f * m_renderWindow->GetSize().y));

		float rotation_horizontal = FOV * (window_center_x - sf::Mouse::getPosition(*m_renderWindow->GetHandle()).x) / m_renderWindow->GetSize().x * info.m_speedAngle;
		float rotation_vertical = FOV * (window_center_y - sf::Mouse::getPosition(*m_renderWindow->GetHandle()).y) / m_renderWindow->GetSize().y * info.m_speedAngle;

		info.m_angleX = Math::get_degrees(info.m_angleX + rotation_horizontal);
		info.m_angleY = std::clamp<float>(info.m_angleY + rotation_vertical, -89, 89);

		sf::Mouse::setPosition(sf::Vector2i(window_center_x, window_center_y), *m_renderWindow->GetHandle());
		sf::Vector2f movement(0.0f, 0.0f); // Vecteur de mouvement
		bool updownIspressed = false;
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			movement.x += dCos(info.m_angleX) * info.m_speedMove;
			movement.y += dSin(info.m_angleX) * info.m_speedMove;
			updownIspressed = true;
		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			movement.x -= dCos(info.m_angleX) * info.m_speedMove;
			movement.y -= dSin(info.m_angleX) * info.m_speedMove;
			updownIspressed = true;
		}

		float localspeedMove = info.m_speedMove;
		if (updownIspressed)
		{
			localspeedMove /= 2;
		}

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			movement.x += dCos(info.m_angleX - 90) * localspeedMove;
			movement.y += dSin(info.m_angleX - 90) * localspeedMove;
		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			movement.x += dCos(info.m_angleX + 90) * localspeedMove;
			movement.y += dSin(info.m_angleX + 90) * localspeedMove;
		}

		UpdateRay();
		CheckCollisionWithWalls(movement);
		m_camera.SetRotation(info.m_angleX);
		m_camera.SetPostion(info.m_position);

		std::cout << info.m_angleX << std::endl;
	}
}


void Player::UpdateRay()
{
	for (unsigned short a = 0; a < WIDTHSCREEN; a++)
	{
		sf::Vector2f pos0 = m_vecRays[a][0].position;
		sf::Vector2f pos1 = m_vecRays[a][1].position;

		pos0 = Math::CalculateNormal(pos0);
		pos1 = Math::CalculateNormal(pos1);
		float ray_length = Math::CalculateDistance(pos0, pos1);
		info.view_rays[a] = ray_length;
	}

	//for (unsigned short a = 0; a < WIDTHSCREEN; a++)
	//{
	//	sf::Vector2f pos0 = m_vecRays[a][0].position;
	//	sf::Vector2f pos1 = m_vecRays[a][1].position;	
	//	float ray_length = Math::CalculateDistance(pos0, pos1);;
	//	char cell_step_y = 0;
	//	char cell_step_x = 0;
	//	float ray_direction = info.m_angleX + Fov / 2 - a * (Fov / WIDTHSCREEN);
	//	float ray_direction_x = dCos(ray_direction);
	//	float ray_direction_y = dSin(ray_direction);
	//	//This is the value we need.
	//	float ray_start_x = m_vecRays[a][0].position.x + 0.5f * blockSize;
	//	float ray_start_y = m_vecRays[a][0].position.y + 0.5f * blockSize;
	//	//This ray checks for horizontal collisions.
	//	float x_ray_length = 0;
	//	//This ray checks for vertical collisions.
	//	float y_ray_length = 0;
	//	//This is the length of the ray after moving 1 unit along the x-axis.
	//	float x_ray_unit_length = static_cast<float>(blockSize * sqrt(1 + pow(ray_direction_y / ray_direction_x, 2)));
	//	//This is the length of the ray after moving 1 unit along the y-axis.
	//	float y_ray_unit_length = static_cast<float>(blockSize * sqrt(1 + pow(ray_direction_x / ray_direction_y, 2)));

	//	unsigned char current_cell_x = static_cast<unsigned char>(ray_start_x / blockSize);
	//	unsigned char current_cell_y = static_cast<unsigned char>(ray_start_y / blockSize);
	//	if (0 > ray_direction_x)
	//	{
	//		cell_step_x = 1;

	//		//In order for the algorithm to work, the ray must start at the cell borders.
	//		//So if the starting position of the ray is not a cell border (which is very likely), we'll stretch it to the closest one.
	//		x_ray_length = x_ray_unit_length * (info.m_position.x / blockSize + current_cell_x);
	//	}
	//	else if (0 < ray_direction_x)
	//	{
	//		cell_step_x = -1;

	//		x_ray_length = x_ray_unit_length * (1 + current_cell_x - info.m_position.x / blockSize);
	//	}
	//	else
	//	{
	//		cell_step_x = 0;
	//	}

	//	if (0 > ray_direction_y)
	//	{
	//		cell_step_y = 1;

	//		y_ray_length = y_ray_unit_length * (info.m_position.x / blockSize - current_cell_y);
	//	}
	//	else if (0 < ray_direction_y)
	//	{
	//		cell_step_y = -1;

	//		y_ray_length = y_ray_unit_length * (1 + current_cell_y - info.m_position.x / blockSize);
	//	}
	//	else
	//	{
	//		cell_step_y = 0;
	//	}

	//	//We continue casting the ray until it reaches the render distance.
	//	while (RENDER_DISTANCE >= ray_length)
	//	{
	//		//In case the ray hits a corner (which is very unlikely, but I like to add this kind of stuff just in case).
	//		bool corner_collision = 0;

	//		//We stretch the shortest ray.
	//		if (x_ray_length < y_ray_length)
	//		{
	//			ray_length += x_ray_length;
	//			x_ray_length += x_ray_unit_length;

	//			current_cell_x += cell_step_x;
	//		}
	//		else if (x_ray_length > y_ray_length)
	//		{
	//			ray_length += y_ray_length;
	//			y_ray_length += y_ray_unit_length;

	//			current_cell_y += cell_step_y;
	//		}
	//		else
	//		{
	//			//If the rays are equal, that means we hit the corner, so we stretch both rays.
	//			corner_collision = 1;

	//			ray_length += x_ray_length;
	//			x_ray_length += x_ray_unit_length;
	//			y_ray_length += y_ray_unit_length;

	//			current_cell_x += cell_step_x;
	//			current_cell_y += cell_step_y;
	//		}

	//		//Making sure the current cell we're checking is inside our map.
	//		if (0 <= current_cell_x && 0 <= current_cell_y && yCase > current_cell_y && xCase > current_cell_x)
	//		{
	//			if (Wall == m_map.GetCellType(current_cell_x, current_cell_y))
	//			{
	//				//We stop casting the ray if it hits the wall.
	//				break;
	//			}
	//			 if (1 == corner_collision)
	//			{
	//				//The ray can't go through 2 walls standing diagonally.
	//				if (Wall == m_map.GetCellType(current_cell_x - cell_step_x, current_cell_y) && Wall == m_map.GetCellType(current_cell_x, current_cell_y - cell_step_y))
	//				{
	//					break;
	//				}
	//			}
	//		}
	//	}

	//	//The ray's length must be less than or equal to the render distance.
	//	ray_length = std::min(RENDER_DISTANCE, ray_length);

	//	info.view_rays[a] = ray_length;
	//}
}
void Player::CheckCollisionWithWalls(sf::Vector2f& movement)
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



bool Player::map_collision(float i_x, float i_y)
{
	float cell_x = i_x / blockSize;
	float cell_y = i_y / blockSize;

	if (Wall == m_map.GetCellType(cell_x, cell_y))
	{
		std::cout << "Collision" << std::endl;
		return 1;
	}

	return 0;
}

void Player::Projection()
{

	for (unsigned int i = 0; i < m_vecRays.size(); i++)
	{
		m_vecRays[i][0].position = info.m_position;
		if (Intersect(i))
		{

			m_vecRays[i][1].position = info.m_intersection;
		}
		else {
			m_vecRays[i][1].position = { info.m_position.x + info.m_rayLength * dCos((info.m_angleX + FOV / 2) - i * (FOV / WIDTHSCREEN))
			,info.m_position.y + info.m_rayLength * dSin((info.m_angleX + FOV / 2) - i * (FOV / WIDTHSCREEN)) };
		}
	}
}

bool Player::Intersect(unsigned int it) {
	float fAngle = info.m_angleX + FOV / 2 - it * (FOV / WIDTHSCREEN);
	sf::Vector2f direction = { dCos(fAngle), dSin(fAngle) };

	for (float l = 0; l < RENDER_DISTANCE; l++)
	{
		int dx = info.m_position.x + l * direction.x;
		int dy = info.m_position.y + l * direction.y;

		int mapX = (dx / blockSize);
		int mapY = (dy / blockSize);

		if (mapX >= 0 && mapX < xCase && mapY >= 0 && mapY < yCase)
		{
			if (m_map.GetCellType(mapX, mapY) == Wall)
			{
				info.m_intersection.x = dx;
				info.m_intersection.y = dy;
				return true;
			}
		}
		else
		{
			// Sortir du raycasting si les coordonnées sortent des limites de la carte
			break;
		}
	}
	return false;
}

void Player::SetPosition(sf::Vector2f position)
{
	info.m_position = position;
}

void Player::SetPosition(float x, float y)
{
	info.m_position = { x,y };
}

sf::Vector2f Player::GetPosition() const
{
	return info.m_position;
}


