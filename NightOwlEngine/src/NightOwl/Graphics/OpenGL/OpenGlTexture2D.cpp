#include <NightOwlPch.h>

#include "OpenGlTexture2D.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Graphics/Types/AccessType.h"
#include "NightOwl/Graphics/Types/GraphicsFormat.h"
#include "NightOwl/Graphics/Types/TextureFormat.h"

namespace NightOwl
{
	OpenGlTexture2D::OpenGlTexture2D()
		: textureId(std::numeric_limits<unsigned int>::max()), 
		  height(0), 
		  width(0),
		  maxMipMapLevel(1),
		  graphicsFormat(GraphicsFormat::None),
		  textureFormat(TextureFormat::None),
		  wrapModeU(TextureWrapMode::ClampToEdge),
		  wrapModeV(TextureWrapMode::ClampToEdge),
		  textureFilterMode(TextureFilterMode::Point),
		  boundTextureUnit(std::numeric_limits<unsigned int>::max()),
		  boundImageUnit(std::numeric_limits<unsigned int>::max()),
		  accessForBoundImageUnit(AccessType::Read),
		  borderColor(0.0f, 0.0f, 0.0f, 1.0f)
	{ }

	OpenGlTexture2D::OpenGlTexture2D(const void* pixelData, int height, int width, GraphicsFormat format, TextureWrapMode wrapModeU /* = TextureWrapMode::ClampToEdge */, TextureWrapMode wrapModeV /* = TextureWrapMode::ClampToEdge  */, TextureFilterMode filterMode /* = TextureFilterMode::Nearest */)
		: textureId(std::numeric_limits<unsigned int>::max()),
		  height(height),
		  width(width),
		  graphicsFormat(format),
		  textureFormat(GraphicsFormatToTextureFormat(format)),
		  boundTextureUnit(std::numeric_limits<unsigned int>::max()),
		  borderColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
		CreateTexture();
		AllocateTexture();

		SetFilterMode(textureFilterMode);

		SetWrapModeU(wrapModeU);
		SetWrapModeV(wrapModeV);

		SetMaxMipMapLevel(1);

		if (pixelData != nullptr)
		{
			SetData(pixelData);
			GL_CALL(glGenerateTextureMipmap, textureId);
		}
	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		GL_CALL(glDeleteTextures, 1, &textureId);
	}

	void OpenGlTexture2D::Bind(unsigned int textureUnit)
	{
		boundTextureUnit = GL_TEXTURE0 + textureUnit;

		GL_CALL(glActiveTexture, boundTextureUnit);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureId);
	}

	void OpenGlTexture2D::BindAsImageUnit(unsigned imageUnit, AccessType access)
	{
		boundImageUnit = imageUnit;
		accessForBoundImageUnit = access;

		GL_CALL(glBindImageTexture, boundImageUnit, textureId, 0, GL_FALSE, 0, AccessTypeToOpenGlType(accessForBoundImageUnit), GraphicsFormatToOpenGlFormat(graphicsFormat));
	}

	void OpenGlTexture2D::Unbind()
	{
		ENGINE_ASSERT(boundTextureUnit != std::numeric_limits<unsigned int>::max(), "Unbinding texture from invalid texture unit.");

		GL_CALL(glActiveTexture, boundTextureUnit);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);

		boundTextureUnit = std::numeric_limits<unsigned int>::max();
	}

	void OpenGlTexture2D::UnbindImageUnit()
	{
		ENGINE_ASSERT(boundImageUnit != std::numeric_limits<unsigned int>::max(), "Unbinding texture from invalid texture unit.");

		GL_CALL(glBindImageTexture, boundImageUnit, 0, 0, GL_FALSE, 0, AccessTypeToOpenGlType(accessForBoundImageUnit), GraphicsFormatToOpenGlFormat(graphicsFormat));

		boundImageUnit = std::numeric_limits<unsigned int>::max();
	}

	void OpenGlTexture2D::SetData(const void* pixelData)
	{
		GL_CALL(glTextureSubImage2D, textureId, 0, 0, 0, width, height, TextureFormatToOpenGlFormat(textureFormat), GraphicsFormatToOpenGlDataFormat(graphicsFormat), pixelData);
	}

	void OpenGlTexture2D::Resize(int height, int width, GraphicsFormat format)
	{
		ReleaseTexture();
		CreateTexture();
		AllocateTexture();

		SetFilterMode(textureFilterMode);

		SetWrapModeU(wrapModeU);
		SetWrapModeV(wrapModeV);
	}

	void OpenGlTexture2D::SetWrapModeU(TextureWrapMode wrapMode)
	{
		wrapModeU = wrapMode;

		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenglWrapMode(wrapModeU));
	}

	TextureWrapMode OpenGlTexture2D::GetWrapModeU() const
	{
		return wrapModeU;
	}

	void OpenGlTexture2D::SetWrapModeV(TextureWrapMode wrapMode)
	{
		wrapModeV = wrapMode;

		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenglWrapMode(wrapModeV));
	}

	TextureWrapMode OpenGlTexture2D::GetWrapModeV() const
	{
		return wrapModeV;
	}

	void OpenGlTexture2D::SetFilterMode(TextureFilterMode filterMode)
	{
		textureFilterMode = filterMode;

		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_MIN_FILTER, TextureFilterModeToOpenGlMinFilter(textureFilterMode));
		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_MAG_FILTER, TextureFilterModeToOpenGlMagFilter(textureFilterMode));
	}

	TextureFilterMode OpenGlTexture2D::GetFilterMode() const
	{
		return textureFilterMode;
	}

	int OpenGlTexture2D::GetWidth() const
	{
		return width;
	}

	int OpenGlTexture2D::GetHeight() const
	{
		return height;
	}

	Vec4F OpenGlTexture2D::GetBorderColor() const
	{
		return borderColor;
	}

	void OpenGlTexture2D::SetBorderColor(const Vec4F& borderColor)
	{
		this->borderColor = borderColor;

		GL_CALL(glTextureParameterfv, textureId, GL_TEXTURE_BORDER_COLOR, borderColor.GetValuePointer());
	}

	GraphicsFormat OpenGlTexture2D::GetGraphicsFormat()
	{
		return graphicsFormat;
	}

	TextureFormat OpenGlTexture2D::GetTextureFormat()
	{
		return textureFormat;
	}

	unsigned int OpenGlTexture2D::GetTextureId() const
	{
		return textureId;
	}

	void OpenGlTexture2D::SetMaxMipMapLevel(int maxMipMapLevel)
	{
		ENGINE_ASSERT(maxMipMapLevel > 0, "Invalid mipmap level set on Texture2D!");

		this->maxMipMapLevel = maxMipMapLevel;

		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_MAX_LEVEL, maxMipMapLevel - 1);
	}

	int OpenGlTexture2D::GetMaxMipMapLevel() const
	{
		return maxMipMapLevel;
	}

	void OpenGlTexture2D::CreateTexture()
	{
		GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureId);
	}

	void OpenGlTexture2D::ReleaseTexture()
	{
		GL_CALL(glDeleteTextures, 1, &textureId);
	}

	void OpenGlTexture2D::AllocateTexture()
	{
		GL_CALL(glTextureStorage2D, textureId, 1 + std::floor(std::log2(std::max(width, height))), GraphicsFormatToOpenGlFormat(graphicsFormat), width, height);
	}
}
