
#include "window.hpp"
#include "engine.hpp"

int main()
{
	Window window;
	EngineState* engine;

	window.open(1280, 720);
	engine = new PlayingState();
	engine->init();
	while (!window.shouldClose() && !engine->shouldClose())
	{
		EngineState* new_state = engine->update(window);
		if (new_state)
		{
			delete engine;
			engine = new_state;
			engine->init();
		}
		engine->render();
		window.update();
	}
	return 0;
}