#pragma once

#include "rectangle.h"

class ECS;

class GameView
{
public:
	GameView(float width, float height, ECS* ecs);
	void setView(float x, float y);
	void offsetView(float offsetX, float offsetY);
	void update(int deltaTime);
	void setBounds(Rectangle<int> bounds);

	const Rectangle<float>& getView() const;
private:
	Rectangle<float> view;
	Rectangle<int> viewBounds;
	ECS* ecs;
};