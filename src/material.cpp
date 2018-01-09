#include "material.hpp"
#include "logger.hpp"

Material::Material(aiMaterial * mat, const std::string& model_dir)
{

	auto checkForTexture = [&, this](aiTextureType t)
	{
		if (mat->GetTextureCount(t) == 0)
			return;

		aiString filename;
		mat->Get(AI_MATKEY_TEXTURE(t, 0), filename);

		Texture tex;
		std::string tex_file = model_dir + std::string(filename.C_Str());
		if (tex.load(tex_file))
		{
			textures[t] = std::move(tex);
		}
		else
		{
			
		}
	};
	checkForTexture(aiTextureType_DIFFUSE);
	checkForTexture(aiTextureType_SPECULAR);
	checkForTexture(aiTextureType_NORMALS);

}