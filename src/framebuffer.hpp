#pragma once

#include <GL/glew.h>
#include <vector>

#include "texture.hpp"

class Framebuffer
{
	GLuint fbo;
	GLuint width, height;

	GLint max_attachments;
	GLuint attachment_count = 0;
	GLenum* attachments;
	std::vector<Texture> textures;

	Framebuffer(GLuint width_, GLuint  height_);
	~Framebuffer();

public:
	Texture* attach(GLuint num_values);

};