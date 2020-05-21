#pragma once

#include <string>

#include "enums.h"

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

struct StateComponent : public Component
{
	StateComponent(int entityId, DrawState drawState) : Component(entityId), drawState(drawState)
	{}
	DrawState drawState;
};

struct AnimationFrame
{
	int tileNum;
	bool flipDiagonal;
	bool flipHorizontal;
	bool flipVertical;
};

struct AnimationComponent : public Component
{
	AnimationComponent(int entityId, int timeToUpdate, std::unordered_map<DrawState, std::vector<AnimationFrame>>* animationMap) :
		Component(entityId), timeToUpdate(timeToUpdate), animationMap(animationMap)
	{}
	DrawState prevState = DrawState::standDown;
	int timeElapsed = 0;
	int frameNum = 0;
	int timeToUpdate;
	std::unordered_map<DrawState, std::vector<AnimationFrame>>* animationMap;
};