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

	// // Drunk walk scene
	// assetManager->LoadModel("./assets/Drunk Walking Turn.fbx");
	// assetManager->LoadAnimation("./assets/Drunk Walking Turn.fbx");
	//
	// NightOwl::Model* model = assetManager->GetModelRepository().GetAsset("Drunk Walking Turn");
	// NightOwl::Animation* animation = assetManager->GetAnimationRepository().GetAsset("mixamo.com");
	//
	// auto& rootGameObject = AddGameObject("Drunk Walking Turn");
	// auto* renderer = rootGameObject.AddComponent<NightOwl::MeshRenderer>();
	//
	// // Make sure mesh gets a copy
	// renderer->SetMesh(model->renderer->GetMesh());
	//
	// renderer->SetMaterial(model->renderer->GetMaterial());
	// NightOwl::GameObject& skeleton = AddCloneOfGameObject(model->skeleton[0]);
	// skeleton.GetTransform()->SetParent(rootGameObject.GetTransform());
	//
	// auto* animator = rootGameObject.AddComponent<NightOwl::Animator>();
	// animator->AddAnimation(animation);
	// animator->SetCurrentAnimation("mixamo.com");
	// animator->SetSkeleton(skeleton.GetTransform());
	// animator->Play();
	//
	// NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	// mainCameraGameObject.AddComponent<NightOwl::Camera>();
	// mainCameraGameObject.GetTransform()->Translate({ 0.0f, 100.0f, 325.0f }, NightOwl::Space::World);

	// backpack test
	assetManager->LoadModel("./assets/backpack/backpack.obj");
	
	std::shared_ptr<NightOwl::Mesh> modelMesh = assetManager->GetModelRepository().GetAsset("backpack")->renderer->GetMesh();
	NightOwl::GameObject& backpack = AddGameObject("backpack");
	NightOwl::MeshRenderer* backpackRenderer = backpack.AddComponent<NightOwl::MeshRenderer>();
	backpackRenderer->SetMesh(modelMesh);

	NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	mainCameraGameObject.AddComponent<NightOwl::Camera>();
	mainCameraGameObject.GetTransform()->Translate({ 0.0f, 0.0f, 10.0f }, NightOwl::Space::World);

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
