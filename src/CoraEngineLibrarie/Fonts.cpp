
#include <iostream>
#include <CoraEngineLibrarie/Fonts.hpp>

Fonts::Fonts()
{
    for (const auto& entry : std::filesystem::directory_iterator("Resources/Font")) {

        sf::Font font;
        font.loadFromFile(entry.path().string());
        m_fonts.push_back(font);
        std::cout << entry.path().filename().string() << " Has added " << std::endl;
    }
}



sf::Font Fonts::GetFont(int index)
{
	if (index > m_fonts.size())
	{
        index = 0;
	}
	else if(index< 0)
	{
		index = m_fonts.size();
	}
	return m_fonts[index];
}
