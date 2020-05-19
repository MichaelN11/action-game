#pragma once

#include <string>

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
		tileNum(tileNum), width(width), height(height), layer(layer) {}
	std::string filePath;
	int tileNum;
	int width;
	int height;
	int layer;
};

struct MovementComponent : public Component
{
	MovementComponent(int entityId, float dx, float dy, float moveSpeed) : Component(entityId), dx(dx), dy(dy), moveSpeed(moveSpeed) {}
	float dx;
	float dy;
	float moveSpeed;
};

struct PlayerComponent : public Component
{
	PlayerComponent(int entityId) : Component(entityId) {}
};