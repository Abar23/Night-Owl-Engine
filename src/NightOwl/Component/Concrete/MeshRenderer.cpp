#include "MeshRenderer.h"
#include "NightOwl/Component/Materials/StandardMaterial.h"
#include "NightOwl/Core/Locator/MeshRenderSystemLocator.h"

namespace NightOwl::Component
{
	MeshRenderer::MeshRenderer()
		:	Component(ComponentType::MeshRenderer),
			isVisible(true)
	{
		material = std::make_shared<StandardMaterial>();
		Core::MeshRendererSystemLocator::GetMeshRendererSystem()->AddMeshRenderer(this);
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

	START_REFLECTION(MeshRenderer)
	CLASS_MEMBER_REFLECTION(mesh)
	CLASS_MEMBER_REFLECTION(isVisible)
	END_REFLECTION(MeshRenderer)
}
