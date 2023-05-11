#pragma once

#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"	
#include "NightOwl/Physics/2D/Collider/Collider2D.h"

namespace NightOwl::Graphics
{
	class IMaterial
	{
	public:
		virtual ~IMaterial() = default;

		virtual std::shared_ptr<IMaterial> Clone() = 0;

		virtual void SetTexture(Core::WeakPointer<ITexture2D> texture2D) = 0;

		virtual Core::WeakPointer<ITexture2D> GetTexture() = 0;

		virtual void SetAlpha(float alpha) = 0;

		virtual void Draw(Component::MeshRenderer& meshRenderer) = 0;

		virtual void Draw(Physics::Collider2D& collider) = 0;
	};
}
