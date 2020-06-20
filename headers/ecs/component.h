#pragma once

#include <string>

#include "enums.h"
#include "rectangle.h"

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
	PositionComponent(int entityId, float x, float y) : Component(entityId), x(x), y(y) {}
	float x;
	float y;
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
	StateComponent(int entityId, DrawState draw, int drawBuffer, ActivityState activity = ActivityState::alive) : 
		Component(entityId), drawState(draw), bufferedDrawState(drawState), drawBufferMax(drawBuffer), activityState(activity)
	{}

	const int drawBufferMax;
	int drawBuffer = 0;

	void setDrawState(DrawState dState)
	{
		drawState = dState;
		bufferedDrawState = dState;
	}
	void setBufferedDrawState(DrawState dState)
	{
		bufferedDrawState = dState;
		drawBuffer = 0;
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

	int timeToDie = 5000;
	int deathTimer = 0;

	// how long entity stays invincible as a multiplier compared to normal
	const float invTimeFactor = 4.0f;

private:
	DrawState drawState;
	DrawState bufferedDrawState;
};

struct AnimationFrame
{
	int tileNum;
	bool flipDiagonal;
	bool flipHorizontal;
	bool flipVertical;
};

struct Animation
{
	std::vector<AnimationFrame> frameList;
	bool loop;
};

struct AnimationComponent : public Component
{
	AnimationComponent(int entityId, int timeToUpdate, std::unordered_map<DrawState, Animation>* animationMap) :
		Component(entityId), timeToUpdate(timeToUpdate), animationMap(animationMap)
	{}
	DrawState prevState = DrawState::standDown;
	int timeElapsed = 0;
	int frameNum = 0;
	int timeToUpdate;
	std::unordered_map<DrawState, Animation>* animationMap;

	bool flashing = false;
	int flashTimer = 0;
	int flashSpeed = 100;
};

struct CollisionComponent : public Component
{
	CollisionComponent(int entityId, Rectangle<float> boundingBox, bool solid, bool interactable) : Component(entityId), boundingBox(boundingBox), solid(solid), interactable(interactable)
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
	DamageComponent(int entityId, int damage, std::vector<Group> damageGroups) : Component(entityId), damage(damage), damageGroups(damageGroups)
	{}
	int damage;
	std::vector<Group> damageGroups;
	float knockback = 0.2f;
	float knockbackDeceleration = 0.0004f;
};

struct GroupComponent : public Component
{
	GroupComponent(int entityId, Group group, std::vector<Group> hostileGroups) : Component(entityId), group(group), hostileGroups(hostileGroups)
	{}
	Group group;
	std::vector<Group> hostileGroups;
};

//struct StunComponent : public Component
//{
//	StunComponent(int entityId, DrawState previousDrawState, int recoveryTimer) : Component(entityId), previousDrawState(previousDrawState), recoveryTimer(recoveryTimer)
//	{}
//	DrawState previousDrawState = DrawState::none;
//	int recoveryTimer = 0;
//};


