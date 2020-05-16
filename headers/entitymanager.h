#pragma once

#include <string>
#include "componentmanager.h"

struct EntityData
{
	std::string spritePath = "";
	int tileNum = -1;
	int spriteWidth = -1;
	int spriteHeight = -1;
	float moveSpeed = -1.f;
	bool player = false;
};

class EntityManager
{
public:
	void createEntity(ComponentManager& compManager, const EntityData& data);
	void createEntity(ComponentManager& compManager, float x, float y, const EntityData& data);

	static const EntityData PLAYER;
	static const EntityData DUMMY;
private:
	int nextId = 0;

	int getNextId();
	void createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data);
};