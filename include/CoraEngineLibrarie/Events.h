#pragma once
#include <CoraEngineLibrarie/Game.h>
void PlayerTakeDamage(Entity& user)
{
	std::cout << "TakeDamage" << std::endl;
	Npc* npc = &Game::I()->npc[user.id];
	Player* player = &Game::I()->player;
	player->playerdata.life -= npc->data.damage;
	if (player->playerdata.isDead())
		Game::I()->GameOver();
}
