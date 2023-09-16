#include "NightOwlPch.h"

#include "SkinnedMeshRenderer.h"
#include "NightOwl/Graphics/Materials/Material.h"

namespace NightOwl
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
	{
		type = ComponentType::SkinnedMeshRenderer;
		material = std::make_shared<Material>();
	}

	std::shared_ptr<Component> SkinnedMeshRenderer::Clone()
	{
		return Renderer::Clone();
	}

	void SkinnedMeshRenderer::Remove()
	{
	}
}
