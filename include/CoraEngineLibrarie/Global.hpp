#pragma once
#include  <CoraEngineLibrarie/Export.hpp>
#ifndef VARIABLES_HPP_INCLUDED

const float PI = 3.1415f;
const unsigned int xCase = 30, yCase = 19;
const unsigned char blockSize = 30;
const unsigned int WIDTHSCREEN = xCase * blockSize,

HEIGHTSCREEN = yCase * blockSize;
const float RENDER_DISTANCE = 1024;
const float PlayerColliderRadius = 1;
const unsigned int Radius = 10;
const float SpeedMove = 100;
const float SpeedAngle = 200;

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
	Cell() :m_CellPosition(sf::Vector2f(0, 0)), m_CellType(CellType::Empty),m_shape(sf::RectangleShape()) {}
	Cell(sf::Vector2f& cellposition, CellType cellType, sf::RectangleShape& m_shape) :m_shape(m_shape), m_CellPosition(cellposition), m_CellType(cellType) {}
	void SetPos(sf::Vector2f& position)
	{
		m_CellPosition = position;
	}

	sf::FloatRect CellBounds()
	{
		return m_shape.getGlobalBounds();
	}

	void SetCellType(CellType cell)
	{
		m_CellType = cell;
	}

	void UpdateCellColision(sf::Vector2f& entityPosition)
	{
		if (entityPosition.x < m_shape.getGlobalBounds().left)
		{
			entityPosition.x = m_shape.getGlobalBounds().left;
		}
		if (entityPosition.x < m_shape.getGlobalBounds().top)
		{
			entityPosition.y = m_shape.getGlobalBounds().top;
		}
	}

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
	CellType m_CellType = CellType::Empty;
	sf::RectangleShape m_shape;
};

#define VARIABLES_HPP_INCLUDED
#endif // VARIABLES_HPP_INCLUDED