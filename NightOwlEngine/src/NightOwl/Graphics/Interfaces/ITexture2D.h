#pragma once

namespace NightOwl
{
	class ITexture2D
	{
	public:
		virtual ~ITexture2D() = default;

		virtual void Bind(unsigned int textureUnit) const = 0;

		virtual void Unbind() const = 0;

		virtual int GetWidth() = 0;

		virtual int GetHeight() = 0;

		virtual unsigned int GetTextureId() = 0;
	};
}
