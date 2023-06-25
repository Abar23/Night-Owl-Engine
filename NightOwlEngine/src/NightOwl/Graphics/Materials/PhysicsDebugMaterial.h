#pragma once

#include "IMaterial.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"

namespace NightOwl
{
	class PhysicsDebugMaterial : public IMaterial
	{
	public:
		PhysicsDebugMaterial();

		std::shared_ptr<IMaterial> Clone() override;

		void SetTexture(ITexture2D* texture2D) override;

		ITexture2D* GetTexture() override;

		void Draw(MeshRenderer& meshRenderer) override;

		void Draw(Collider2D& collider) override;

		void SetAlpha(float alpha) override;

	private:
		IShader* physicsDebugShader;
	};
}


