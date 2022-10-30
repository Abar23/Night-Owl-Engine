#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Component/Component.h"

namespace NightOwl::Component
{
	class IMaterial;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();

		void Draw();

		Mesh* GetMesh();

		void SetVisible(bool isVisible);

		bool IsVisible();

		const std::shared_ptr<IMaterial> GetMaterial();

		void SetMaterial(std::shared_ptr<IMaterial> material);

	private:
		Mesh mesh;

		std::shared_ptr<IMaterial> material;

		bool isVisible;

	};
}
