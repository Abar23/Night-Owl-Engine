#include "NightOwlPch.h"

#include "GraphicsFormat.h"
#include <glad/glad.h>

namespace NightOwl
{
	TextureFormat GraphicsFormatToTextureFormat(GraphicsFormat graphicsFormat)
	{
		switch (graphicsFormat)
		{
		case GraphicsFormat::R8:
		case GraphicsFormat::R16:
		case GraphicsFormat::R16F:
		case GraphicsFormat::R32F:
			return TextureFormat::Red;

		case GraphicsFormat::RG8:
		case GraphicsFormat::RG16:
		case GraphicsFormat::RG16F:
		case GraphicsFormat::RG32F:
			return TextureFormat::RG;

		case GraphicsFormat::RGB8:
		case GraphicsFormat::RGB16:
		case GraphicsFormat::RGB16F:
		case GraphicsFormat::RGB32F:
			return TextureFormat::RGB;

		case GraphicsFormat::RGBA8:
		case GraphicsFormat::RGBA16:
		case GraphicsFormat::RGBA16F:
		case GraphicsFormat::RGBA32F:
			return TextureFormat::RGBA;

		case GraphicsFormat::Depth16:
		case GraphicsFormat::Depth24:
		case GraphicsFormat::Depth32F:
			return TextureFormat::Depth;

		case GraphicsFormat::Depth24_Stencil8:
			return TextureFormat::DepthStencil;

		case GraphicsFormat::Stencil8:
			return TextureFormat::Stencil;
		}

		return TextureFormat::None;
	}

	int GraphicsFormatToOpenGlFormat(GraphicsFormat graphicsFormat)
	{
		switch (graphicsFormat)
		{
		case GraphicsFormat::R8:
			return GL_R8;

		case GraphicsFormat::R16:
			return GL_R16;

		case GraphicsFormat::R16F:
			return GL_R16F;

		case GraphicsFormat::R32F:
			return GL_R32F;

		case GraphicsFormat::RG8:
			return GL_RG8;

		case GraphicsFormat::RG16:
			return GL_RG16;

		case GraphicsFormat::RG16F:
			return GL_RG16F;

		case GraphicsFormat::RG32F:
			return GL_RG32F;

		case GraphicsFormat::RGB8:
			return GL_RGB8;

		case GraphicsFormat::RGB16:
			return GL_RGB16;

		case GraphicsFormat::RGB16F:
			return GL_RGB16F;

		case GraphicsFormat::RGB32F:
			return GL_RGB32F;

		case GraphicsFormat::RGBA8:
			return GL_RGBA8;

		case GraphicsFormat::RGBA16:
			return GL_RGBA16;

		case GraphicsFormat::RGBA16F:
			return GL_RGBA16F;

		case GraphicsFormat::RGBA32F:
			return GL_RGBA32F;

		case GraphicsFormat::Depth16:
			return GL_DEPTH_COMPONENT16;

		case GraphicsFormat::Depth24:
			return GL_DEPTH_COMPONENT24;

		case GraphicsFormat::Depth32F:
			return GL_DEPTH_COMPONENT32F;

		case GraphicsFormat::Depth24_Stencil8:
			return GL_DEPTH24_STENCIL8;

		case GraphicsFormat::Stencil8:
			return GL_STENCIL_INDEX8;
		}

		return GL_RGBA8;
	}

	int GraphicsFormatToOpenGlDataFormat(GraphicsFormat graphicsFormat)
	{
		switch (graphicsFormat)
		{
		case GraphicsFormat::R8:
		case GraphicsFormat::RG8:
		case GraphicsFormat::RGB8:
		case GraphicsFormat::RGBA8:
		case GraphicsFormat::Stencil8:
			return GL_UNSIGNED_BYTE;

		case GraphicsFormat::R16:
		case GraphicsFormat::RG16:
		case GraphicsFormat::RGB16:
		case GraphicsFormat::RGBA16:
		case GraphicsFormat::Depth16:
			return GL_UNSIGNED_SHORT;

		case GraphicsFormat::R16F:
		case GraphicsFormat::RG16F:
		case GraphicsFormat::RGB16F:
		case GraphicsFormat::RGBA16F:
			return GL_HALF_FLOAT;

		case GraphicsFormat::R32F:
		case GraphicsFormat::RG32F:
		case GraphicsFormat::RGB32F:
		case GraphicsFormat::RGBA32F:
		case GraphicsFormat::Depth32F:
			return GL_FLOAT;

		case GraphicsFormat::Depth24:
			return GL_UNSIGNED_INT;

		case GraphicsFormat::Depth24_Stencil8:
			return GL_UNSIGNED_INT_24_8;
		}

		return GL_UNSIGNED_BYTE;
	}

	int GraphicsFormatToOpenGlImageFormat(GraphicsFormat graphicsFormat)
	{
		switch (graphicsFormat)
		{
		case GraphicsFormat::R8:
			return GL_R8;

		case GraphicsFormat::R16:
			return GL_R16;

		case GraphicsFormat::R16F:
			return GL_R16F;

		case GraphicsFormat::R32F:
			return GL_R32F;

		case GraphicsFormat::RG8:
			return GL_RG8;

		case GraphicsFormat::RG16:
			return GL_RG16;

		case GraphicsFormat::RG16F:
			return GL_RG16F;

		case GraphicsFormat::RG32F:
			return GL_RG32F;

		case GraphicsFormat::RGB8:
			return GL_RGB8;

		case GraphicsFormat::RGB16:
			return GL_RGB16;

		case GraphicsFormat::RGB16F:
			return GL_RGB16F;

		case GraphicsFormat::RGB32F:
			return GL_RGB32F;

		case GraphicsFormat::RGBA8:
			return GL_RGBA8;

		case GraphicsFormat::RGBA16:
			return GL_RGBA16;

		case GraphicsFormat::RGBA16F:
			return GL_RGBA16F;

		case GraphicsFormat::RGBA32F:
			return GL_RGBA32F;

		case GraphicsFormat::Depth16:
			return GL_DEPTH_COMPONENT16;

		case GraphicsFormat::Depth24:
			return GL_DEPTH_COMPONENT24;

		case GraphicsFormat::Depth32F:
			return GL_DEPTH_COMPONENT32F;

		case GraphicsFormat::Depth24_Stencil8:
			return GL_DEPTH24_STENCIL8;

		case GraphicsFormat::Stencil8:
			return GL_STENCIL_INDEX8;
		}

		return GL_RGBA8;
	}
}
