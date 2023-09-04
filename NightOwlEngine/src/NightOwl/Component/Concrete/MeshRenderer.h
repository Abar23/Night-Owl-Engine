#pragma once

#include "NightOwl/Component/Abstrtact/Renderer.h"
#include "NightOwl/Core/Reflection/Reflection.h"

namespace NightOwl
{
	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer();

		std::shared_ptr<Component> Clone() override;
		
	private:
		void Remove() override;
	};
}
