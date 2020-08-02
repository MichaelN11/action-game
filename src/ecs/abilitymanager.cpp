#include "ecs/abilitymanager.h"
#include "ecs/statesystem.h"
#include "ability.h"

// DELETE LATER %%%%%%%%%%%%%%%%%%%%%%
#include <iostream>

AbilityManager::AbilityManager(ComponentManager& compManager, EventManager& eventManager, EntityManager& entityManager)
	: System(compManager), eventManager(eventManager), entityManager(entityManager)
{
	int swordDuration = 350;
	Ability swordAttack(compManager, entityManager, eventManager, "Sword Attack", swordDuration);
	swordAttack.addEffect(std::unique_ptr<effect::AbilityEffect>(
		new effect::SpawnAdjacentEntity(SWORD, Rectangle<float>(0, 0, 16, 16), false, true, -1, 10, swordDuration)
		));
	abilityMap.insert({ "swordAttack", std::move(swordAttack) });
}

void AbilityManager::useAbility(ComponentManager::EntityComponents* source, int abilityNum, float x, float y)
{
	std::cout << "got here" << std::endl;
	AbilityComponent* abilities = source->getComponent<AbilityComponent>();
	if (abilities)
	{
		auto it = abilities->abilityMap.find(abilityNum);
		if (it != abilities->abilityMap.end())
		{
			Ability* ability = it->second;
			std::cout << "ability activated" << std::endl;
			ability->activate(source, x, y);

			MovementComponent* movement = source->getComponent<MovementComponent>();
			if (movement)
			{
				movement->dx = 0;
				movement->dy = 0;
				movement->xAcceleration = 0;
				movement->yAcceleration = 0;
			}
			StateComponent* state = source->getComponent<StateComponent>();
			if (state)
			{
				DrawState attackState;
				switch (state->facing)
				{
				case Direction::down:
					attackState = DrawState::attackDown;
					break;
				case Direction::up:
					attackState = DrawState::attackUp;
					break;
				case Direction::left:
					attackState = DrawState::attackLeft;
					break;
				case Direction::right:
					attackState = DrawState::attackRight;
					break;
				}
				StateSystem::setStunned(source, (float)ability->getCastTime(), attackState);
			}
		}
	}
}

// booleans = diagonal, horizontal, vertical flips
std::unordered_map<DrawState, Animation> createSwordAnims()
{
	float offset = 4.f;
	std::unordered_map<DrawState, Animation> map;
	map[DrawState::attackDown] = Animation(std::vector<AnimationFrame>({
		AnimationFrame(1, false, false, true, offset, 0),
		AnimationFrame(2, false, true, true),
		AnimationFrame(1, true, false, false, 0, -offset) }), false);
	map[DrawState::attackUp] = Animation(std::vector<AnimationFrame>({
		AnimationFrame(1, false, false, false, -offset, 0),
		AnimationFrame(2, false, false, false),
		AnimationFrame(1, true, true, false, 0, offset) }), false);
	map[DrawState::attackLeft] = Animation(std::vector<AnimationFrame>({
		AnimationFrame(1, true, false, false, 0, offset),
		AnimationFrame(2, false, true, false),
		AnimationFrame(1, false, false, false, offset, 0) }), false);
	map[DrawState::attackRight] = Animation(std::vector<AnimationFrame>({
		AnimationFrame(1, true, true, false, 0, -offset),
		AnimationFrame(2, false, false, true),
		AnimationFrame(1, false, false, true, -offset, 0) }), false);
	return map;
}

const AnimationSet AbilityManager::SWORD =
{
	// animation map
	createSwordAnims(),
	// sprite file name path
	"content/tilesheets/attack.png",
	// tile number on spritesheet
	2,
	// sprite width
	16,
	// sprite height
	16,
	// sprite layer
	1,
	// animation time to update
	150,
};