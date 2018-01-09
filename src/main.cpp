
#include "window.hpp"
#include "engine.hpp"
#include "logger.hpp"

int main()
{
	Log::showDebug(true);
	Log::showError(true);
	Log::showWarning(true);
	Log::showOutput(true);

	Window window;
	EngineState* engine;

	window.open(1280, 720);
	engine = new PlayingState();
	engine->init();
	while (!window.shouldClose() && !engine->shouldClose())
	{
		EngineState* new_state = engine->tick(window);
		if (new_state)
		{
			delete engine;
			engine = new_state;
			engine->init();
		}
		engine->render(window);
		window.update();
	}
	return 0;
}