#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Graphics/Interfaces/IComputeShader.h"
#include "NightOwl/Graphics/Interfaces/IContext.h"
#include "NightOwl/Graphics/Interfaces/IRenderTexture.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Types/BufferType.h"
#include "NightOwl/Window/Interfaces/IWindow.h"
#include <memory>

#ifdef OPEN_GL
#include "NightOwl/Graphics/OpenGL/OpenGlComputeShader.h"
#include "NightOwl/Graphics/OpenGL/OpenGlContext.h"
#include "NightOwl/Graphics/OpenGL/OpenGlGraphicsBuffer.h"
#include "NightOwl/Graphics/OpenGL/OpenGlRenderTexture.h"
#include "NightOwl/Graphics/OpenGL/OpenGlShader.h"
#include "NightOwl/Graphics/OpenGL/OpenGlShaderStage.h"
#include "NightOwl/Graphics/OpenGL/OpenGlTexture2D.h"
#include "NightOwl/Graphics/OpenGL/OpenGlVertexArrayObject.h"
#endif

namespace NightOwl
{
	class Graphics
	{
	public:
		static void Initialize();

		static void Render();

		static void Shutdown();

		template<typename... Args>
		static std::shared_ptr<IShader> CreateShader(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlShader>(std::forward<Args>(args)...);
			#endif
		}

		static std::shared_ptr<IComputeShader> CreateComputeShader(const std::string& computeShaderName)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlComputeShader>(computeShaderName);
			#endif
		}


		template<typename... Args>
		static std::shared_ptr<IShaderStage> CreateShaderStage(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlShaderStage>(std::forward<Args>(args)...);
			#endif
		}

		template<typename... Args>
		static std::shared_ptr<ITexture2D> CreateTexture2D(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlTexture2D>(std::forward<Args>(args)...);
			#endif
		}

		template<typename... Args>
		static std::shared_ptr<IRenderTexture> CreateRenderTexture(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlRenderTexture>(std::forward<Args>(args)...);
			#endif
		}

		static void CreateContext(IWindow* window)
		{
			if (graphicsContext) return;

			#ifdef OPEN_GL
			graphicsContext = std::make_unique<OpenGlContext>(static_cast<GLFWwindow*>(window->GetWindowHandle()));
			#endif
		}

		static const std::unique_ptr<IContext>& GetContext()
		{
			ENGINE_ASSERT(graphicsContext != nullptr, "Tried to fetch graphics context before window was created!");

			return graphicsContext;
		}
		
		static std::shared_ptr<IGraphicsBuffer> CreateGraphicsBuffer(BufferType bufferType)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlGraphicsBuffer>(bufferType);
			#endif
		}

		template<typename... Args>
		static std::shared_ptr<IVertexArrayObject> CreateVertexArrayObject(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlVertexArrayObject>(std::forward<Args>(args)...);
			#endif
		}

	private:
		inline static std::unique_ptr<IContext> graphicsContext{ nullptr };

		inline static std::unique_ptr<IRenderTexture> deferredGBuffer{ nullptr };

		inline static std::unique_ptr<Mesh> quadMesh;
	};
}
