#pragma once
#include <array>
#include <CoraEngineLibrarie/Constant.h>
#include <CoraEngineLibrarie/Stripe.h>
#include <CoraEngineLibrarie/Export.hpp>
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/MapManager.hpp>
#include <CoraEngineLibrarie/WeaponManager.hpp>
#include <CoraEngineLibrarie/NpcManager.hpp>

class CASSOULET_DLL Game
{
	bool show_map;

	float hand_offset;

	std::array<Stripe, gbl::SCREEN::WIDTH> stripes;

	//static pointer to the engine object
	static Game* i;


	sf::Event event;


	sf::VertexArray fov_visualization;

	gbl::Animation hand_animation;


public:
	static Game* Game::I();
	sf::RenderWindow window;

	int Game::get_width(SpriteManager& sprite_manager, float distance, std::string name) const;

	void Game::DrawAnimation(const sf::Vector2<short>& i_position,
		sf::RenderWindow& i_window, const gbl::SpriteData& sprite_data, const gbl::Animation& animation, const bool i_mirror_horizontal = 0,
		const bool i_mirror_vertical = 0, const float i_scale_x = 1,
		const float i_scale_y = 1, const sf::Color& i_color = sf::Color(255, 255, 255),
		const sf::Rect<unsigned short>& i_texture_box = sf::Rect<unsigned short>(0, 0, USHRT_MAX, USHRT_MAX))const ;
	MapManager managerMap;

	SpriteManager sprite_manager;

	WeaponManager weapon_manager;

	NpcManager npc_manager;

	entt::entity playerEntity;

	gbl::MAP::Map<> map;

	entt::registry m_registry;

	Game(entt::registry& registry);

	void GameOver();
	bool is_open() const;

	void calculate_fov_visualization(entt::registry& registry);
	void Render(entt::registry& registry);
	void draw_map(entt::registry& registry);
	void handle_events();
	void GameRaycast(entt::registry& registry);
	NpcManager* GetNpcData();
	void set_title(const std::string& i_title);
	void Update(float deltaTime, entt::registry& registry);
};

namespace Item {

	enum ItemType {
		NONE, ACTIVE, PASSIVE, FAMILLIER
	};
	struct ItemData {
		int index;
		int rarety;
		std::string name;
		virtual void UseItem(Game* game, entt::registry& registry) { std::cout << "UseItem"; }
		virtual ItemType GetItemType() { std::cout << "Get Item Type"; };
	};

	struct Active : public ItemData
	{
		int chargeMax;
		int currentCharge;
		sf::Sound activeSound;
		ItemType GetItemType() { return ItemType::ACTIVE; };
	};
	struct ActiveProjeticle : public Active {
		int damageBonus;
		int quantity;
		void UseItem(Game* game, entt::registry& registry) {
			;
		}
	};
}