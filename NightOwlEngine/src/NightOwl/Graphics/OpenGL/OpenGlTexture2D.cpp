#include <NightOwlPch.h>

#include "OpenGlTexture2D.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"

namespace NightOwl
{
	OpenGlTexture2D::OpenGlTexture2D()
		: textureHandle(0),
		  textureId(0), 
		  height(0), 
		  width(0), 
		  numberOfChannels(0)
	{
	}

	OpenGlTexture2D::OpenGlTexture2D(const void* pixelData, int height, int width, int numberOfChannels)
		:	height(height),
			width(width),
			numberOfChannels(numberOfChannels)
	{
		GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureId);
		GL_CALL(glTextureStorage2D, textureId, 1, GL_RGBA8, width, height);

		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		GL_CALL(glTextureSubImage2D, textureId, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		GL_CALL(glDeleteTextures, 1, &textureId);
	}

	void OpenGlTexture2D::Bind(unsigned int textureUnit) const
	{
		GL_CALL(glActiveTexture, GL_TEXTURE0 + textureUnit);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureId);
	}

	void OpenGlTexture2D::Unbind() const
	{
		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
	}

	int OpenGlTexture2D::GetWidth()
	{
		return width;
	}

	int OpenGlTexture2D::GetHeight()
	{
		return height;
	}

	unsigned int OpenGlTexture2D::GetTextureId()
	{
		return textureId;
	}
}
