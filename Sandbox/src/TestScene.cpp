#include "TestScene.h"

#include "Behaviors/CameraController.h"
#include "Behaviors/ImGuiInterface.h"
#include "Behaviors/InfinitePlane.h"
#include "Behaviors/SplineDebugger.h"
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
	
	NightOwl::Model* model = assetManager->GetModelRepository().GetAsset("Y Bot");
	NightOwl::Animation* shoveAnimation = assetManager->GetAnimationRepository().GetAsset("Shoved Reaction With Spin");
	NightOwl::Animation* startWalkingAnimation = assetManager->GetAnimationRepository().GetAsset("Start Walking");
	NightOwl::Animation* drunkWalkingTurn = assetManager->GetAnimationRepository().GetAsset("Drunk Walking Turn");
	NightOwl::Animation* runningSlideAnimation = assetManager->GetAnimationRepository().GetAsset("Running Slide");
	
	auto& yBotGameObject = AddGameObject("Y Bot");

	yBotGameObject.AddComponent<SplineDebugger>();
	NightOwl::CatmullRomSpline* splineComponent = yBotGameObject.AddComponent<NightOwl::CatmullRomSpline>();

	splineComponent->AddControlPoint({  0, 0,  0 });
	splineComponent->AddControlPoint({ -4, 0,  1 });
	splineComponent->AddControlPoint({  4, 0,  2 });
	splineComponent->AddControlPoint({ -4, 0,  3 });
	splineComponent->AddControlPoint({  4, 0,  4 });
	splineComponent->AddControlPoint({ -4, 0,  5 });
	splineComponent->AddControlPoint({  4, 0,  6 });
	splineComponent->AddControlPoint({ -4, 0,  7 });
	splineComponent->AddControlPoint({  4, 0,  8 });
	splineComponent->AddControlPoint({  0, 0,  9 });


	auto* renderer = yBotGameObject.AddComponent<NightOwl::MeshRenderer>();
	// Make sure mesh gets a copy
	renderer->CloneRenderer(model->renderer);
	
	NightOwl::GameObject& skeleton = AddCloneOfGameObject(model->skeleton[0]);
	skeleton.GetTransform()->SetParent(yBotGameObject.GetTransform());

	yBotGameObject.GetTransform()->Scale(NightOwl::Vec3F(2.0f), NightOwl::Space::World);

	auto* animator = yBotGameObject.AddComponent<NightOwl::Animator>();
	animator->AddAnimation(shoveAnimation);
	animator->AddAnimation(startWalkingAnimation);
	animator->AddAnimation(drunkWalkingTurn);
	animator->AddAnimation(runningSlideAnimation);
	animator->SetCurrentAnimation("Shoved Reaction With Spin");
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
