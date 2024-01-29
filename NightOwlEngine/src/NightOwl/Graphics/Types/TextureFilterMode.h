#pragma once

namespace NightOwl
{
	enum class TextureFilterMode
	{
		Point,
		Bilinear,
		Trilinear
	};

	int TextureFilterModeToOpenGlMinFilter(TextureFilterMode textureFilterMode);

	int TextureFilterModeToOpenGlMagFilter(TextureFilterMode textureFilterMode);
}
