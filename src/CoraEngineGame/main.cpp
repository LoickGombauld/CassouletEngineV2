#include <iostream>
#include <exception>
#include <math.h>
#include <CoraEngineLibrarie/TextureManager.hpp>
#include <CoraEngineLibrarie/MapManager.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#define PI 3.14159265

#include <CoraEngineLibrarie/Player.h>
#include <CoraEngineLibrarie/Constant.h>

inline float toRadian(float degree);
inline float dCos(float degree);
inline float dSin(float degree);
inline float dTan(float degree);
sf::RectangleShape generate_map(int i, int nb_case_w, int nb_case_h);
bool isCollision(int x, int y);
void line_view(sf::RenderWindow& wi, float nb_dec);

Player player;
std::vector<CellType> map;
int main()
{
	std::cout << "Welcome to Fantasy Zone Get Ready !" << std::endl;

	MapManager mapManager("Level");
	sf::Image mapImage(*mapManager.GetLevel(0));
	const int mapSizeX = (int)mapImage.getSize().x;
	const int mapSizeY = (int)mapImage.getSize().y;
	sf::Vector2f PlayerSpawn;
	for (unsigned char a = 0; a < mapSizeX; a++)
	{
		for (unsigned char b = 0; b < mapSizeY; b++)
		{
			sf::Color pixel = mapImage.getPixel(a, b);

			if (pixel == sf::Color::White)
			{
				map.push_back(CellType::Wall);
			}
			else if (pixel == sf::Color::Red)
				PlayerSpawn = sf::Vector2f(a, b);
			else
			{
				map.push_back(CellType::Empty);
			}
		}

	}
	sf::RenderWindow window(sf::VideoMode(width, height), "CoraEngine 1.2");

	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(fps);
	options.antialiasingLevel = 8;
	bool activeMouse = false;
	while (window.isOpen())
	{
		std::cout << activeMouse << std::endl;
		sf::IntRect windowRect(window.getPosition(), sf::Vector2i(window.getSize()));
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					player.Posx += dCos(player.angleX - 90) * vitesse;
					player.Posy += dSin(player.angleX - 90) * vitesse;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					player.Posx += dCos(player.angleX + 90) * vitesse;
					player.Posy += dSin(player.angleX + 90) * vitesse;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
					//if (!isCollision(player.Posx + cos(player.angleX * PI / 180) * vitesse, player.Posy - sin(player.angleX * PI / 180) * vitesse)) {
					player.Posx += dCos(player.angleX) * vitesse;
					player.Posy -= dSin(player.angleX) * vitesse;
					//}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
					//if (!isCollision(player.Posx - cos(player.angleX * PI / 180) * vitesse, player.Posy + sin(player.angleX * PI / 180) * vitesse)) {
					player.Posx -= dCos(player.angleX) * vitesse;
					player.Posy += dSin(player.angleX) * vitesse;
					//}
				}
			}

		}

		//Minimap
		for (int i = 0; i < mapSizeX * mapSizeY; i++) {
			if (map[i] == CellType::Wall)
			{
				window.draw(generate_map(i, mapSizeX, mapSizeY));
			}
		}
		window.draw(player.body);
		window.display();
	}
	return 0;
};

sf::RectangleShape generate_map(int i, int nb_case_w, int nb_case_h) {
	float width_map = width / rate_map;
	float height_map = height / rate_map;
	sf::RectangleShape border(sf::Vector2f(width_map / nb_case_w, height_map / nb_case_h));
	border.setPosition(i % nb_case_w * width_map / nb_case_w, std::floor(i / nb_case_h) * height_map / nb_case_h);
	return border;
}

bool isCollision(int x, int y, int nb_case_w, int nb_case_h) {
	int list_indice[4]{
		(x + rayon * 2) / (width / nb_case_w) + (y + rayon * 2) / (height / nb_case_h) * nb_case_w,
		x / (width / nb_case_w) + y / (height / nb_case_h) * nb_case_w,
		(x + rayon * 2) / (width / nb_case_w) + y / (height / nb_case_h) * nb_case_w,
		x / (width / nb_case_w) + (y + rayon * 2) / (height / nb_case_h) * nb_case_w
	};
	if (map[list_indice[0]] == CellType::Empty || map[list_indice[1]] == CellType::Wall || map[list_indice[2]] == CellType::Wall || map[list_indice[3]] == CellType::Wall) {
		return true;
	}
	return false;
}


inline float toRadian(float degree) { return (PI / 180) * degree; }
inline float dCos(float degree) { return cos(toRadian(degree)); }
inline float dSin(float degree) { return sin(toRadian(degree)); }
inline float dTan(float degree) { return tan(toRadian(degree)); }