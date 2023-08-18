#include <iostream>
#include <CoraEngineLibrarie/Window.hpp>
#include <CoraEngineLibrarie/Global.hpp>
#include  <CoraEngineLibrarie/Player.hpp>


int main()
{
	std::printf("Welcome to Fantasy Zone Get Ready !");
	Window window(WIDTHSCREEN, HEIGHTSCREEN, "Cora Engine (Ou Cassoulet Engine 2 why not)");
	window.SetFrameRate(144);
	window.SetVerticalSyncEnable(true);
	window.SetMouseCursorVisible(false);
	Map map(window);

	Player player(window, map);
	map.SpawnPlayerOnMap(player);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.PollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.Close();
		}
		player.Update();
		window.Clear(sf::Color::Cyan);

		player.Draw();

		window.Display();
	}
	return 0;
};
