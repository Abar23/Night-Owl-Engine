#pragma once

#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"

namespace NightOwl::Component
{
	class IMaterial
	{
	public:
		virtual ~IMaterial() = default;

		virtual void SetTexture(std::shared_ptr<Graphics::ITexture2D> texture2D) = 0;

		virtual void Draw(MeshRenderer& meshRenderer) = 0;
	};
}

