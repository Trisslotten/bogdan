#pragma once

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "window.hpp"
#include "model.hpp"
#include "shader.hpp"

//void recursiveDraw(Model::Node* node, glm::mat4 transform, ShaderProgram & shader);

class Renderer;

class Drawable
{
public:
	virtual void draw(Renderer& renderer) = 0;
};


class Renderer
{
public:
	void draw(Drawable& drawable);

	void draw(Model& model, glm::mat4 world = glm::mat4());
};