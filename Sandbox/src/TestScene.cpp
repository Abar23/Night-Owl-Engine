#include "TestScene.h"

#include "Behaviors/CameraController.h"
#include "Behaviors/ImGuiInterface.h"
#include "Behaviors/InfinitePlane.h"
#include "Behaviors/SplineDebugger.h"
#include "NightOwl/Animation/3D/Animation.h"
#include "NightOwl/Animation/3D/BlendTree.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/CatmullRomSpline.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/SkinnedMeshRenderer.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Graphics.h"

class InfinitePlane;

TestScene::TestScene()
	: Scene("TestScene")
{
}

void TestScene::Init()
{
	auto* assetManager = NightOwl::AssetManagerLocator::GetAssetManager();

	// Drunk walk scene
	assetManager->LoadShaders("./assets/Shaders");
	assetManager->LoadModel("./assets/Bot/Y Bot.dae");
	assetManager->LoadAnimation("./assets/Bot/Shoved Reaction With Spin.dae");
	assetManager->LoadAnimation("./assets/Bot/Start Walking.dae");
	assetManager->LoadAnimation("./assets/Bot/Drunk Walking Turn.dae");
	assetManager->LoadAnimation("./assets/Bot/Running Slide.dae");
	assetManager->LoadAnimation("./assets/Bot/Standard Walk.dae");
	assetManager->LoadAnimation("./assets/Bot/Standard Idle.dae");
	assetManager->LoadAnimation("./assets/Bot/Standard Run.dae");


	NightOwl::Model* model = assetManager->GetModelRepository().GetAsset("Y Bot");
	NightOwl::Animation* shoveAnimation = assetManager->GetAnimationRepository().GetAsset("Shoved Reaction With Spin");
	NightOwl::Animation* startWalkingAnimation = assetManager->GetAnimationRepository().GetAsset("Start Walking");
	NightOwl::Animation* drunkWalkingTurn = assetManager->GetAnimationRepository().GetAsset("Drunk Walking Turn");
	NightOwl::Animation* runningSlideAnimation = assetManager->GetAnimationRepository().GetAsset("Running Slide");
	NightOwl::Animation* idle = assetManager->GetAnimationRepository().GetAsset("Standard Idle");
	NightOwl::Animation* walk = assetManager->GetAnimationRepository().GetAsset("Standard Walk");
	NightOwl::Animation* run = assetManager->GetAnimationRepository().GetAsset("Standard Run");


	auto& yBotGameObject = AddGameObject("Y Bot");

	yBotGameObject.AddComponent<SplineDebugger>();
	NightOwl::CatmullRomSpline* splineComponent = yBotGameObject.AddComponent<NightOwl::CatmullRomSpline>();

	const int numPoints = 10;
	for (int i = 0; i <= numPoints; ++i) {
		float angleDegrees = i * (360.0f / numPoints);
		float angleRadians = angleDegrees * M_PI / 180.0f;
		float x = std::cos(angleRadians);
		float z = std::sin(angleRadians);
		splineComponent->AddControlPoint(NightOwl::Vec3F(x, 0, z) * 10.f); // (1, 0) - 0 degrees
	}

	auto* renderer = yBotGameObject.AddComponent<NightOwl::MeshRenderer>();
	// Make sure mesh gets a copy
	renderer->CloneRenderer(model->renderer);
	
	NightOwl::GameObject& skeleton = AddCloneOfGameObject(model->skeleton[0]);
	skeleton.GetTransform()->SetParent(yBotGameObject.GetTransform());

	yBotGameObject.GetTransform()->Scale(NightOwl::Vec3F(2.0f), NightOwl::Space::World);

	// Setup Blend tree

	NightOwl::BlendTree* blend = new NightOwl::BlendTree();
	blend->SetName("blend");
	blend->AddBlendTreeNode({ idle,  0.0f, 1.0f });
	blend->AddBlendTreeNode({ walk,   0.10f, 1.0f });
	blend->AddBlendTreeNode({ run, 1.0f, 1.0f });
	blend->SetBlendParameterXName("test");

	auto* animator = yBotGameObject.AddComponent<NightOwl::Animator>();
	animator->AddMotion(shoveAnimation);
	animator->AddMotion(startWalkingAnimation);
	animator->AddMotion(drunkWalkingTurn);
	animator->AddMotion(runningSlideAnimation);
	animator->AddMotion(blend);
	animator->SetCurrentMotion("blend");
	animator->SetFloat(blend->GetBlendParameterXName(), 0.0f);
	animator->SetSkeleton(skeleton.GetTransform());
	animator->Play();
	
	yBotGameObject.AddComponent<ImGuiInterface>();

	NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	mainCameraGameObject.AddComponent<NightOwl::Camera>();
	mainCameraGameObject.AddComponent<CameraController>();

	auto& infinitePlane = AddGameObject("Infinite Plane");
	infinitePlane.AddComponent<NightOwl::MeshRenderer>();
	infinitePlane.AddComponent<InfinitePlane>();

	NightOwl::Graphics::GetContext()->SetClearColor(NightOwl::Vec4F(0.2f, 0.2f, 0.2f, 1.0f));
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
