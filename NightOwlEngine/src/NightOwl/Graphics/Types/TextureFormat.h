#pragma once

#include "glad/glad.h"

namespace NightOwl
{
	// OpenGL texture format to graphics format: https://registry.khronos.org/OpenGL/specs/gl/glspec45.compatibility.pdf
	enum class TextureFormat
	{
		None,
		Red,
		RG,
		RGB,
		RGBA,
		Depth,
		DepthStencil,
		Stencil
	};

	int TextureFormatToOpenGlFormat(TextureFormat textureFormat);
}
