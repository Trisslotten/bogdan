#pragma once

#include "transformcomponent.hpp"

template <class... Ts>
struct Types {};


// add new components to this list
using ComponentTypes = Types<
	Transform
>;