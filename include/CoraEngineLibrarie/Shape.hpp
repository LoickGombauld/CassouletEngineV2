#pragma once
#include <SFML/Graphics.hpp>
#include  <CoraEngineLibrarie/Export.hpp>


class CASSOULET_DLL Shape
{
public:
	Shape(sf::CircleShape&);
	Shape(sf::RectangleShape& p_shape);
	Shape();
	~Shape();
	void SetRadius(float radius);
	float GetRadius() const;
	sf::Vector2f GetPosition() const;
	void Move(sf::Vector2f move);
	void SetFillColor(sf::Color color);
	void SetOrigin(sf::Vector2f vector2);
	void SetPostion(sf::Vector2f vector2);
	void SetRotation(float angle);
	sf::CircleShape GethandleCircle();
	sf::RectangleShape GethandleRect();
	sf::FloatRect GetGlobalRectBounds()const;
	sf::FloatRect GetGlobalCircleBound()const;
	sf::Vector2f GetRectSize()const;
private:
	sf::CircleShape m_cshape;
	sf::RectangleShape m_rshape;
};