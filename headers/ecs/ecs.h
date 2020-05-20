#pragma once
#include "ecs/componentmanager.h"
#include "ecs/playersystem.h"
#include "ecs/positionupdatesystem.h"
#include "ecs/spritesystem.h"

// forward declare
class Graphics;
class EventManager;

struct EntityData
{
	std::string spritePath = "";
	int tileNum = -1;
	int spriteWidth = -1;
	int spriteHeight = -1;
	int spriteLayer = -1;
	float moveSpeed = -1.f;
	bool player = false;
};

class ECS
{
public:
	ECS(EventManager& eventManager);
	void draw(Graphics& graphics, const Rectangle<float>& view);
	void update(int deltaTime, const Rectangle<float>& view);
	
	void createEntity(const EntityData& data);
	void createEntity(float x, float y, const EntityData& data);

	std::pair<float, float> getPlayerPosition();

	static const EntityData PLAYER;
	static const EntityData DUMMY;
private:
	int nextEntityId = 0;

	int getNextEntityId();
	void createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data);

	ComponentManager compManager;
	PlayerSystem playerSystem;
	PositionUpdateSystem posUpdateSystem;
	SpriteSystem spriteSystem;
};