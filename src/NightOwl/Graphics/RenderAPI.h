#pragma once

#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include "NightOwl/Graphics/Interfaces/IContext.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/OpenGL/OpenGlShader.h"
#include "NightOwl/Graphics/OpenGL/OpenGlTexture2D.h"
#include "NightOwl/Graphics/OpenGL/OpenGlContext.h"
#include "NightOwl/Graphics/OpenGL/OpenGlVertexBuffer.h"
#include "NightOwl/Graphics/OpenGL/OpenGlIndexBuffer.h"
#include "NightOwl/Graphics/OpenGL/OpenGlVertexArrayObject.h"
#include "NightOwl/Window/Interfaces/IWindow.h"
#include <memory>

namespace NightOwl::Graphics
{
	class RenderApi
	{
	public:
		template<typename... Args>
		static std::shared_ptr<IShader> CreateShader(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlShader>(std::forward<Args>(args)...);
			#endif
		}

		template<typename... Args>
		static std::shared_ptr<ITexture2D> CreateTexture2D(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlTexture2D>(std::forward<Args>(args)...);
			#endif
		}

		static std::unique_ptr<IContext> CreateContext(Window::IWindow* window)
		{
			#ifdef OPEN_GL
			return std::make_unique<OpenGlContext>(static_cast<GLFWwindow*>(window->GetWindowHandle()));
			#endif
		}

		template<typename... Args>
		static std::shared_ptr<IVertexBuffer> CreateVertexBuffer(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlVertexBuffer>(std::forward<Args>(args)...);
			#endif
		}

		template<typename... Args>
		static std::shared_ptr<IIndexBuffer> CreateIndexBuffer(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlIndexBuffer>(std::forward<Args>(args)...);
			#endif
		}

		template<typename... Args>
		static std::shared_ptr<IVertexArrayObject> CreateVertexArrayObject(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlVertexArrayObject>(std::forward<Args>(args)...);
			#endif
		}
	};
}
