#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Component/Component.h"
#include "NightOwl/Core/Reflection/Reflection.h"

namespace NightOwl::Graphics
{
	class IMaterial;
}

namespace NightOwl::Component
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();

		std::shared_ptr<Component> Clone() override;

		void Draw();

		Core::WeakPointer <Mesh> GetMesh();

		void SetVisible(bool isVisible);

		bool IsVisible();

		const std::shared_ptr<Graphics::IMaterial> GetMaterial();

		void SetMaterial(std::shared_ptr<Graphics::IMaterial> material);

		REFLECT()
		
	private:
		Mesh mesh;

		std::shared_ptr<Graphics::IMaterial> material;

		bool isVisible;

		void Remove() override;
	};
}
