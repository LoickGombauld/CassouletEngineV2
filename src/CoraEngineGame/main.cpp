#include <iostream>
#include <CoraEngineLibrarie/Window.hpp>
#include <CoraEngineLibrarie/Global.hpp>
#include  <CoraEngineLibrarie/Player.hpp>
#include  <CoraEngineLibrarie/Entity.hpp>
#include  <CoraEngineLibrarie/TextureManager.hpp>


int main()
{
	std::cout << "Welcome to Fantasy Zone Get Ready !" << std::endl;
	Window mapWindow(HEIGHTSCREEN, HEIGHTSCREEN, "Map");
	mapWindow.GetHandle()->setPosition({ mapWindow.GetHandle()->getPosition() / 6 });
	mapWindow.SetFrameRate(144);
	mapWindow.SetVerticalSyncEnable(true);
	Window window(WIDTHSCREEN, HEIGHTSCREEN, "Cora Engine (Ou Cassoulet Engine 2 why not)");
	window.SetFrameRate(144);
	window.SetVerticalSyncEnable(true);
	window.SetMouseCursorVisible(false);

	Fonts fonts;
	TextureManager FloorManager("Floor");
	TextureManager WallManager("Wall");
	TextureManager charManager("CharacterSprite");


	Map map;
	Player player(window, WallManager, FloorManager, map);
	std::vector<Entity> entities;
	entities.emplace_back(Entity(*charManager.GetTexture(0), map));
	map.SpawnPlayerOnMap(player);
	for (auto& entity : entities)
	{
		entity.SetOriginPosition(map.GetSpawnerCell(0).GetCellPosition());
	}

	while (window.isOpen())
	{
		sf::IntRect WindowRect(window.GetHandle()->getPosition(), static_cast<sf::Vector2i>(window.GetSize()));
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
		if (WindowRect.contains(sf::Mouse::getPosition()) & sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			player.SetMouse(true);
		}
		player.Update();
		for (auto& entity : entities) {
			entity.Update();
		}
		window.Clear(sf::Color::Cyan);
		mapWindow.Clear();

		map.Draw(mapWindow, WallManager.GetTexture(0), FloorManager.GetTexture(0));
		for (auto& entity : entities)
		{
			player.Draw(mapWindow, entity);
			entity.DrawOnMap(mapWindow);
		}


		window.Display();
		mapWindow.Display();
	}
	return 0;
};
