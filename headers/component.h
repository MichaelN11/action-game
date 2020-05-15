#pragma once

struct Component
{
	bool exists = true;
	int entityId = -1;
	virtual ~Component() {}
};

struct InactiveComponent : public Component
{};