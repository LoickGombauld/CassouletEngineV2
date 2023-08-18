#pragma once
#include  <SFML/Graphics.hpp>


struct Math
{
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
	double static  CalculateDistance(sf::Vector2f a, sf::Vector2i b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	}

	float static  deg_to_rad(float i_degrees)
	{
		//After googling I found out that smart people use radians because of calculus.
		//I don't know what that word means so I'll keep using degrees.
		return PI * i_degrees / 180;
	}

	float static  get_degrees(float i_degrees)
	{
		//Thank you, Stackoverflow!
		return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
	}

};


