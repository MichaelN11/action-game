#include "system.h"

class ComponentManager;

class AISystem : public System
{
public:
	AISystem(ComponentManager& compManager);
	void update(int deltaTime);
private:
};