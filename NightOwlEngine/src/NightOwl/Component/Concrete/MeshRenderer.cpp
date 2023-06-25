#include <NightOwlPch.h>

#include "MeshRenderer.h"
#include "NightOwl/Graphics/Materials/StandardMaterial.h"
#include "NightOwl/Core/Locator/MeshRenderSystemLocator.h"

namespace NightOwl
{
	MeshRenderer::MeshRenderer()
		:	Component(ComponentType::MeshRenderer),
			isVisible(true)
	{
		material = std::make_shared<StandardMaterial>();
		MeshRendererSystemLocator::GetMeshRendererSystem()->AddMeshRenderer(this);
	}

	std::shared_ptr<Component> MeshRenderer::Clone()
	{
		std::shared_ptr<MeshRenderer> clone = std::make_shared<MeshRenderer>();

		clone->mesh.SetUVs(mesh.GetUVs());
		clone->mesh.SetVertices(mesh.GetVertices());
		clone->mesh.SetColors(mesh.GetColors());
		clone->mesh.SetTriangles(mesh.GetTriangles());
		clone->material = material->Clone();
		clone->isVisible = isVisible;

		return clone;
	}

	void MeshRenderer::Draw()
	{
		if(isVisible)
		{
			material->Draw(*this);
		}
	}

	Mesh* MeshRenderer::GetMesh()
	{
		return &mesh;
	}

	void MeshRenderer::SetVisible(bool isVisible)
	{
		this->isVisible = isVisible;
	}

	bool MeshRenderer::IsVisible()
	{
		return isVisible;
	}

	const std::shared_ptr<IMaterial> MeshRenderer::GetMaterial()
	{
		return material;
	}

	void MeshRenderer::SetMaterial(std::shared_ptr<IMaterial> material)
	{
		this->material = material;
	}

	void MeshRenderer::Remove()
	{
		MeshRendererSystemLocator::GetMeshRendererSystem()->RemoveMeshRenderer(this);
	}

	START_REFLECTION(MeshRenderer)
	CLASS_MEMBER_REFLECTION(mesh)
	CLASS_MEMBER_REFLECTION(isVisible)
	END_REFLECTION(MeshRenderer)
}
