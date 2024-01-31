
#include <CoraEngineLibrarie/Game.h>



void Entity::UpdateCollider(std::function<void(Entity&)> onCollisionEnter, Game* game, const std::string& tag)
{
    for (auto& [key,entity] : game->props)
	{
		if (1 > sqrt(pow(position.x - entity.position.x,2) + pow(position.y - entity.position.y,2))&& entity.tag == tag)
		{
			onCollisionEnter(entity);
		}
	}
}
