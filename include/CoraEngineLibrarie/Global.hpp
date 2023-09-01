#pragma once
#include  <CoraEngineLibrarie/Export.hpp>
#include  <SFML/Graphics.hpp>

#include <CoraEngineLibrarie/Math.hpp>
#ifndef VARIABLES_HPP_INCLUDED

const float PI = 3.1415f;
const unsigned int xCase = 19, yCase = 19;
const unsigned char blockSize = 32;
constexpr unsigned short WIDTHSCREEN = xCase * blockSize,

HEIGHTSCREEN = yCase * blockSize;
const float RENDER_DISTANCE = 200;
const float SpeedMove = 8.f;
const float SpeedAngle = 5;
const float repulsionMagnitude = 10;
const float elasticityCoefficient = 10;
const float interpolationFactor = 25;
const float FOV = 90;

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