#pragma once
#include <array>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/Stripe.h>
#include <CoraEngineLibrarie/Prop.h>
#include <CoraEngineLibrarie/Player.h>
#include <CoraEngineLibrarie/Npc.h>
#include <CoraEngineLibrarie/Export.hpp>
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/MapManager.hpp>

class CASSOULET_DLL Game
{
	bool show_map;

	float hand_offset;

	std::array<Stripe, gbl::SCREEN::WIDTH> stripes;

	std::vector<Prop> props;

	sf::Event event;

	MapManager managerMap;

	sf::RenderWindow window;

	sf::VertexArray fov_visualization;

	SpriteManager sprite_manager;

	Animation hand_animation;

	Player player;

	std::vector<Npc> npc;

	gbl::MAP::Map<> map;
public:
	Game();

	bool is_open() const;

	void calculate_fov_visualization();
	void draw();
	void draw_map();
	void handle_events();
	void raycast();
	void set_title(const std::string& i_title);
	void update();
};