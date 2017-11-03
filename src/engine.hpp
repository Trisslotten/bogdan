#pragma once
#include "window.hpp"

class EngineState
{
public:
	virtual bool shouldClose() = 0;
	virtual void init() = 0;
	virtual EngineState* update(Window&) = 0;
	virtual void render() = 0;
};


class PlayingState : public EngineState
{
public:
	virtual bool shouldClose();

	virtual void init();

	virtual EngineState* update(Window&);

	virtual void render();
};