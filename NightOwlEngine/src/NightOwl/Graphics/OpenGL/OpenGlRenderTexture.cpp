#include "NightOwlPch.h"

#include "OpenGlRenderTexture.h"
#include "OpenGlTexture2D.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Graphics/Graphics.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include "NightOwl/Graphics/Types/GraphicsFormat.h"

namespace NightOwl
{
	OpenGlRenderTexture::OpenGlRenderTexture(int height, int width, TextureFilterMode textureFilterMode, GraphicsFormat firstColorAttachmentFormat, GraphicsFormat depthBufferFormat)
		: IRenderTexture(height, width, textureFilterMode)
	{
		GL_CALL(glCreateFramebuffers, 1, &frameBufferId);

		AddColorAttachment(firstColorAttachmentFormat);

		AttachDepthBufferWithFormat(depthBufferFormat);

		ValidateFrameBuffer();
	}

	OpenGlRenderTexture::~OpenGlRenderTexture()
	{
		GL_CALL(glDeleteFramebuffers, 1, &frameBufferId);
	}

	void OpenGlRenderTexture::Bind()
	{
		GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, frameBufferId);
	}

	void OpenGlRenderTexture::Unbind()
	{
		GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
	}

	void OpenGlRenderTexture::SetResolution(int width, int height)
	{
		this->width = width;
		this->height = height;

		for (const auto& colorAttachment : colorAttachments)
		{
			colorAttachment->Resize(height, width, colorAttachment->GetGraphicsFormat());
		}

		depthBuffer->Resize(height, width, depthBuffer->GetGraphicsFormat());

		ValidateFrameBuffer();
	}

	void OpenGlRenderTexture::AddColorAttachment(GraphicsFormat format)
	{
		const std::shared_ptr<ITexture2D> colorAttachment = std::make_shared<OpenGlTexture2D>(nullptr, height, width, format);
		colorAttachment->SetFilterMode(textureFilterMode);
		colorAttachments.push_back(colorAttachment);

		const unsigned int colorAttachmentValue = GL_COLOR_ATTACHMENT0 + colorAttachments.size();
		glColorAttachmentValues.push_back(colorAttachmentValue);

		GL_CALL(glNamedFramebufferTexture, frameBufferId, colorAttachmentValue, colorAttachment->GetTextureId(), 0);
		GL_CALL(glNamedFramebufferDrawBuffers, frameBufferId, glColorAttachmentValues.size(), glColorAttachmentValues.data());

		ValidateFrameBuffer();
	}

	void OpenGlRenderTexture::RemoveColorAttachment(unsigned attachmentIndex)
	{
		ENGINE_ASSERT(attachmentIndex >= 0 && attachmentIndex < colorAttachments.size(), "Trying to remove color attachment from invalid index in frame buffer");

		GL_CALL(glNamedFramebufferTexture, frameBufferId, TextureFormatToOpenGlFormat(colorAttachments[attachmentIndex]->GetTextureFormat()), 0, 0);

		std::iter_swap(colorAttachments.begin() + attachmentIndex, colorAttachments.end() - 1);
		colorAttachments.erase(colorAttachments.begin() + attachmentIndex);

		GL_CALL(glNamedFramebufferDrawBuffers, frameBufferId, glColorAttachmentValues.size(), glColorAttachmentValues.data());

		ValidateFrameBuffer();
	}

	void OpenGlRenderTexture::AttachDepthBufferWithFormat(GraphicsFormat graphicsFormat)
	{
		ENGINE_ASSERT(graphicsFormat != GraphicsFormat::None, "Tried setting frame buffer's depth attachment graphicsFormat to None.");
		ENGINE_ASSERT(GraphicsFormatToTextureFormat(graphicsFormat) != TextureFormat::Depth || GraphicsFormatToTextureFormat(graphicsFormat) != TextureFormat::DepthStencil, "Tried setting frame buffer's depth attachment to non-depth texture graphicsFormat.");

		if (depthBuffer == nullptr)
		{
			depthBuffer = std::make_shared<OpenGlTexture2D>(nullptr, height, width, graphicsFormat);
		}

		depthBuffer->Resize(height, width, graphicsFormat);

		const unsigned int depthAttachmentType = depthBuffer->GetTextureFormat() == TextureFormat::Depth ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL_ATTACHMENT;
		GL_CALL(glNamedFramebufferTexture, frameBufferId, depthAttachmentType, depthBuffer->GetTextureId(), 0);

		ValidateFrameBuffer();
	}

	ITexture2D* OpenGlRenderTexture::GetColorAttachment(unsigned attachmentIndex)
	{
		ENGINE_ASSERT(attachmentIndex >= 0 && attachmentIndex < colorAttachments.size(), "Trying to get color attachment from invalid index in frame buffer");

		return colorAttachments[attachmentIndex].get();
	}

	void OpenGlRenderTexture::RemoveDepthAttachment()
	{
		const unsigned int depthAttachmentType = depthBuffer->GetTextureFormat() == TextureFormat::Depth ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL_ATTACHMENT;
		GL_CALL(glNamedFramebufferTexture, frameBufferId, depthAttachmentType, depthBuffer->GetTextureId(), 0);

		GL_CALL(glNamedFramebufferTexture, frameBufferId, GL_DEPTH_ATTACHMENT, 0, 0);

		ValidateFrameBuffer();
	}

	void OpenGlRenderTexture::SetFilterMode(TextureFilterMode textureFilterMode)
	{
		for (const auto& colorAttachment : colorAttachments)
		{
			colorAttachment->SetFilterMode(textureFilterMode);
		}
	}

	void OpenGlRenderTexture::ValidateFrameBuffer()
	{
		CHECK_FRAME_BUFFER_COMPLETENESS(frameBufferId);
	}
}
