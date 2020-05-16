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

struct TestComponent : public Component
{
	int test = 0;
};

struct PositionComponent : public Component
{
	PositionComponent(int entityId, int x, int y) : Component(entityId), x(x), y(y) {}
	int x = 0;
	int y = 0;
};

struct SpriteComponent : public Component
{
	SpriteComponent(int entityId, std::string filePath, int tileNum, int width, int height) : Component(entityId), filePath(filePath), tileNum(tileNum), width(width), height(height) {}
	std::string filePath;
	int tileNum;
	int width;
	int height;
};