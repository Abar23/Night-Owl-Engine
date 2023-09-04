#pragma once

#include "IMaterial.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"

namespace NightOwl
{
	class StandardMaterial : public IMaterial
	{
	public:
		StandardMaterial();

		std::shared_ptr<IMaterial> Clone() override;

		void SetTexture(ITexture2D* texture2D) override;

		ITexture2D* GetTexture() override;

		void Draw(Renderer& renderer) override;

		void Draw(Collider2D& collider) override;

		void SetAlpha(float alpha) override;

	private:
		IShader* standardShader;

		ITexture2D* diffuseTexture;

		float alpha;
	};
}
