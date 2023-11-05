#include <iostream>
#include <exception>
#include <math.h>
#include <CoraEngineLibrarie/TextureManager.hpp>
#include <CoraEngineLibrarie/MapManager.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/GpuPreference.hpp>
#include <CoraEngineLibrarie/Player.h>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/Map.h>

SFML_DEFINE_DISCRETE_GPU_PREFERENCE
bool isCollision(int x, int y, int nb_case_w, int nb_case_h, Map& m_map);
void UpdatePhysic(sf::Vector2f& movement, int mapSizeX, int mapSizeY, Map& map);
sf::RectangleShape generate_map(int x, int y, int nb_case_w, int nb_case_h);
void line_view(sf::RenderWindow& windows, float nb_dec);

Player player;


int main()
{
	sf::RenderWindow window(sf::VideoMode(width, height), "CoraEngine 1.2");
	std::cout << "Welcome to Fantasy Zone Get Ready !" << std::endl;
	TextureManager::Init();
	MapManager mapManager("Level");
	sf::Image mapImage(*mapManager.GetLevel(0));
	int mapSizeX = mapImage.getSize().x;
	int mapSizeY = mapImage.getSize().y;
	Map map(mapImage);

	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(fps);
	bool activeMouse = false;
	sf::IntRect windowRect(window.getPosition(), sf::Vector2i(window.getSize()));
	player.SetPosition(map.playerStarter.x, map.playerStarter.y);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
				windowRect = sf::IntRect(window.getPosition(), sf::Vector2i(window.getSize()));
		}
		window.clear(sf::Color::Blue);
		player.UpdatePostion();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			activeMouse = false;
			window.setMouseCursorVisible(!activeMouse);
		}
		else if (windowRect.contains(sf::Mouse::getPosition()) & sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			activeMouse = true;
			window.setMouseCursorVisible(!activeMouse);
		}
		if (activeMouse)
		{
			if (window.hasFocus()) {

				unsigned short window_center_x = static_cast<unsigned short>(round(0.5f * window.getSize().x));
				unsigned short window_center_y = static_cast<unsigned short>(round(0.5f * window.getSize().y));

				float rotation_horizontal = angle_vision * (window_center_x - sf::Mouse::getPosition(window).x) / window.getSize().x * v_angle;
				float rotation_vertical = angle_vision * (window_center_y - sf::Mouse::getPosition(window).y) / window.getSize().y * v_angle;

				player.angleX = static_cast<float>(fmod(360 + fmod(player.angleX + rotation_horizontal, 360), 360));
				player.angleY = std::clamp<float>(player.angleY + rotation_vertical, -89, 89);

				sf::Mouse::setPosition(sf::Vector2i(window_center_x, window_center_y), window);
				sf::Vector2f movement;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					movement.x += dCos(player.angleX - 90) * vitesse;
					movement.y += dSin(player.angleX - 90) * vitesse;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					movement.x += dCos(player.angleX + 90) * vitesse;
					movement.y += dSin(player.angleX + 90) * vitesse;
				}
				//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				//	if (isCollision(player.Posx + cos(player.angleX * PI / 180) * vitesse, player.Posy - sin(player.angleX * PI / 180) * vitesse, mapSizeX, mapSizeY, map)) {
				//		movement.x += dCos(player.angleX) * vitesse;
				//		movement.y -= dSin(player.angleX) * vitesse;
				//	}
				//}
				//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				//	if (isCollision(player.Posx - cos(player.angleX * PI / 180) * vitesse, player.Posy + sin(player.angleX * PI / 180) * vitesse, mapSizeX, mapSizeY, map)) {
				//		movement.x -= dCos(player.angleX) * vitesse;
				//		movement.y += dSin(player.angleX) * vitesse;
				//	}
				//}
				//UpdatePhysic(movement, mapSizeX, mapSizeY, map);
			}
		}

		//Minimap
		//map.Draw(window);
		window.draw(player.body);
		window.display();
	}
	std::cout << "See you in space cowboy..." << std::endl;
	return 0;
};

void UpdatePhysic(sf::Vector2f& movement, int mapSizeX, int mapSizeY, Map& map)
{
	if (0 == isCollision(movement.x + player.Posx, movement.y + player.Posy, mapSizeX, mapSizeY, map))
	{
		player.Posx += movement.x;
		player.Posy += movement.y;
	}
	else if (0 == isCollision(movement.x + player.Posx, player.Posy, mapSizeX, mapSizeY, map))
	{
		player.Posx += movement.x;
	}
	else if (0 == isCollision(player.Posx, movement.y + player.Posy, mapSizeX, mapSizeY, map))
	{
		player.Posy += movement.y;
	}
	else
	{
		//We're placing the player on the nearest cell so that it touches the wall.
		player.Posx = rate_map * round(player.Posx / rate_map);
		player.Posy = rate_map * round(player.Posy / rate_map);
	}
}

sf::RectangleShape generate_map(int x, int y, int nb_case_w, int nb_case_h) {
	float width_map = width / rate_map;
	float height_map = height / rate_map;
	sf::RectangleShape border(sf::Vector2f(width_map / nb_case_w, height_map / nb_case_h));
	border.setPosition(x / nb_case_w, y / nb_case_h);
	return border;
}

bool isCollision(int x, int y, int nb_case_w, int nb_case_h, Map& m_map) {
	float cell_x = x / nb_case_w;
	float cell_y = y / nb_case_h;

	if (Wall == m_map.GetCellType(cell_x, cell_y))
	{
		std::cout << "Collision" << std::endl;
		return 1;
	}

	return 0;
}