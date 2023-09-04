#pragma once

#include "NightOwl/Component/Abstrtact/Renderer.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"	
#include "NightOwl/Physics/2D/Collider/Collider2D.h"

namespace NightOwl
{
	class IMaterial
	{
	public:
		virtual ~IMaterial() = default;

		virtual std::shared_ptr<IMaterial> Clone() = 0;

		virtual void SetTexture(ITexture2D* texture2D) = 0;

		virtual ITexture2D* GetTexture() = 0;

		virtual void SetAlpha(float alpha) = 0;

		virtual void Draw(Renderer& renderer) = 0;

		virtual void Draw(Collider2D& collider) = 0;
	};
}
