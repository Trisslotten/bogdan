#include "engine.hpp"

#include <iostream>
#include "entity.hpp"


bool PlayingState::shouldClose()
{
	return false;
}

void PlayingState::init()
{
	

}

EngineState* PlayingState::update(Window& window)
{
	
}

void PlayingState::render()
{}
