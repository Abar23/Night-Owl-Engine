#pragma once

#include "IMaterial.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include "NightOwl/Core/Memory/WeakPointer.h"


namespace NightOwl::Graphics
{
	class StandardMaterial : public IMaterial
	{
	public:
		StandardMaterial();

		std::shared_ptr<IMaterial> Clone() override;

		void SetTexture(Core::WeakPointer<ITexture2D> texture2D) override;

		Core::WeakPointer<ITexture2D> GetTexture() override;

		void Draw(Component::MeshRenderer& meshRenderer) override;

		void Draw(Physics::Collider2D& collider) override;

		void SetAlpha(float alpha) override;

	private:
		Core::WeakPointer<IShader> standardShader;

		Core::WeakPointer<ITexture2D> diffuseTexture;

		float alpha;
	};
}
