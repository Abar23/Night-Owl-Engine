#include "NightOwlPch.h"

#include "Renderer.h"
#include "NightOwl/Graphics/Materials/Material.h"

namespace NightOwl
{
	Renderer::Renderer()
		: isVisible(true)
	{
	}

	void Renderer::Draw()
	{
		if (isVisible)
		{
			// TODO: move all of the material draw functionality into this method
			material->Draw(*this);
		}
	}

	std::shared_ptr<Mesh> Renderer::GetMesh()
	{
		return mesh;
	}

	void Renderer::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		this->mesh = mesh;
	}

	void Renderer::SetVisible(bool isVisible)
	{
		this->isVisible = isVisible;
	}

	bool Renderer::IsVisible()
	{
		return isVisible;
	}

	const std::shared_ptr<Material> Renderer::GetMaterial()
	{
		return material;
	}

	void Renderer::SetMaterial(std::shared_ptr<Material> material)
	{
		this->material = material;
	}

	void Renderer::CloneRenderer(const std::shared_ptr<Renderer>& clone)
	{
		clone->mesh->SetUVs(mesh->GetUVs());
		clone->mesh->SetVertices(mesh->GetVertices());
		clone->mesh->SetColors(mesh->GetColors());
		clone->mesh->SetTriangles(mesh->GetTriangles());
		clone->material = material->Clone();
		clone->isVisible = isVisible;
	}
}
