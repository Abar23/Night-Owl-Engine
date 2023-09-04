#pragma once

#include "Transform.h"
#include "NightOwl/Component/Abstrtact/Renderer.h"

namespace NightOwl
{
	class SkinnedMeshRenderer : public Renderer
	{
	public:
		SkinnedMeshRenderer();

		std::shared_ptr<Component> Clone() override;

	private:
		std::vector<Transform> bones;

		void Remove() override;
	};
}
