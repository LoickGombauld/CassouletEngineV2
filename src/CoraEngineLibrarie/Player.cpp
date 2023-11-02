#include <CoraEngineLibrarie/Player.h>

Player::Player() {
	body.setRadius(rayon);
	body.setFillColor(sf::Color::Green);
}
void Player::SetPosition(float x, float y)
{
	Posx = x;
	Posy = y;
}

void Player::UpdatePostion()
{
	body.setPosition(Posx,Posy);
}

