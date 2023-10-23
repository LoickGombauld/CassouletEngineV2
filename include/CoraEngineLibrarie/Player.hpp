#pragma once
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Global.hpp>
#include <CoraEngineLibrarie/Export.hpp>
#include <CoraEngineLibrarie/Window.hpp>
#include <CoraEngineLibrarie/Shape.hpp>
#include  <CoraEngineLibrarie/Map.hpp>
#include <CoraEngineLibrarie/TextureManager.hpp>

struct CASSOULET_DLL EntityInfo {
public:
	sf::Vector2f m_position, m_intersection;

	float m_angleX, m_angleY, m_rayLength;

	float m_speedMove, m_speedAngle;

	std::array<float, WIDTHSCREEN> view_rays;
};
class CASSOULET_DLL Player
{
protected:
	Window* m_renderWindow;
	EntityInfo info;
	std::vector<sf::VertexArray>m_vecRays;

public:
	Player(Window& window, TextureManager& texture_manager, TextureType type, Map& map);
	Player(Window& window, TextureManager& WallTextures,TextureManager& FloorTextures, Map& map);
	void Draw(Window&,Entity& entity);
	void Update();
	void SetMap(Map& map);
	void SetPosition(sf::Vector2f);
	void SetPosition(float,float);

	void Player::SetMouse(bool isUnlocked);
	sf::Vector2f GetPosition() const;
	inline float toRadian(float degree) { return (PI / 180) * degree; }
	inline float dCos(float degree) { return cos(toRadian( degree)); }
	inline float dSin(float degree) { return sin(toRadian( degree)); }
	inline float dTan(float degree) { return tan(toRadian( degree)); }
	float test = 1;

private:

	void CheckCollisionWithWalls(sf::Vector2f&);
	void Projection();
	bool IntersectSprite(int it,Entity&);
	void UpdateKeyboardHit();
	void UpdateRay();
	bool Intersect(unsigned int it);
	bool map_collision(float i_x, float i_y);
	void SetupPlayer();
	sf::Sprite m_wallSprite;
	sf::Sprite m_floorSprite;
	sf::Vector2f previous;
	TextureManager m_WallTexureManager;
	TextureManager m_FloorTextureManager;
	Shape m_camera;
	Map m_map;

	bool mouseIslocked;
};