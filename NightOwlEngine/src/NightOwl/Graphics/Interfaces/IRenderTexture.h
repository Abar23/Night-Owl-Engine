#pragma once

#include "NightOwl/Core/Utitlity/Assert.h"
#include <memory>
#include <vector>

namespace NightOwl
{
	enum class TextureFilterMode;

	enum class GraphicsFormat;

	class ITexture2D;

	class IRenderTexture
	{
	public:
		IRenderTexture(int height, int width, TextureFilterMode textureFilterMode)
			: frameBufferId(std::numeric_limits<unsigned int>::max()),
			  height(height),
			  width(width),
			  textureFilterMode(textureFilterMode)
		{ }

		virtual ~IRenderTexture() = default;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual void SetResolution(int width, int height) = 0;

		virtual void AddColorAttachment(GraphicsFormat format) = 0;

		virtual ITexture2D* GetColorAttachment(unsigned int attachmentIndex) = 0;

		virtual void RemoveColorAttachment(unsigned int attachmentIndex) = 0;

		virtual void AttachDepthBufferWithFormat(GraphicsFormat format) = 0;

		virtual void RemoveDepthAttachment() = 0;

		virtual void SetFilterMode(TextureFilterMode textureFilterMode) = 0;

		int GetFrameBufferHandle() const
		{
			return frameBufferId;
		}

		int GetHeight() const
		{
			return height;
		}

		int GetWidth() const
		{
			return width;
		}

		int GetNumberOfColorAttachment() const
		{
			return colorAttachments.size();
		}

		std::shared_ptr<ITexture2D> GetColorAttachment(unsigned int colorIndex) const
		{
			ENGINE_ASSERT(colorIndex > 0 && colorIndex < colorAttachments.size(), "Invalid index when retrieving color attachment from framebuffer");

			return colorAttachments[colorIndex];
		}

		std::shared_ptr<ITexture2D> GetDepthBuffer() const
		{
			return depthBuffer;
		}

	protected:
		unsigned int frameBufferId;

		int height;

		int width;

		TextureFilterMode textureFilterMode;

		std::vector<std::shared_ptr<ITexture2D>> colorAttachments;

		std::shared_ptr<ITexture2D> depthBuffer;

		virtual void ValidateFrameBuffer() = 0;
	};
}
