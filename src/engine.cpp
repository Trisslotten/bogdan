#include "engine.hpp"

#include <iostream>
#include "entity.hpp"
#include "component.hpp"


bool PlayingState::shouldClose()
{
	return false;
}

void PlayingState::init()
{
	Timer t;
	t.restart();
	for (int i = 0; i < 1000000; i++)
	{
		Entities::createEntity<Transform, Orientation>();
	}
	std::cout << t.elapsed() << "\n";
	t.restart();
	auto es = Entities::getEntities<>();
	std::cout << t.elapsed() << " | size = " << es.size() << "\n";
	t.restart();
	for (int i = 0; i < 1000000; i++)
	{
		Entities::removeEntity(es[i]);
	}
	std::cout << t.elapsed() << "\n";
}

EngineState* PlayingState::update(Window& window)
{



	return nullptr;
}

void PlayingState::render()
{}
