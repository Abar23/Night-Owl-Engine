#pragma once

#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include <string>

namespace NightOwl::Graphics
{
	class OpenGlTexture2D : public ITexture2D
	{
	public:
		OpenGlTexture2D(const std::string& texturePath);

		~OpenGlTexture2D() override;

		void Bind(unsigned int textureUnit) override;

		void UnBind() override;

		int GetWidth() override;

		int GetHeight() override;

		unsigned int GetTextureId() override;

	private:
		unsigned int textureId;

		int height;

		int width;

		void LoadTexture(const std::string& texturePath);
	};
}

