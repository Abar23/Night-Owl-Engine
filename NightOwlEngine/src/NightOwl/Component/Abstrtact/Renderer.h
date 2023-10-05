#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Mat4.h"
#include <memory>
#include <vector>

namespace NightOwl
{
	class Mesh;
	class Material;

	class Renderer : public Component
	{
	public:
		Renderer();

		void CloneRenderer(const std::shared_ptr<Renderer>& clone);

		void Draw();

		std::shared_ptr<Mesh> GetMesh();

		void SetMesh(std::shared_ptr<Mesh> mesh);

		void SetVisible(bool isVisible);

		bool IsVisible();

		const std::shared_ptr<Material> GetMaterial();

		void SetMaterial(const std::shared_ptr<Material>& material);

		const std::vector<std::shared_ptr<Material>>& GetMaterials();

		void SetMaterials(const std::vector<std::shared_ptr<Material>>& materials);

		std::vector<Mat4F>& GetFinalBoneMatrices();

	protected:
		std::shared_ptr<Mesh> mesh;

		std::vector<std::shared_ptr<Material>> materials;

		bool isVisible;

		std::vector<Mat4F> finalBoneMatrices;

		friend class AssimpModelLoader;
	};
}
