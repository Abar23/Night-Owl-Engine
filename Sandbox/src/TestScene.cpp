#include "TestScene.h"

#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Graphics/Materials/IMaterial.h"

TestScene::TestScene()
	: Scene("TestScene")
{
}

void TestScene::Init()
{
	auto* assetManager = NightOwl::AssetManagerLocator::GetAssetManager();

	assetManager->LoadModel("./assets/Viking rigged1/Viking rigged1.fbx");

	NightOwl::Mesh* modelMesh = assetManager->GetModelRepository().GetAsset("Viking rigged1");
	modelMesh->SetColorPerVertex({0.0f, 255.0f, 0.0f});
	NightOwl::GameObject& backpack = AddGameObject("backpack");
	NightOwl::MeshRenderer* backpackRenderer = backpack.AddComponent<NightOwl::MeshRenderer>();
	backpackRenderer->SetMesh(modelMesh);
	//backpackRenderer->GetMaterial()->SetTexture(assetManager->GetTextureRepository().GetAsset("diffuse.jpg"));
	backpack.GetTransform()->Rotate({0, 9, 0}, NightOwl::Space::World);

	// assetManager->LoadModel("./assets/backpack/backpack.obj");
	//
	// NightOwl::Mesh* modelMesh = assetManager->GetModelRepository().GetAsset("backpack");
	// modelMesh->SetColorPerVertex({ 0.0f, 255.0f, 0.0f });
	// NightOwl::GameObject& backpack = AddGameObject("backpack");
	// NightOwl::MeshRenderer* backpackRenderer = backpack.AddComponent<NightOwl::MeshRenderer>();
	// backpackRenderer->SetMesh(modelMesh);
	// backpackRenderer->GetMaterial()->SetTexture(assetManager->GetTextureRepository().GetAsset("diffuse.jpg"));
	// backpack.GetTransform()->Rotate({ 0, 9, 0 }, NightOwl::Space::World);

	NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	mainCameraGameObject.AddComponent<NightOwl::Camera>();
	mainCameraGameObject.GetTransform()->Translate({ 0.0f, 0.0f, 100.0f }, NightOwl::Space::World);
}

void TestScene::Reset()
{
}

void TestScene::Load()
{
}

void TestScene::Shutdown()
{
}
