#pragma once
#include <SFML/Graphics.hpp>
#include <CoraEngineLibrarie/Export.hpp>
#include <filesystem>


class CASSOULET_DLL Fonts
{
public:
	Fonts();
	sf::Font GetFont(int index);
private:

	std::vector< sf::Font> m_fonts;

};

