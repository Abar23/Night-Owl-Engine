#pragma once

namespace NightOwl
{
	enum class AccessType;

	enum class GraphicsFormat;

	enum class TextureFilterMode;

	enum class TextureFormat;

	enum class TextureWrapMode;

	class ITexture2D
	{
	public:
		virtual ~ITexture2D() = default;

		virtual void Bind(unsigned int textureUnit) = 0;

		virtual void BindAsImageUnit(unsigned int imageUnit, AccessType access) = 0;

		virtual void Unbind() = 0;

		virtual void UnbindImageUnit() = 0;

		virtual void SetData(const void* pixelData) = 0;

		virtual void Resize(int height, int width, GraphicsFormat format) = 0;
		
		virtual void SetWrapModeU(TextureWrapMode wrapMode) = 0;
		
		virtual TextureWrapMode GetWrapModeU() const = 0;
		
		virtual void SetWrapModeV(TextureWrapMode wrapMode) = 0;
		
		virtual TextureWrapMode GetWrapModeV() const = 0;
		
		virtual void SetFilterMode(TextureFilterMode filterMode) = 0;
		
		virtual TextureFilterMode GetFilterMode() const = 0;

		virtual int GetWidth() const = 0;

		virtual int GetHeight() const = 0;

		virtual GraphicsFormat GetGraphicsFormat() = 0;

		virtual TextureFormat GetTextureFormat() = 0;

		virtual unsigned int GetTextureId() const = 0;

	protected:
		virtual void CreateTexture() = 0;

		virtual void ReleaseTexture() = 0;

		virtual void AllocateTexture() = 0;
	};
}
