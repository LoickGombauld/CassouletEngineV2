#include <iostream>
#include <CoraEngineLibrarie/Player.hpp>


Player::Player(Window& window, Map& map) : m_renderWindow(&window), m_map(map)
{
	info.m_radius = Radius;
	info.m_camera.SetRadius(info.m_radius);
	info.m_camera.SetFillColor(sf::Color(0, 255, 0));
	info.m_camera.SetOrigin(sf::Vector2f(5, 5));
	info.m_colliderRadius = PlayerColliderRadius;
	info.m_speedMove = SpeedMove;
	info.m_speedAngle = SpeedAngle;
	info.m_position = sf::Vector2f{ 0,0 };
	info.m_rayLength = RayLength;


	for (unsigned int i = 0; i < WIDTHSCREEN; i++)
	{
		sf::VertexArray ray(sf::Lines, 2);
		ray[0].color = sf::Color::Red;
		ray[1].color = sf::Color::Red;
		m_vecRays.push_back(ray);
	}
}



void Player::Draw() const
{
	/*for (auto& vec : m_vecRays)
		m_renderWindow->DrawVertex(vec);
	m_renderWindow->DrawCircleShape(info.m_camera);*/
	float projectionDist = 0.5f * blockSize / tan(Math::deg_to_rad(0.5f * Fov));

	short previous_column = SHRT_MIN;

	sf::RectangleShape floor_shape(sf::Vector2f(1 * WIDTHSCREEN, 0.5f * HEIGHTSCREEN));
	floor_shape.setFillColor(sf::Color(36, 54, 0));
	floor_shape.setPosition(0, 0.5f * HEIGHTSCREEN);

	m_renderWindow->DrawRectShape(Shape(floor_shape));

	for (unsigned short a = 0; a < WIDTHSCREEN; a++)
	{
		float rayDir = Fov * (floor(0.5f * WIDTHSCREEN) - a) / (WIDTHSCREEN - 1);
		float ray_projection_position = 0.5f * tan(Math::deg_to_rad(rayDir)) / tan(Math::deg_to_rad(0.5f * Fov));
		short current_Column = static_cast<short>(round(WIDTHSCREEN * (0.5f - ray_projection_position)));
		short next_column = WIDTHSCREEN;
		if (a < WIDTHSCREEN - 1)
		{
			float next_Ray_Dir = Fov * (floor(0.5f * WIDTHSCREEN) - 1 - a) / (WIDTHSCREEN - 1);
			ray_projection_position = 0.5f * tan(Math::deg_to_rad(next_Ray_Dir)) / tan(Math::deg_to_rad(0.5f * Fov));
			next_column = static_cast<short>(round(WIDTHSCREEN * (0.5f - ray_projection_position)));
		}
		if (previous_column < current_Column)
		{
			float raydist = Math::CalculateDistance(m_vecRays[a][1].position, m_vecRays[a][0].position);

			//float brightness = round(255 * std::max<float>(0, 2 * raydist / RENDER_DISTANCE - 1));
			//std::cout << "Brightness : " << brightness << std::endl;
			unsigned short column_Height = HEIGHTSCREEN * projectionDist / (raydist * cos(Math::deg_to_rad(rayDir)));
			sf::RectangleShape shape(sf::Vector2f(std::max(1, next_column - current_Column), column_Height));
			shape.setFillColor(sf::Color(146, 146, 170));
			shape.setPosition(current_Column, round(0.5f * (HEIGHTSCREEN - column_Height)));

			previous_column = current_Column;

			m_renderWindow->DrawRectShape(Shape(shape));
			shape.setFillColor(sf::Color(73, 255, 255));
			shape.setPosition(current_Column, round(0.5f * (HEIGHTSCREEN - column_Height)));
		}
	}
}




void Player::Update()
{
	sf::Time dt = info.m_clock.restart();
	UpdateKeyboardHit(dt);
	Projection();
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
			m_vecRays[i][1].position = { info.m_position.x + info.m_rayLength * dCos((info.m_angle + Fov / 2) - i * (Fov / WIDTHSCREEN))
			,info.m_position.y + info.m_rayLength * dSin((info.m_angle + Fov / 2) - i * (Fov / WIDTHSCREEN)) };
		}
	}
}



void Player::UpdateKeyboardHit(sf::Time dt) {

	previous = info.m_position;
	unsigned short window_center_x = static_cast<unsigned short>(round(0.5f * m_renderWindow->GetSize().x));
	unsigned short window_center_y = static_cast<unsigned short>(round(0.5f * m_renderWindow->GetSize().y));

	//Mouse control!
	//By the way, do I need to write comments? Can't you just watch my video? I've already explained everything there.
	float mouseDist = Math::CalculateDistance(sf::Vector2f(window_center_x, window_center_y), sf::Mouse::getPosition(*m_renderWindow->GetHandle()));
	std::cout << " MouseDit: " << mouseDist << std::endl;
	if (mouseDist > MouseDeadZone)
	{
		info.m_angle += Fov * (window_center_x - sf::Mouse::getPosition(*m_renderWindow->GetHandle()).x) / m_renderWindow->GetSize().x * info.m_speedAngle * dt.asSeconds();
	}
	//rotation_vertical = Fov * (window_center_y - sf::Mouse::getPosition(*m_renderWindow->GetHandle()).y) / m_renderWindow->GetSize().y;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		info.m_position.x += dCos(info.m_angle) * info.m_speedMove * dt.asSeconds();
		info.m_position.y += dSin(info.m_angle) * info.m_speedMove * dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		info.m_position.x -= dCos(info.m_angle) * info.m_speedMove * dt.asSeconds();
		info.m_position.y -= dSin(info.m_angle) * info.m_speedMove * dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		info.m_position.x = dCos(info.m_angle) * info.m_speedMove * dt.asSeconds();
		info.m_position.y += dSin(info.m_angle) * info.m_speedMove * dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		info.m_angle += info.m_speedAngle * dt.asSeconds();
	}

	if (info.m_angle > 180)
		info.m_angle = -180;
	if (info.m_angle < -180)
		info.m_angle = 180;

	for (int y = 0; y < yCase; y++)
	{
		for (int x = 0; x < xCase; x++)
		{

			if (m_map.GetCellType(x, y) == CellType::Wall &&
				Math::CalculateDistance(m_map.GetCell(x, y).GetCellPosition(), info.m_position) < info.m_colliderRadius ||
				m_map.ContainPoint(x, y, info.m_position) && m_map.GetCellType(x, y) == ::Wall)
			{
				info.m_position = previous;
			}
		}
	}

	info.m_camera.SetRotation(info.m_angle);
	info.m_camera.SetPostion(info.m_position);

}



bool Player::Intersect(unsigned int it) {
	float fAngle = (info.m_angle + Fov / 2 - it * (Fov / WIDTHSCREEN));
	sf::Vector2f direction = { dCos(fAngle), dSin(fAngle) };

	for (unsigned int l = 0; l < info.m_rayLength; l++)
	{
		int dx = (int)(info.m_position.x + l * direction.x);
		int dy = (int)(info.m_position.y + l * direction.y);
		if (m_map.CheckMapCase(dy / blockSize, dx / blockSize))
		{
			info.m_intersection.x = dx;
			info.m_intersection.y = dy;
			return true;
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


