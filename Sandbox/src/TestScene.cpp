#include "TestScene.h"

#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/SkinnedMeshRenderer.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Graphics/Materials/Material.h"

TestScene::TestScene()
	: Scene("TestScene")
{
}

void TestScene::Init()
{
	auto* assetManager = NightOwl::AssetManagerLocator::GetAssetManager();

	// Drunk walk scene
	assetManager->LoadModel("./assets/Y Bot.dae");
	assetManager->LoadAnimation("./assets/Shoved Reaction With Spin.dae");

	NightOwl::Model* model = assetManager->GetModelRepository().GetAsset("Y Bot");
	NightOwl::Animation* shoveAnimation = assetManager->GetAnimationRepository().GetAsset("Shoved Reaction With Spin");

	auto& rootGameObject = AddGameObject("Y Bot");
	auto* renderer = rootGameObject.AddComponent<NightOwl::MeshRenderer>();
	rootGameObject.GetTransform()->Scale(NightOwl::Vec3F(1.5f), NightOwl::Space::World);
	// Make sure mesh gets a copy
	renderer->CloneRenderer(model->renderer);
	
	NightOwl::GameObject& skeleton = AddCloneOfGameObject(model->skeleton[0]);
	skeleton.GetTransform()->SetParent(rootGameObject.GetTransform());
	
	auto* animator = rootGameObject.AddComponent<NightOwl::Animator>();
	animator->AddAnimation(shoveAnimation);
	animator->SetCurrentAnimation("Shoved Reaction With Spin");
	animator->SetSkeleton(skeleton.GetTransform());
	animator->Play();
	
	NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	mainCameraGameObject.AddComponent<NightOwl::Camera>();
	mainCameraGameObject.GetTransform()->Translate({ 0.0f, 2, 10 }, NightOwl::Space::World);

	// backpack test
	// assetManager->LoadModel("./assets/backpack/backpack.obj");
	//
	// const NightOwl::Model* backpackModel = assetManager->GetModelRepository().GetAsset("backpack");
	// NightOwl::GameObject& backpack = AddGameObject("backpack");
	// NightOwl::MeshRenderer* backpackRenderer = backpack.AddComponent<NightOwl::MeshRenderer>();
	// backpackRenderer->CloneRenderer(backpackModel->renderer);
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
