#pragma once

#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include "NightOwl/Graphics/Types/TextureFilterMode.h"
#include "NightOwl/Graphics/Types/TextureWrapMode.h"

namespace NightOwl
{
	enum class GraphicsFormat;

	class OpenGlTexture2D : public ITexture2D
	{
	public:
		OpenGlTexture2D();

		OpenGlTexture2D(const void* pixelData, int height, int width, GraphicsFormat format, TextureWrapMode wrapModeU = TextureWrapMode::ClampToEdge, TextureWrapMode wrapModeV = TextureWrapMode::ClampToEdge, TextureFilterMode filterMode = TextureFilterMode::Point);

		~OpenGlTexture2D() override;

		void Bind(unsigned int textureUnit) override;

		void BindAsImageUnit(unsigned imageUnit, AccessType access) override;

		void Unbind() override;

		void UnbindImageUnit() override;

		void SetData(const void* pixelData) override;

		void Resize(int height, int width, GraphicsFormat format) override;

		void SetWrapModeU(TextureWrapMode wrapMode) override;

		TextureWrapMode GetWrapModeU() const override;

		void SetWrapModeV(TextureWrapMode wrapMode) override;

		TextureWrapMode GetWrapModeV() const override;

		void SetFilterMode(TextureFilterMode filterMode) override;

		TextureFilterMode GetFilterMode() const override;

		int GetWidth() const override;

		int GetHeight() const override;

		GraphicsFormat GetGraphicsFormat() override;

		TextureFormat GetTextureFormat() override;

		unsigned int GetTextureId() const override;

	protected:
		void CreateTexture() override;

		void ReleaseTexture() override;

		void AllocateTexture() override;

	private:
		unsigned int textureId;

		int height;

		int width;

		GraphicsFormat graphicsFormat;

		TextureFormat textureFormat;

		TextureWrapMode wrapModeU;

		TextureWrapMode wrapModeV;

		TextureFilterMode textureFilterMode;

		unsigned int boundTextureUnit;

		unsigned int boundImageUnit;

		AccessType accessForBoundImageUnit;
	};
}

