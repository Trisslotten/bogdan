#pragma once

#include <GL/glew.h>
#include <string>
 
//GL_TEXTURE_2D

class Texture
{
	friend class Framebuffer;


	GLuint id;
	GLuint width, height;

	enum class Type : GLenum
	{
		_2D = GL_TEXTURE_2D,
		
	} type = Type::_2D;

public:

	~Texture();
	bool load(const std::string& file);
	void bind(size_t slot);
};