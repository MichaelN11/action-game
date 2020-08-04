#pragma once
#include "component.h"

struct EntityData
{
	AnimationSet animSet;
	float moveSpeed = -1.f;
	bool player = false;
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
	Behavior aiBehavior = Behavior::none;
};