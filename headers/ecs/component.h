#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "enums.h"
#include "rectangle.h"

//forward declare
class Ability;

struct Component
{
	Component(int entityId) : entityId(entityId) {}
	virtual ~Component() {}
	//bool exists = true;
	int entityId = -1;
};

struct InactiveComponent : public Component
{
	InactiveComponent(int entityId) : Component(entityId) {}
};

struct PositionComponent : public Component
{
	PositionComponent(int entityId, float x, float y, float w, float h) : Component(entityId), x(x), y(y), width(w), height(h){}
	float x;
	float y;
	float width;
	float height;
};

struct SpriteComponent : public Component
{
	SpriteComponent(int entityId, std::string filePath, int tileNum, int width, int height, int layer) : Component(entityId), filePath(filePath), 
		tileNum(tileNum), width(width), height(height), layer(layer) 
	{}
	SpriteComponent(int entityId, std::string filePath, int tileNum, int width, int height, int layer, bool flipVert, bool flipHoriz, bool flipDiag) : 
		Component(entityId), filePath(filePath), tileNum(tileNum), width(width), height(height), layer(layer), flipVertical(flipVert), flipHorizontal(flipHoriz), flipDiagonal(flipDiag)
	{}
	std::string filePath;
	int tileNum;
	int width;
	int height;
	int layer;
	bool flipVertical = false;
	bool flipHorizontal = false;
	bool flipDiagonal = false;
	float xOffset = 0;
	float yOffset = 0;
	bool visible = true;
};

struct MovementComponent : public Component
{
	MovementComponent(int entityId, float dx, float dy, float moveSpeed) : Component(entityId), dx(dx), dy(dy), moveSpeed(moveSpeed) {}
	float dx;
	float dy;
	float xAcceleration = 0;
	float yAcceleration = 0;
	float moveSpeed;
};

struct PlayerComponent : public Component
{
	PlayerComponent(int entityId) : Component(entityId) {}
};

struct StateComponent : public Component
{
	StateComponent(int entityId, DrawState draw, Direction facing, int drawBuffer, ActivityState activity = ActivityState::alive) : 
		Component(entityId), drawState(draw), bufferedDrawState(drawState), drawBufferMax(drawBuffer), activityState(activity), facing(facing)
	{}

	Direction facing;
	const int drawBufferMax;
	int drawBuffer = 0;

	void setDrawState(DrawState dState)
	{
		drawState = dState;
		bufferedDrawState = dState;
	}
	void setBufferedDrawState(DrawState dState)
	{
		if (bufferedDrawState != dState)
		{
			bufferedDrawState = dState;
			drawBuffer = 0;
		}
	}

	DrawState getDrawState() const
	{
		return drawState;
	}

	DrawState getBufferedDrawState() const
	{
		return bufferedDrawState;
	}

	ActivityState activityState;
	DrawState previousDrawState = DrawState::none;
	int stunTimer = 0;

	bool invincible = false;
	int invincibilityTimer = 0;

	bool flashing = false;

	// how long entity sticks around after death
	int timeToDie = 0;
	int deathTimer = 0;

	// if lifetime > 0, entity dies after lifetime milliseconds
	int lifetime = -1;
	int lifeTimer = 0;

	// how long entity stays invincible as a multiplier compared to normal
	float invTimeFactor = 1.0f;

private:
	DrawState drawState;
	DrawState bufferedDrawState;
};

struct AnimationFrame
{
	int tileNum = 0;
	bool flipDiagonal = false;
	bool flipHorizontal = false;
	bool flipVertical = false;
	float xOffset = 0.f;
	float yOffset = 0.f;

	AnimationFrame() {}

	AnimationFrame(int tileNum) : tileNum(tileNum)
	{}

	AnimationFrame(int tileNum, bool flipDiagonal, bool flipHorizontal, bool flipVertical) 
		: tileNum(tileNum), flipDiagonal(flipDiagonal), flipHorizontal(flipHorizontal), flipVertical(flipVertical)
	{}

	AnimationFrame(int tileNum, bool flipDiagonal, bool flipHorizontal, bool flipVertical, float xOffset, float yOffset) 
		: tileNum(tileNum), flipDiagonal(flipDiagonal), flipHorizontal(flipHorizontal), flipVertical(flipVertical), xOffset(xOffset), yOffset(yOffset)
	{}
};

struct Animation
{
	std::vector<AnimationFrame> frameList;
	bool loop = true;

	Animation() {}

	Animation(std::vector<AnimationFrame> frameList) : frameList(frameList) 
	{}

	Animation(std::vector<AnimationFrame> frameList, bool loop) : frameList(frameList), loop(loop)
	{}
};

struct AnimationSet
{
	std::unordered_map<DrawState, Animation> animationMap;
	std::string filePath = "";
	int initialTileNum = 0;
	int spriteWidth = 0;
	int spriteHeight = 0;
	int spriteLayer = 0;
	int timeToUpdate = 0;
};

struct AnimationComponent : public Component
{
	AnimationComponent(int entityId, int timeToUpdate, const std::unordered_map<DrawState, Animation>* animationMap) :
		Component(entityId), timeToUpdate(timeToUpdate), animationMap(animationMap)
	{}
	DrawState prevState = DrawState::standDown;
	int timeElapsed = 0;
	int frameNum = 0;
	int timeToUpdate;
	const std::unordered_map<DrawState, Animation>* animationMap;

	bool flashing = false;
	int flashTimer = 0;
	int flashSpeed = 100;
};

struct CollisionComponent : public Component
{
	CollisionComponent(int entityId, Rectangle<float> boundingBox, bool solid, bool interactable) 
		: Component(entityId), boundingBox(boundingBox), solid(solid), interactable(interactable)
	{}
	bool solid = false;
	bool collideWithEntities = true;
	bool interactable = false;
	Rectangle<float> boundingBox;
};

struct HealthComponent : public Component
{
	HealthComponent(int entityId, int health) : Component(entityId), health(health)
	{}
	int health;
};

struct DamageComponent : public Component
{
	DamageComponent(int entityId, int damage, std::vector<Group> damageGroups, Direction direction = Direction::none) 
		: Component(entityId), damage(damage), damageGroups(damageGroups), direction(direction)
	{}
	int damage;
	std::vector<Group> damageGroups;
	float knockback = 0.2f;
	float knockbackDeceleration = 0.0004f;
	Direction direction;
};

struct GroupComponent : public Component
{
	GroupComponent(int entityId, Group group, std::vector<Group> hostileGroups) : Component(entityId), group(group), hostileGroups(hostileGroups)
	{}
	Group group;
	std::vector<Group> hostileGroups;
};

struct AbilityComponent : public Component
{
	AbilityComponent(int entityId) : Component(entityId)
	{}

	std::unordered_map<int, Ability*> abilityMap;
};

struct AIComponent : public Component
{
	AIComponent(int entityId, Behavior behavior) : Component(entityId), behavior(behavior)
	{}

	Behavior behavior = Behavior::none;
	int timer = 0;
};




