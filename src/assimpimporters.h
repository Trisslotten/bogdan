#pragma once

#include <assimp/Importer.hpp>
#include <map>
#include <future>
#include <thread>

class Importers
{
	static std::map<std::thread::id, Assimp::Importer> importers;
public: 
	static Assimp::Importer* getInstance()
	{
		auto id = std::this_thread::get_id();

		auto it = importers.find(id);

		if (it == importers.end())
		{
			importers[id] = Assimp::Importer();
			return &importers[id];
		}
		else
		{
			return &it->second;
		}
	}
};
std::map<std::thread::id, Assimp::Importer> Importers::importers = {};