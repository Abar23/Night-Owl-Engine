#pragma once

namespace NightOwl
{
	enum class TextureWrapMode
	{
		ClampToEdge,
		Repeat,
		ClampToBorder,
		MirroredRepeat,
		MirrorClampToEdge
	};

	int TextureWrapModeToOpenglWrapMode(TextureWrapMode textureWrapMode);
}
