#pragma once

#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include "NightOwl/Graphics/Interfaces/IContext.h"
#include "NightOwl/Graphics/OpenGL/OpenGlShader.h"
#include "NightOwl/Graphics/OpenGL/OpenGlTexture2D.h"
#include "NightOwl/Graphics/OpenGL/OpenGlContext.h"
#include <memory>

#include "NightOwl/Window/Interfaces/IWindow.h"

namespace NightOwl::Graphics
{
	class RenderApi
	{
	public:
		RenderApi() = default;

		static RenderApi* GetInstance();

		RenderApi(RenderApi& other) = delete;

		void operator=(const RenderApi&) = delete;

		template<typename... Args>
		std::shared_ptr<IShader> CreateShader(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlShader>(std::forward<Args>(args)...);
			#endif
		}

		template<typename... Args>
		std::shared_ptr<ITexture2D> CreateTexture2D(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlTexture2D>(std::forward<Args>(args)...);
			#endif
		}

		std::unique_ptr<IContext> CreateContext(Window::IWindow* window) const
		{
			#ifdef OPEN_GL
			return std::make_unique<OpenGlContext>(static_cast<GLFWwindow*>(window->GetWindowHandle()));
			#endif
		}

	private:
		static RenderApi* renderApi;

	};
}
