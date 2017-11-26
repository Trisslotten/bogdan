#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <queue>
#include <unordered_map>


#define COMPONENT_BIT 1U << (__COUNTER__)

using EntityID = uint32_t;
using ComponentFlag = uint64_t;

template <class First, class... Rest>
ComponentFlag getFlags()
{
	ComponentFlag flag = First::flag;
	if constexpr(sizeof...(Rest) > 0)
	{
		flag |= getFlags<Rest...>();
	}
	return flag;
}

template <class T, ComponentFlag bit_>
struct Component
{
	static void addComponent(EntityID id)
	{
		auto it = mappings.find(id);
		if (it == mappings.end())
		{
			size_t index;
			if (!free.empty())
			{
				index = free.front();
				free.pop();
			}
			else
				index = components.size();
			mappings[id] = index;
			components.emplace_back();
		}
	}

	static T* getComponent(EntityID id)
	{
		auto it = mappings.find(id);
		if (it == mappings.end())
			return nullptr;

		size_t index = it->second;
		return &components[index];
	}


	static void removeComponent(EntityID id)
	{
		auto it = mappings.find(id);
		if (it == mappings.end())
			return;

		size_t index = it->second;
		free.push(index);

		mappings.erase(it);
	}
	
	
	static void cleanup()
	{
		std::unordered_map<EntityID, size_t> new_mappings;
		std::vector<T> keep_components;

		keep_components.reserve(components.size - free.size()+1);

		int i = 0;
		for (auto map_elem : mappings)
		{
			keep_components.push_back(std::move(components[map_elem.second]));
			new_mappings[map_elem.first] = i;
			i++;
		}
		components = keep_components;
		mappings = new_mappings;
		free = {};
	}
	

	static void removeAll()
	{
		mappings.clear();
		//mappings.reserve(n);
		components.clear();
		//components.reserve(n);
		free = {};
	}

	static ComponentFlag flag;
private:
	static std::unordered_map<EntityID, size_t> mappings;
	static std::vector<T> components;
	static std::queue<size_t> free;
};
template <class T, ComponentFlag bit_>
ComponentFlag Component<T, bit_>::flag = bit_;
template <class T, ComponentFlag bit_>
std::unordered_map<EntityID, size_t> Component<T, bit_>::mappings = {};
template <class T, ComponentFlag bit_>
std::vector<T> Component<T, bit_>::components = {};
template <class T, ComponentFlag bit_>
std::queue<size_t> Component<T, bit_>::free = {};
