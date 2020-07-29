#pragma once
#include "component.h"

struct EntityData
{
	std::string spritePath = "";
	int tileNum = -1;
	int spriteWidth = -1;
	int spriteHeight = -1;
	int spriteLayer = -1;
	float moveSpeed = -1.f;
	bool player = false;
	std::unordered_map<DrawState, Animation> animationMap;
	int animationTimeToUpdate = -1;
	Rectangle<float> boundingBox;
	bool solid = false;
	bool interactable = false;
	int health = -1;
	int damage = -1;
	std::vector<Group> damageGroups;
	Group group;
	std::vector<Group> hostileGroups;
	int lifetime = -1;
	float width;
	float height;
	std::vector<std::string> abilityList;
};