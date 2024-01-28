#include <NightOwlPch.h>

#include "OpenGlTexture2D.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Graphics/Types/GraphicsFormat.h"
#include "NightOwl/Graphics/Types/TextureFormat.h"

namespace NightOwl
{
	OpenGlTexture2D::OpenGlTexture2D()
		: textureId(-1), 
		  height(0), 
		  width(0),
		  graphicsFormat(GraphicsFormat::None),
		  textureFormat(TextureFormat::None),
		  wrapModeU(TextureWrapMode::ClampToEdge),
		  wrapModeV(TextureWrapMode::MirrorClampToEdge),
		  textureFilterMode(TextureFilterMode::Point)
	{
	}

	OpenGlTexture2D::OpenGlTexture2D(const void* pixelData, int height, int width, GraphicsFormat format, TextureWrapMode wrapModeU /* = TextureWrapMode::ClampToEdge */, TextureWrapMode wrapModeV /* = TextureWrapMode::ClampToEdge  */, TextureFilterMode filterMode /* = TextureFilterMode::Nearest */)
		: height(height),
		  width(width),
		  graphicsFormat(format),
		  textureFormat(GraphicsFormatToTextureFormat(format)),
		  wrapModeU(wrapModeU),
		  wrapModeV(wrapModeV),
		  textureFilterMode(filterMode)
	{
		GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &textureId);
		GL_CALL(glTextureStorage2D, textureId, 1, GraphicsFormatToOpenGlFormat(graphicsFormat), width, height);

		SetFilterMode(textureFilterMode);

		SetWrapModeU(wrapModeU);
		SetWrapModeV(wrapModeV);

		SetData(pixelData, height, width, format);
	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		GL_CALL(glDeleteTextures, 1, &textureId);
	}

	void OpenGlTexture2D::Bind(unsigned int textureUnit) const
	{
		GL_CALL(glActiveTexture, GL_TEXTURE0 + textureUnit);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, textureId);
	}

	void OpenGlTexture2D::Unbind() const
	{
		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
	}

	void OpenGlTexture2D::SetData(const void* pixelData, int height, int width, GraphicsFormat format)
	{
		GL_CALL(glTextureSubImage2D, textureId, 0, 0, 0, width, height, TextureFormatToOpenGlFormat(textureFormat), GraphicsFormatToOpenGlDataFormat(format), pixelData);
	}

	void OpenGlTexture2D::SetWrapModeU(TextureWrapMode wrapMode)
	{
		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_WRAP_S, TextureWrapModeToOpenglWrapMode(wrapModeU));
	}

	TextureWrapMode OpenGlTexture2D::GetWrapModeU() const
	{
		return wrapModeU;
	}

	void OpenGlTexture2D::SetWrapModeV(TextureWrapMode wrapMode)
	{
		GL_CALL(glTextureParameteri, textureId, GL_TEXTURE_WRAP_T, TextureWrapModeToOpenglWrapMode(wrapModeU));
	}

	TextureWrapMode OpenGlTexture2D::GetWrapModeV() const
	{
		return wrapModeV;
	}

	void OpenGlTexture2D::SetFilterMode(TextureFilterMode filterMode)
	{
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

	unsigned int OpenGlTexture2D::GetTextureId() const
	{
		return textureId;
	}
}
