#pragma once
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Npc.h>

struct LineSegment {
	sf::Vector2f  start, end;
	LineSegment(sf::Vector2f Pstart, sf::Vector2f Pend) : start(Pstart), end(Pend) {};
};
class Raycast {
private:

	static bool isPointWithinLineSegment(const sf::Vector2f& point, const  LineSegment& segment) {
		float crossProduct = (point.y - segment.start.y) * (segment.end.x - segment.start.x) -
			(point.x - segment.start.x) * (segment.end.y - segment.start.y);

		if (std::abs(crossProduct) > 0.00001f) {
			return false;
		}

		float dotProduct = (point.x - segment.start.x) * (segment.end.x - segment.start.x) +
			(point.y - segment.start.y) * (segment.end.y - segment.start.y);

		if (dotProduct < 0) {
			return false;
		}

		float squaredLength = (segment.end.x - segment.start.x) * (segment.end.x - segment.start.x) +
			(segment.end.y - segment.start.y) * (segment.end.y - segment.start.y);

		if (dotProduct > squaredLength) {
			return false;
		}

		return true;
	}

public:
	static Npc* Send(const sf::Vector2f& rayOrigin, float angle, float range, Npc& player)
	{
		float rayDirectionX = std::cos(angle);
		float rayDirectionY = std::sin(angle);

		sf::Vector2f rayEnd = { rayOrigin.x + rayDirectionX * range, rayOrigin.y + rayDirectionY * range };

		LineSegment raySegment = { rayOrigin, rayEnd };
		if (isPointWithinLineSegment(sf::Vector2f(floor( player.get_x()), floor(player.get_y())), raySegment)) {
			return &player; // Retourne le joueur touché
		}

		return nullptr;
	}
};