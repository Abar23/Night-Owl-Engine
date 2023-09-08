#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Component/Component.h"
#include <memory>

namespace NightOwl
{
	class IMaterial;

	class Renderer : public Component
	{
	public:
		Renderer();

		void Draw();

		std::shared_ptr<Mesh> GetMesh();

		void SetMesh(std::shared_ptr<Mesh> mesh);

		void SetVisible(bool isVisible);

		bool IsVisible();

		const std::shared_ptr<IMaterial> GetMaterial();

		void SetMaterial(std::shared_ptr<IMaterial> material);

	protected:
		std::shared_ptr<Mesh> mesh;

		std::shared_ptr<IMaterial> material;

		bool isVisible;

		void CloneRenderer(const std::shared_ptr<Renderer>& clone);

		friend class AssimpModelLoader;
	};
}
