#pragma once

#include "TextureFormat.h"
#include <glad/glad.h>

namespace NightOwl
{
	// OpenGL texture format to graphics format: https://registry.khronos.org/OpenGL/specs/gl/glspec45.compatibility.pdf
	enum class GraphicsFormat
	{
		None,
		R8,
		R16,
		R16F,
		R32F,
		RG8,
		RG16,
		RG16F,
		RG32F,
		RGB8,
		RGB16,
		RGB16F,
		RGB32F,
		RGBA8,
		RGBA16,
		RGBA16F,
		RGBA32F,
		Depth16,
		Depth24,
		Depth32F,
		Depth24_Stencil8,
		Stencil8
	};

	TextureFormat GraphicsFormatToTextureFormat(GraphicsFormat graphicsFormat);

	int GraphicsFormatToOpenGlFormat(GraphicsFormat graphicsFormat);

	int GraphicsFormatToOpenGlDataFormat(GraphicsFormat graphicsFormat);
}