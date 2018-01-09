#pragma once

#include <assimp/material.h>
#include <map>
#include <glm/glm.hpp>

#include "texture.hpp"


class Material
{

	std::map<aiTextureType, Texture> textures;


public:
	Material(aiMaterial* mat, const std::string& model_dir);


};