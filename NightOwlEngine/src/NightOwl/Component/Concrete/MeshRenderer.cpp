#include <NightOwlPch.h>

#include "MeshRenderer.h"
#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Core/Locator/MeshRenderSystemLocator.h"

namespace NightOwl
{
	MeshRenderer::MeshRenderer()
	{
		type = ComponentType::MeshRenderer;
		materials.push_back(std::make_shared<Material>());
		MeshRendererSystemLocator::GetMeshRendererSystem()->AddMeshRenderer(this);
	}

	std::shared_ptr<Component> MeshRenderer::Clone()
	{
		std::shared_ptr<MeshRenderer> clone = std::make_shared<MeshRenderer>();

		CloneRenderer(clone);

		return clone;
	}

	void MeshRenderer::Remove()
	{
		MeshRendererSystemLocator::GetMeshRendererSystem()->RemoveMeshRenderer(this);
	}
}
