#include <iostream>
#include <CoraEngineLibrarie/Window.hpp>
#include <CoraEngineLibrarie/Global.hpp>
#include  <CoraEngineLibrarie/Player.hpp>
#include  <CoraEngineLibrarie/TextureManager.hpp>


int main()
{
	std::printf("Welcome to Fantasy Zone Get Ready !");
	Window window(WIDTHSCREEN, HEIGHTSCREEN, "Cora Engine (Ou Cassoulet Engine 2 why not)");
	window.SetFrameRate(144);
	window.SetVerticalSyncEnable(true);
	window.SetMouseCursorVisible(false);
	TextureManager WallManager("Resources/Wall");
	Map map(window);
	Player player(window, map);
	map.SpawnPlayerOnMap(player);
	sf::IntRect WindowRect(window.GetHandle()->getPosition(), static_cast<sf::Vector2i>(window.GetSize()));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.PollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.Close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			player.SetMouse(false);
		}
		if (WindowRect.contains(sf::Mouse::getPosition()) & sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			player.SetMouse(true);
		}

		player.Update();
		window.Clear(sf::Color::Cyan);

		player.Draw();

		map.Draw();

		window.Display();
	}
	return 0;
};
