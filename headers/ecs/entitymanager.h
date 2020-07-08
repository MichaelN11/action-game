#pragma once

#include "ecs/component.h"
#include "rectangle.h"
#include "enums.h"
#include <string>
#include <unordered_map>

class ComponentManager;

struct EntityData
{
	std::string spritePath = "";
	int tileNum = -1;
	int spriteWidth = -1;
	int spriteHeight = -1;
	int spriteLayer = -1;
	float moveSpeed = -1.f;
	bool player = false;
	std::unordered_map<DrawState, Animation>* animationMap;
	int animationTimeToUpdate = -1;
	Rectangle<float> boundingBox;
	bool solid = false;
	bool interactable = false;
	int health = -1;
	int damage = -1;
	std::vector<Group> damageGroups;
	Group group;
	std::vector<Group> hostileGroups;
};

class EntityManager
{
public:
	EntityManager(ComponentManager& compManager);
	void createEntity(const EntityData& data);
	void createEntity(float x, float y, const EntityData& data);
	void destroyEntity(int entityId);
private:
	ComponentManager& compManager;

	int nextEntityId = 0;
	std::vector<int> unusedEntityIds;

	int getNextEntityId();
	void createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data);
};