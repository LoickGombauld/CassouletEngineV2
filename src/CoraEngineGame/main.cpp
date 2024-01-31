#include <iostream>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/Game.h> 
#include <CoraEngineLibrarie/SoundManager.hpp>
#include <CoraEngineLibrarie/TextureManager.hpp>


int main()
{
	//Randomizing the seed.
	srand(std::chrono::system_clock::now().time_since_epoch().count());

	//This should be 30 since I switched to 30 FPS.
	std::string fps_text = "CoraEngine 1.3 (Ont fait du neuf avec du vieux) FPS: 60";

	SoundManager::Init();
	TextureManager::Init();
	entt::registry registry;
	Game game(registry);
	sf::Clock clock;
	float lastTime = 0;

	game.Render(registry);

	while (1 == game.is_open())
	{
		float currentTime = clock.restart().asSeconds();
		int fps = 1.f / currentTime;
		lastTime = currentTime;

		//The game lagged a lot during debugging so I needed to see the FPS.
		fps_text = "CoraEngine 1.3 (Ont fait du neuf avec du vieux)  FPS: " + std::to_string(fps);
		game.handle_events();
		game.Update(currentTime, registry);
		game.Render(registry);
		game.set_title(fps_text);
	}
}
