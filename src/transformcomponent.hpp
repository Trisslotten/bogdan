#pragma once

#include "component.hpp"

struct Transform : public Component<Transform, COMPONENT_BIT>
{
	glm::vec3 pos;
	glm::quat orientation;
};
