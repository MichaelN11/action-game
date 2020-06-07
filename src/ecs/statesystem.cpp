#include "ecs/statesystem.h"

StateSystem::StateSystem(ComponentManager& compManager) : System(compManager)
{}

void StateSystem::update(int deltaTime)
{
	auto stateList = compManager.getComponentList<StateComponent>();
	for (auto state : stateList)
	{
		// small buffer when changing sprites, useful for things like diagonal movement
		if (state->getBufferedDrawState() != state->getDrawState())
		{
			if (state->drawBuffer >= state->drawBufferMax)
			{
				state->setDrawState(state->getBufferedDrawState());
			}
			else
			{
				state->drawBuffer += deltaTime;
			}
		}
	}
}