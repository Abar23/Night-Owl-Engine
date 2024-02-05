#pragma once

#include "NightOwl/Graphics/Interfaces/IRenderTexture.h"

namespace NightOwl
{
	class OpenGlRenderTexture : IRenderTexture
	{
	public:
		OpenGlRenderTexture(int height, int width, TextureFilterMode textureFilterMode, GraphicsFormat firstColorAttachmentFormat, GraphicsFormat depthBufferFormat);

		~OpenGlRenderTexture() override;

		void Bind() override;

		void Unbind() override;

		void SetResolution(int width, int height) override;

		void AddColorAttachment(GraphicsFormat format) override;

		void RemoveColorAttachment(unsigned attachmentIndex) override;

		void AttachDepthBufferWithFormat(GraphicsFormat graphicsFormat) override;

		ITexture2D* GetColorAttachment(unsigned int attachmentIndex) override;

		void RemoveDepthAttachment() override;

		void SetFilterMode(TextureFilterMode textureFilterMode) override;

	protected:
		void ValidateFrameBuffer() override;

	private:
		std::vector<unsigned int> glColorAttachmentValues;
	};
}
