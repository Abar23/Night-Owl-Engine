#include "NightOwlPch.h"

#include "TextureWrapMode.h"
#include <glad/glad.h>

namespace NightOwl
{
	int TextureWrapModeToOpenglWrapMode(TextureWrapMode textureWrapMode)
	{
		switch (textureWrapMode)
		{
		case TextureWrapMode::ClampToEdge:
			return GL_CLAMP_TO_EDGE;

		case TextureWrapMode::Repeat:
			return GL_REPEAT;

		case TextureWrapMode::ClampToBorder:
			return GL_CLAMP_TO_BORDER;

		case TextureWrapMode::MirroredRepeat:
			return GL_MIRRORED_REPEAT;

		case TextureWrapMode::MirrorClampToEdge:
			return GL_MIRRORED_REPEAT;
		}

		return GL_CLAMP_TO_EDGE;
	}
}