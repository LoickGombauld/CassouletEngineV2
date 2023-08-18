#include <CoraEngineLibrarie/Export.hpp>
#include <SFML/Graphics.hpp>



class CASSOULET_DLL SFMLPP
{
public:
	SFMLPP();
	SFMLPP(const SFMLPP&) = delete;
	SFMLPP(SFMLPP&&) = delete;
	~SFMLPP();

	sf::Event::EventType GetEventType(sf::Event* event);

	SFMLPP& operator=(const SFMLPP&) = delete;
	SFMLPP& operator=(SFMLPP&&) = delete;

};

SFMLPP::SFMLPP()
{
}

SFMLPP::~SFMLPP()
{
}

sf::Event::EventType SFMLPP::GetEventType(sf::Event* event)
{
	return event->type;
}
