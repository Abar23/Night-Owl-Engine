#include "OpenGlTexture2D.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "stb/stb_image.h"

namespace NightOwl::Graphics
{
	OpenGlTexture2D::OpenGlTexture2D(const std::string& texturePath)
	{
		LoadTexture(texturePath);
	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		GL_CALL(glDeleteTextures, 1, &textureId);
	}

	void OpenGlTexture2D::Bind(unsigned int textureUnit)
	{
		GL_CALL(glActiveTexture, GL_TEXTURE0 + textureUnit);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureId);
	}

	void OpenGlTexture2D::UnBind()
	{
		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
	}

	int OpenGlTexture2D::GetWidth()
	{
		return width;
	}

	int OpenGlTexture2D::GetHeight()
	{
		return height;
	}

	unsigned int OpenGlTexture2D::GetTextureId()
	{
		return textureId;
	}

	void OpenGlTexture2D::LoadTexture(const std::string& texturePath)
	{
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(texturePath.c_str(), &width, &height, nullptr, 4);

		ENGINE_ASSERT(data == nullptr, std::format("Failed to load texture from file path: {0}", texturePath));

		GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureId);
		GL_CALL(glTextureStorage2D, textureId, 1, GL_RGBA, width, height);

		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GL_CALL(glTextureSubImage2D, textureId, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
}
