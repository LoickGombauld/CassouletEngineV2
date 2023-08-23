#include <CoraEngineLibrarie/Shape.hpp>

Shape::Shape(sf::CircleShape& p_shape) : m_cshape(p_shape)
{
}
Shape::Shape(sf::RectangleShape& p_shape) : m_rshape(p_shape)
{
}

Shape::Shape( )
{
		m_cshape = sf::CircleShape(5);
		m_rshape = sf::RectangleShape();
}




Shape::~Shape()
{
}

void Shape::SetRadius(float radius)
{
	m_cshape.setRadius(radius);
}

float Shape::GetRadius() const
{
	return  m_cshape.getRadius();
}

sf::Vector2f Shape::GetPosition() const
{
	return m_cshape.getPosition();
}
void Shape::Move(sf::Vector2f move)
{
	m_cshape.move(move);
}
void Shape::SetFillColor(sf::Color color) {
	m_cshape.setFillColor(color);
}

void Shape::SetOrigin(sf::Vector2f vector2)
{
	m_cshape.setOrigin(vector2);
}

void Shape::SetPostion(sf::Vector2f vector2)
{
	m_cshape.setPosition(vector2);
}

void Shape::SetRotation(float angle) { m_cshape.setRotation(angle); }

sf::CircleShape Shape::GethandleCircle()
{
	return m_cshape;
}

sf::RectangleShape Shape::GethandleRect()
{
	return m_rshape;
}

sf::FloatRect Shape:: GetGlobalRectBounds() const
{
	return m_rshape.getGlobalBounds();
}
sf::FloatRect Shape:: GetGlobalCircleBound() const
{
	return m_rshape.getGlobalBounds();
}

sf::Vector2f Shape::GetRectSize() const
{
	return m_rshape.getSize();
}
