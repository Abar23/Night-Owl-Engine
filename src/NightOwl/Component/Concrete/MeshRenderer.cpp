#include "MeshRenderer.h"

#include "NightOwl/Component/Materials/StandardMaterial.h"

namespace NightOwl::Component
{
	MeshRenderer::MeshRenderer(GameObject::GameObject& gameObject)
		: Component(&gameObject, ComponentType::MeshRenderer),
		  isVisible(true)
	{
		material = std::make_shared<StandardMaterial>();
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
}
