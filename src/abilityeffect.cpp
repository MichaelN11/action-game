#include "abilityeffect.h"
#include "ecs/entitymanager.h"
#include "eventmanager.h"

effect::SpawnAdjacentEntity::SpawnAdjacentEntity(const EntityData& entity) : entitySpawned(entity)
{}

void effect::SpawnAdjacentEntity::trigger(ComponentManager& compManager, EntityManager& entityManager, EventManager& eventManager, ComponentManager::EntityComponents* source, float x, float y)
{
	DrawState attackState = DrawState::none;

	StateComponent* state = source->getComponent<StateComponent>();
	PositionComponent* position = source->getComponent<PositionComponent>();
	if (state && position)
	{
		float localX = position->x;
		float localY = position->y;

		switch (state->facing)
		{
		case Direction::down:
				localY += position->height;
				attackState = DrawState::attackDown;
				break;
		case Direction::up:
				localY -= position->height;
				attackState = DrawState::attackUp;
				break;
		case Direction::left:
				localX -= position->width;
				attackState = DrawState::attackLeft;
				break;
		case Direction::right:
				localX += position->width;
				attackState = DrawState::attackRight;
				break;
		}

		int attackId = entityManager.createEntity(localX, localY, SWORD, attackState, state->facing);
		eventManager.fireEvent<CollisionCheckEvent>(CollisionCheckEvent(attackId));
	}
}

// booleans = diagonal, horizontal, vertical flips
std::unordered_map<DrawState, Animation> effect::createSwordAnims()
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