#include "framebuffer.hpp"
#include "texture.hpp"
#include <iostream>
#include <cassert>

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//                                                               //
// TODO: Create better API when actually using a framebuffer !!! //
//                                                               //
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


Framebuffer::Framebuffer(GLuint width_, GLuint height_)
{
	glGenFramebuffers(1, &fbo);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_attachments);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &fbo);
}

Texture* Framebuffer::attach(GLuint num_values)
{
	if (attachment_count >= max_attachments)
	{
		std::cout << "ERROR: too many color attachments on framebuffer\n";
		return nullptr;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	textures.emplace_back();
	auto t = textures.back();
	t.width = width;
	t.height = height;
	t.type = Texture::Type::_2D;
	glGenTextures(1, &t.id);
	glBindTexture(GL_TEXTURE_2D, t.id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment_count, t.id, 0);

	return &t;
}
