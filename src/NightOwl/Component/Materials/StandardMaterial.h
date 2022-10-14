#pragma once

#include "IMaterial.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"

namespace NightOwl::Component
{
	class StandardMaterial : public IMaterial
	{
	public:
		StandardMaterial();

		void Draw(MeshRenderer& meshRenderer) override;

		void SetTexture(std::shared_ptr<Graphics::ITexture2D> texture2D) override;

	private:
		std::shared_ptr<Graphics::IShader> standardShader;

		std::shared_ptr<Graphics::ITexture2D> diffuseTexture;
	};
}
