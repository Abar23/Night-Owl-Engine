#pragma once

namespace NightOwl::Graphics
{
	class ITexture2D
	{
	public:
		virtual ~ITexture2D() = default;

		virtual void Bind(unsigned int textureUnit) = 0;

		virtual void Unbind() = 0;

		virtual int GetWidth() = 0;

		virtual int GetHeight() = 0;

		virtual unsigned int GetTextureId() = 0;
	};
}
