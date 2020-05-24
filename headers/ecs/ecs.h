#pragma once
#include "ecs/componentmanager.h"
#include "ecs/playersystem.h"
#include "ecs/positionupdatesystem.h"
#include "ecs/spritesystem.h"
#include "ecs/animationsystem.h"
#include "ecs/collisionsystem.h"

// forward declare
class Graphics;
class EventManager;
class TileMap;

struct PlayerAnim
{
	static std::unordered_map<DrawState, std::vector<AnimationFrame>> create_map();
	static std::unordered_map<DrawState, std::vector<AnimationFrame>> map;
};

struct EntityData
{
	std::string spritePath = "";
	int tileNum = -1;
	int spriteWidth = -1;
	int spriteHeight = -1;
	int spriteLayer = -1;
	float moveSpeed = -1.f;
	bool player = false;
	std::unordered_map<DrawState, std::vector<AnimationFrame>>* animationMap;
	int animationTimeToUpdate = -1;
	Rectangle<float> boundingBox;
	bool solid = false;
};

class ECS
{
public:
	ECS(EventManager& eventManager, const TileMap& tileMap);
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

	EventManager& eventManager;
	ComponentManager compManager;
	PlayerSystem playerSystem;
	PositionUpdateSystem posUpdateSystem;
	SpriteSystem spriteSystem;
	AnimationSystem animationSystem;
	CollisionSystem collisionSystem;
};