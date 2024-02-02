#include <NightOwlPch.h>

#include "MeshRenderer.h"
#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/Graphics/System/MeshRendererSystem.h"

namespace NightOwl
{
	MeshRenderer::MeshRenderer()
	{
		type = ComponentType::MeshRenderer;
		materials.push_back(std::make_shared<Material>());
		MeshRendererSystemLocator::Get()->AddMeshRenderer(this);
	}

	std::shared_ptr<Component> MeshRenderer::Clone()
	{
		std::shared_ptr<MeshRenderer> clone = std::make_shared<MeshRenderer>();

		CloneRenderer(clone);

		return clone;
	}

	void MeshRenderer::Remove()
	{
		MeshRendererSystemLocator::Get()->RemoveMeshRenderer(this);
	}
}
