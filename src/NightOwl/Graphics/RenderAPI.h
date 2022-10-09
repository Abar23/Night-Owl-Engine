#pragma once

#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/OpenGL/OpenGlShader.h"
#include <memory>

namespace NightOwl::Graphics
{
	class IRenderApi
	{
	public:
		template<typename... Args>
		std::shared_ptr<IShader> CreateShader(Args&&... args)
		{
			#ifdef OPEN_GL
			return std::make_shared<OpenGlShader>(std::forward<Args>(args)...);
			#endif
		}
	};

}
