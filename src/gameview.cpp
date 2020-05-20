#include "gameview.h"
#include "ecs/ecs.h"

GameView::GameView(float width, float height, ECS* ecs) :
	view(0.f, 0.f, width, height),
	ecs(ecs)
{}

void GameView::setBounds(const Rectangle<int>& bounds)
{
	viewBounds = bounds;
}

void GameView::setView(float x, float y)
{
	if (viewBounds.getW() > 0)
	{
		if (x >= viewBounds.getX() && x + view.getW() < viewBounds.getX2())
			view.setX(x);
		if (y >= viewBounds.getY() && y + view.getH() < viewBounds.getY2())
			view.setY(y);
	}
	else
	{
		view.setX(x);
		view.setY(y);
	}
}

void GameView::offsetView(float offsetX, float offsetY)
{
	setView(view.getX() + offsetX, view.getY() + offsetY);
}

void GameView::update(int deltaTime)
{
	// TEMP CAMERA
	if (ecs)
	{
		std::pair<float, float> playerPos = ecs->getPlayerPosition();
		float x = playerPos.first - (view.getW() / 2);
		float y = playerPos.second - (view.getH() / 2);
		setView(x, y);
	}
}

const Rectangle<float>& GameView::getView() const
{
	return view;
}