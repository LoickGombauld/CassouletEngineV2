#include <iostream>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/Game.h> 


int main()
{
	//Randomizing the seed.
	srand(std::chrono::system_clock::now().time_since_epoch().count());

	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	//This should be 30 since I switched to 30 FPS.
	std::string fps_text = "CoraEngine 1.3 (Ont fait du neuf avec du vieux) FPS: 60";

	Game game;

	previous_time = std::chrono::steady_clock::now();

	game.draw();

	while (1 == game.is_open())
	{
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

		lag += delta_time;

		previous_time += delta_time;

		//The game lagged a lot during debugging so I needed to see the FPS.
		fps_text = "CoraEngine 1.3 (Ont fait du neuf avec du vieux)  FPS: " + std::to_string(1000000.f / delta_time.count());

		while (gbl::SCREEN::FRAME_DURATION <= lag)
		{
			lag -= gbl::SCREEN::FRAME_DURATION;

			game.handle_events();
			game.update();

			if (gbl::SCREEN::FRAME_DURATION > lag)
			{
				game.draw();
				game.set_title(fps_text);
			}
		}
	}
}