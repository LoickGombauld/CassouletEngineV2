#pragma once

#include <CoraEngineLibrarie/Player.hpp>

class CASSOULET_DLL Entity
{

protected:
	EntityInfo info;
public:
	Entity();
	Entity(sf::Texture& entitySprite, Map& map);
	sf::Vector2f GetPos();
	float get_center_x() const;
	float get_center_y() const;
	float GetDirection() const;
	void DrawOnMap(Window& i_window);
	void SetPosition(float i_x, float i_y);
	void SetOriginPosition(float i_x, float i_y);
	void SetOriginPosition(sf::Vector2f pos);
	void Update();
	void CheckCollisionWithWalls(sf::Vector2f& movement);
	void SetScale(sf::Vector2f& scale);
	void Reset();
	sf::Sprite GetSprite();


private:
	bool map_collision(float i_x, float i_y);
	inline float toRadian(float degree) { return (PI / 180) * degree; }
	inline float dCos(float degree) { return cos(toRadian(degree)); }
	inline float dSin(float degree) { return sin(toRadian(degree)); }
	inline float dTan(float degree) { return tan(toRadian(degree)); }
	sf::Sprite m_entitySprite;
	sf::Vector2f m_originPos;
	sf::Vector2f m_originSize;
	Map m_map;
};
