#pragma once
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Global.hpp>
#include <CoraEngineLibrarie/Export.hpp>
#include <CoraEngineLibrarie/Window.hpp>
#include <CoraEngineLibrarie/Shape.hpp>
#include  <CoraEngineLibrarie/Map.hpp>
#include  <CoraEngineLibrarie/Global.hpp>
#include <CoraEngineLibrarie/Math.hpp>

struct CASSOULET_DLL CameraInfo {
public:
	Shape m_camera;
	sf::Vector2f m_position, m_intersection;

	float m_angle, m_rayLength;
	float m_radius;
	float m_speedMove, m_speedAngle;
	float m_colliderRadius;
	sf::Clock m_clock;
};
class CASSOULET_DLL Player
{
protected:
	Window* m_renderWindow;
	CameraInfo info;
	std::vector<sf::VertexArray>m_vecRays;
	Map m_map;

public:
	Player(Window& window, Map& map);
	void Draw() const;
	void Update();
	void Projection();
	void UpdateKeyboardHit(sf::Time dt);
	bool Intersect(unsigned int it);
	void SetPosition(sf::Vector2f);
	void SetPosition(float,float);
	sf::Vector2f GetPosition() const;
	inline float toRadian(float degree) { return (PI / 180) * degree; }
	inline float dCos(float degree) { return cos(toRadian( degree)); }
	inline float dSin(float degree) { return sin(toRadian( degree)); }
	inline float dTan(float degree) { return tan(toRadian( degree)); }

private:
	sf::Vector2f previous;
	sf::Vector2f direction;
	sf::Vector2i currentMousePos;
};