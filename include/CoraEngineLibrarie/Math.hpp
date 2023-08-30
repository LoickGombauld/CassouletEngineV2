#pragma once
#include  <SFML/Graphics.hpp>
#include  <CoraEngineLibrarie/Math.hpp>


struct Math
{
	//calculates det product of 2 vectors
	inline static  float det (const sf::Vector2f& lv, const sf::Vector2f& rv)
	{
		return lv.x * rv.x - lv.y * rv.y;
	}

	//calculates dot product of 2 vectors
	inline static  float dot(const sf::Vector2f& lv, const sf::Vector2f& rv)
	{
		return lv.x * rv.x + lv.y * rv.y;
	}

	//Returns length of a given vector
	inline static  float length(const sf::Vector2f& source)
	{
		return std::sqrt(dot(source, source));
	}

	//Returns a given vector with its length normalized to 1
	inline static  sf::Vector2f normalise(sf::Vector2f source)
	{
		float length = std::sqrt(dot(source, source));
		if (length != 0) source /= length;
		return source;
	}

	double static  CalculateDistance(sf::Vector2f a, sf::Vector2f b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	}


	sf::Vector2f static  CalculateNormal(const sf::Vector2f& vector) {
		return sf::Vector2f(-vector.y, vector.x);
	}

	float static  deg_to_rad(float i_degrees)
	{
		//After googling I found out that smart people use radians because of calculus.
		//I don't know what that word means so I'll keep using degrees.
		return 3.1415f * i_degrees / 180;
	}

	float static  get_degrees(float i_degrees)
	{
		//Thank you, Stackoverflow!
		return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
	}

};


