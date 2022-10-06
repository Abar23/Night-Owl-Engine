#pragma once

namespace NightOwl::Graphics
{
	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void Bind(unsigned int textureUnit) = 0;
		virtual void UnBind() = 0;

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
		virtual int GetTextureId() = 0;
	};
}
