#include "system.h"

class ComponentManager;
class TileMap;
class ComponentManager::EntityComponents;
struct CollisionComponent;

class AISystem : public System
{
public:
	AISystem(ComponentManager& compManager, const TileMap& tileMap);
	void update(int deltaTime);
private:
	const TileMap& tileMap;

	bool pathBlocked(ComponentManager::EntityComponents* entity, CollisionComponent* collision, float dx, float dy);
};