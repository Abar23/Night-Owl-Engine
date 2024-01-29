#include "NightOwlPch.h"

#include "TextureFilterMode.h"
#include <glad/glad.h>

namespace NightOwl
{
	int TextureFilterModeToOpenGlMinFilter(TextureFilterMode textureFilterMode)
	{
		switch (textureFilterMode)
		{
		case TextureFilterMode::Point:
			return GL_NEAREST;

		case TextureFilterMode::Bilinear:
		case TextureFilterMode::Trilinear:
			return GL_LINEAR;

		default:
			return GL_NEAREST;
		}
	}

	int TextureFilterModeToOpenGlMagFilter(TextureFilterMode textureFilterMode)
	{
		switch (textureFilterMode)
		{
		case TextureFilterMode::Point:
			return GL_NEAREST;

		case TextureFilterMode::Bilinear:
			return GL_LINEAR;

		case TextureFilterMode::Trilinear:
			return GL_LINEAR_MIPMAP_LINEAR;

		default:
			return GL_NEAREST;
		}
	}
}