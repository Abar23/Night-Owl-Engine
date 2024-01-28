#include "NightOwlPch.h"

#include "TextureFormat.h"
#include <glad/glad.h>

namespace NightOwl
{
	int TextureFormatToOpenGlFormat(TextureFormat textureFormat)
	{
		switch (textureFormat)
		{
		case TextureFormat::Red:
			return GL_RED;

		case TextureFormat::RG:
			return GL_RG;

		case TextureFormat::RGB:
			return GL_RGB;

		case TextureFormat::RGBA:
			return GL_RGBA;

		case TextureFormat::Depth:
			return GL_DEPTH;

		case TextureFormat::DepthStencil:
			return GL_DEPTH_STENCIL;

		case TextureFormat::Stencil:
			return GL_STENCIL;
		}

		return GL_RGBA;
	}
}