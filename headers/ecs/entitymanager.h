#pragma once

#include "ecs/component.h"
#include <memory>
#include "ecs/abilitymanager.h"
#include "entitydata.h"

class ComponentManager;
class EventManager;

class EntityManager
{
public:
	EntityManager(ComponentManager& compManager, EventManager& eventManager);
	
	// returns entity id
	int createEntity(const EntityData& data);
	int createEntity(float x, float y, const EntityData& data);
	int createEntity(float x, float y, const EntityData& data, DrawState drawState, Direction direction);

	void destroyEntity(int entityId);

private:
	ComponentManager& compManager;
	EventManager& eventManager;
	AbilityManager abilityManager;

	int nextEntityId = 0;
	std::vector<int> unusedEntityIds;

	int getNextEntityId();
	void createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data, DrawState drawState, Direction direction);
};