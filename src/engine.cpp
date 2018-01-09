#include "engine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "entity.hpp"
#include "model.hpp"

Model* model;

bool PlayingState::shouldClose()
{
	return false;
}

void PlayingState::init()
{
	shader.create("assets/shaders/shader.vert", "assets/shaders/shader.frag");

	model = new Model("assets/lumber/lumberJack.DAE");
}

EngineState* PlayingState::tick(Window& window)
{
	
	return nullptr;
}

void PlayingState::render(Window& window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto size = window.size();
	glm::mat4 projection = glm::perspectiveFov(glm::radians(90.f), size.x, size.y, 1.f, 100.f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(2, 2, 2),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);

	shader.use();
	shader.uniform("projection", projection);
	shader.uniform("view", view);
	model->debug_draw(shader);
}
