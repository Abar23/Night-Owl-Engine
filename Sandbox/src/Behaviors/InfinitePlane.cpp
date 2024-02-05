#include "InfinitePlane.h"

#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Component/Structures/SubMeshData.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Math/Vec3.h"

void InfinitePlane::Start()
{
	static const std::vector<NightOwl::Vec3F> VERTICES = {
		{ -1.0f, -1.0f, 0.0f }, // bottom left
		{ -1.0f,  1.0f, 0.0f }, // top left
		{  1.0f, -1.0f, 0.0f }, // bottom right
		{  1.0f,  1.0f, 0.0f }  // top right
	};

	static const std::vector<NightOwl::Vec3UI> INDICES = {
		{ 0, 2, 1 },
		{ 1, 2, 3 }
	};

	static const std::vector<NightOwl::SubMeshData> SUB_MESH = {
		{ 0, 6, 0, 2, 0 }
	};

	NightOwl::MeshRenderer* renderer = gameObject->GetComponent<NightOwl::MeshRenderer>();

	NightOwl::IShader* shader = NightOwl::AssetManagerLocator::Get()->GetShaderRepository().GetAsset("InfinitePlane");

	renderer->GetMesh()->SetVertices(VERTICES);
	renderer->GetMesh()->SetTriangles(INDICES);
	renderer->GetMesh()->SetSubMeshes(SUB_MESH);
	renderer->GetMesh()->UploadMeshData(true);

	renderer->GetMaterial()->SetShader(shader);
	renderer->GetMaterial()->SetVec4F(NightOwl::Vec4F(1.0f, 0.0f, 0.0f, 1.0f), "diffuseColor");
}
