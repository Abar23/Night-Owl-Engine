#include "TestScene.h"

#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/SkinnedMeshRenderer.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Graphics/Materials/IMaterial.h"

TestScene::TestScene()
	: Scene("TestScene")
{
}

void TestScene::Init()
{
	auto* assetManager = NightOwl::AssetManagerLocator::GetAssetManager();

	assetManager->LoadModel("./assets/Crouch Stop Walking.fbx");
	assetManager->LoadAnimation("./assets/Crouch Stop Walking.fbx");

	NightOwl::Model* model = assetManager->GetModelRepository().GetAsset("Crouch Stop Walking");
	NightOwl::Animation* animation = assetManager->GetAnimationRepository().GetAsset("mixamo.com");

	auto& rootGameObject = AddGameObject("Crouch Stop Walking");
	auto* renderer = rootGameObject.AddComponent<NightOwl::MeshRenderer>();

	// Make sure mesh gets a copy
	renderer->SetMesh(model->renderer->GetMesh());

	renderer->SetMaterial(model->renderer->GetMaterial());
	NightOwl::GameObject& skeleton = AddCloneOfGameObject(model->skeleton[0]);
	skeleton.GetTransform()->SetParent(rootGameObject.GetTransform());

	auto* animator = rootGameObject.AddComponent<NightOwl::Animator>();
	animator->AddAnimation(animation);
	animator->SetCurrentAnimation("mixamo.com");
	animator->SetSkeleton(skeleton.GetTransform());
	animator->Play();

	// modelMesh->SetColorPerVertex({0.0f, 255.0f, 0.0f});
	// NightOwl::GameObject& backpack = AddGameObject("backpack");
	// NightOwl::MeshRenderer* backpackRenderer = backpack.AddComponent<NightOwl::MeshRenderer>();
	// backpackRenderer->SetMesh(modelMesh);
	// //backpackRenderer->GetMaterial()->SetTexture(assetManager->GetTextureRepository().GetAsset("diffuse.jpg"));
	// backpack.GetTransform()->Rotate({0, 9, 0}, NightOwl::Space::World);
	//
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
	mainCameraGameObject.GetTransform()->Translate({ 0.0f, 100.0f, 325.0f }, NightOwl::Space::World);
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
