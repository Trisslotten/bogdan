#pragma once
#include "window.hpp"
#include "shader.hpp"

class EngineState
{
public:
	virtual bool shouldClose() = 0;
	virtual void init() = 0;
	virtual EngineState* tick(Window&) = 0;
	virtual void render(Window& window) = 0;
};


class PlayingState : public EngineState
{
	ShaderProgram shader;

public:
	virtual bool shouldClose();

	virtual void init();

	virtual EngineState* tick(Window&);

	virtual void render(Window& window);
};