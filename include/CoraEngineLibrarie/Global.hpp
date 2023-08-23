#pragma once
#include  <CoraEngineLibrarie/Export.hpp>
#include  <SFML/Graphics.hpp>

#include <CoraEngineLibrarie/Math.hpp>
#ifndef VARIABLES_HPP_INCLUDED

const float PI = 3.1415f;
const unsigned int xCase = 19, yCase = 19;
const unsigned char blockSize = 30;
constexpr unsigned short WIDTHSCREEN = xCase * blockSize,

HEIGHTSCREEN = yCase * blockSize;
const float RENDER_DISTANCE = 100;
const float PlayerColliderRadius = 1;
const unsigned int Radius = 10;
const float SpeedMove = 1.f;
const float SpeedAngle = 5;
const float repulsionMagnitude = 10;
const float elasticityCoefficient = 10;
const float interpolationFactor = 25;
const float Fov = 90;
const float RayLength = 200

;

enum CASSOULET_DLL  CellType
{
	Empty,
	Wall,
	Win
};

class CASSOULET_DLL Cell {

public:
	Cell() {}
	Cell(sf::Vector2f& cellposition, CellType cellType, sf::RectangleShape& m_shape) :m_shape(m_shape), m_CellPosition(cellposition), m_CellType(cellType) {}

	void SetPos(sf::Vector2f& position)
	{
		m_CellPosition = position;
	}

	Cell(CellType celltype ):
	m_CellType(celltype){}

	sf::FloatRect CellBounds()
	{
		return m_shape.getGlobalBounds();
	}

	void SetCellType(CellType cell)
	{
		m_CellType = cell;
	}

	//bool Cell::Intersects(const sf::Vector2f& start, const sf::Vector2f& end) const
	//{
	//	// V�rifier si le rayon commence � l'int�rieur ou � l'ext�rieur du mur
	//	if (m_shape.getGlobalBounds().contains(start))
	//	{
	//		return true; // Le rayon commence ou termine � l'int�rieur du mur
	//	}

	//	// Calculer la direction et la longueur du rayon
	//	sf::Vector2f rayDir = end - start;
	//	float rayLength = Math::length(rayDir);
	//	rayDir /= rayLength; // Normaliser la direction du rayon

	//	// Trouver l'intersection entre le rayon et le mur
	//	// Vous pouvez utiliser des math�matiques vectorielles pour effectuer cette v�rification
	//	// Si l'intersection est d�tect�e, renvoyer true, sinon renvoyer false
	//	// Assurez-vous de prendre en compte les dimensions et la position du mur

	//	return false; // Pas d'intersection d�tect�e
	//}


	//void UpdateCellColision(sf::Vector2f& entityPosition)
	//{
	//	if (entityPosition.x < m_shape.getGlobalBounds().left)
	//	{
	//		entityPosition.x -= m_shape.getGlobalBounds().left;
	//	}
	//	if (entityPosition.x < m_shape.getGlobalBounds().top)
	//	{
	//		entityPosition.y += m_shape.getGlobalBounds().top;
	//	}
	//}

	sf::Vector2f GetCellPosition() const
	{
		return  m_CellPosition;
	};
	CellType GetCellType() const
	{
		return  m_CellType;
	};

	sf::RectangleShape GetShape()
	{
		return  m_shape;
	};

	bool operator==(Cell cell) const
	{
		return  cell.GetCellType() == m_CellType;
	}

	bool operator!=(Cell cell) const
	{
		return  cell.GetCellType() != m_CellType;
	}

	bool operator==(CellType cellType) const
	{
		return  cellType == m_CellType;
	}

	bool operator!=(CellType cellType) const
	{
		return  cellType != m_CellType;
	}

private:
	sf::Vector2f m_CellPosition;
	CellType m_CellType = Empty;
	sf::RectangleShape m_shape;
};

#define VARIABLES_HPP_INCLUDED
#endif // VARIABLES_HPP_INCLUDED