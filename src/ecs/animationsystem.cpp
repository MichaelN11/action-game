#include "ecs/animationsystem.h"
#include "ecs/componentmanager.h"

AnimationSystem::AnimationSystem(ComponentManager& compManager) : compManager(compManager)
{}

void AnimationSystem::update(int deltaTime)
{
	auto animationList = compManager.getComponentList<AnimationComponent>();
	for (auto animation : animationList)
	{
		auto entity = compManager.getEntityComponents(animation->entityId);
		auto sprite = entity->getComponent<SpriteComponent>();
		if (sprite)
		{
			auto state = entity->getComponent<StateComponent>();

			// if state changed, set the sprite to frame 0 of the animation
			if (state && animation->prevState != state->drawState)
			{
				animation->frameNum = 0;
				animation->prevState = state->drawState;
				animation->timeElapsed = animation->timeToUpdate;

				//	auto it = animation->animationMap->find(animation->prevState);
				//	if (it != animation->animationMap->end())
				//	{
				//		auto& frameList = it->second;
				//		if (frameList.size() > 0)
				//		{
				//			AnimationFrame& frame = frameList.at(0);
				//			sprite->tileNum = frame.tileNum;
				//			sprite->flipHorizontal = frame.flipHorizontal;
				//			sprite->flipVertical = frame.flipVertical;
				//			sprite->flipDiagonal = frame.flipDiagonal;
				//		}
				//	}
				//}
				//else
			}
			auto it = animation->animationMap->find(animation->prevState);
			if (it != animation->animationMap->end())
			{
				auto& frameList = it->second;
				if (animation->frameNum < (int)frameList.size())
				{
					// animation logic
					animation->timeElapsed += deltaTime;
					if (animation->timeElapsed >= animation->timeToUpdate)
					{
						animation->timeElapsed -= animation->timeToUpdate;
						AnimationFrame& frame = frameList.at(animation->frameNum);
						sprite->tileNum = frame.tileNum;
						sprite->flipHorizontal = frame.flipHorizontal;
						sprite->flipVertical = frame.flipVertical;
						sprite->flipDiagonal = frame.flipDiagonal;

						if (animation->frameNum + 1 >= (int)frameList.size())
						{
							animation->frameNum = 0;
						}
						else
						{
							++animation->frameNum;
						}
					}
				}
			}
		}
	}
}