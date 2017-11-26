#pragma once

#include <type_traits>
#include <cstdint>
#include <unordered_map>
#include "component.hpp"
#include "componenttypes.hpp"

class Entities
{
	static std::unordered_map<EntityID, ComponentFlag> entities;

	static EntityID id_counter;

	template <class ...Ts>
	struct AllComponents;
	template <>
	struct AllComponents<Types<>>
	{
		static void apply(EntityID e) {}
	};
	template <class... Ts>
	struct AllComponents<Types<Ts...>>
	{
		static void remove(EntityID e)
		{
			removeComponents<Ts...>(e);
		}
		static void removeAllComponent()
		{
			removeEveryComponent<Ts...>();
		}
	};
	
	template <class First, class... Rest> 
	static void removeEveryComponent()
	{
		First::removeAll();
		if constexpr (sizeof...(Rest) > 0)
			removeEveryComponent<Rest...>();
	}

	template <class First, class... Rest>
	static void addComponents(typename std::unordered_map<EntityID, ComponentFlag>::iterator& it)
	{
		it->second |= getFlags<First>();
		First::addComponent(it->first);
		if constexpr (sizeof...(Rest) > 0)
			addComponents<Rest...>(it);
	}

	template <class First, class... Rest>
	static void removeComponents(typename std::unordered_map<EntityID, ComponentFlag>::iterator& it)
	{
		it->second &= ~getFlags<First>();
		First::removeComponent(it->first);
		if constexpr (sizeof...(Rest) > 0)
			removeComponents<Rest...>(it);
	}
public:
	template <class... Ts>
	static EntityID createEntity()
	{
		EntityID id = id_counter++;
		entities[id] = 0;
		if constexpr (sizeof...(Ts) > 0)
			addComponents<Ts...>(id);

		return id;
	}
	static void removeEntity(EntityID id)
	{
		auto it = entities.find(id);
		if (it == entities.end())
			return;

		AllComponents<ComponentTypes>::remove(id);
		entities.erase(it);
	}

	template <class First, class... Rest>
	static void addComponents(EntityID id)
	{
		auto it = entities.find(id);
		if (it == entities.end())
			return;
		addComponents<First, Rest...>(it);
	}

	template <class First, class... Rest>
	static void removeComponents(EntityID id)
	{
		auto it = entities.find(id);
		if (it == entities.end())
			return;
		removeComponents<First, Rest...>(it);
	}

	template <class T>
	static T* getComponent(EntityID id)
	{
		return T::getComponent(id);
	}

	template <class... Ts>
	static std::vector<EntityID> getEntities()
	{
		std::vector<EntityID> result;

		uint64_t flags = 0;
		if constexpr(sizeof...(Ts) > 0)
			flags = getFlags<Ts...>();

		for (auto& e : entities)
		{
			if ((e.second & flags) == flags)
			{
				result.push_back(e.first);
			}
		}
		return result;
	}

	static void clear()
	{
		entities = {};
		id_counter = 0;
		AllComponents<ComponentTypes>::removeAllComponent();
	}
};
std::unordered_map<EntityID, ComponentFlag> Entities::entities = {};
EntityID Entities::id_counter = 0;