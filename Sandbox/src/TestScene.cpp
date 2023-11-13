#include "TestScene.h"

#include "Behaviors/CameraController.h"
#include "Behaviors/IkTargetController.h"
#include "Behaviors/ImGuiInterface.h"
#include "Behaviors/InfinitePlane.h"
#include "Behaviors/SplineAnimator.h"
#include "Behaviors/SplineDebugger.h"
#include "NightOwl/Animation/3D/Animation.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/CatmullRomSpline.h"
#include "NightOwl/Component/Concrete/ChainIK.h"
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

	// Load assets for the scene
	assetManager->LoadShaders("./assets/Shaders");
	assetManager->LoadModel("./assets/Bot/Y Bot.dae");
	assetManager->LoadModel("./assets/Sphere/sphere.obj");
	assetManager->LoadAnimation("./assets/Bot/Shoved Reaction With Spin.dae");
	assetManager->LoadAnimation("./assets/Bot/Start Walking.dae");
	assetManager->LoadAnimation("./assets/Bot/Drunk Walking Turn.dae");
	assetManager->LoadAnimation("./assets/Bot/Running Slide.dae");
	assetManager->LoadAnimation("./assets/Bot/Standard Walk.dae");
	assetManager->LoadAnimation("./assets/Bot/Standard Idle.dae");
	assetManager->LoadAnimation("./assets/Bot/Standard Run.dae");

	// Get loaded assets
	NightOwl::Model* model = assetManager->GetModelRepository().GetAsset("Y Bot");
	NightOwl::Model* sphere = assetManager->GetModelRepository().GetAsset("sphere");
	NightOwl::Animation* shoveAnimation = assetManager->GetAnimationRepository().GetAsset("Shoved Reaction With Spin");
	NightOwl::Animation* startWalkingAnimation = assetManager->GetAnimationRepository().GetAsset("Start Walking");
	NightOwl::Animation* drunkWalkingTurn = assetManager->GetAnimationRepository().GetAsset("Drunk Walking Turn");
	NightOwl::Animation* runningSlideAnimation = assetManager->GetAnimationRepository().GetAsset("Running Slide");
	NightOwl::Animation* idle = assetManager->GetAnimationRepository().GetAsset("Standard Idle");
	NightOwl::Animation* walk = assetManager->GetAnimationRepository().GetAsset("Standard Walk");
	NightOwl::Animation* run = assetManager->GetAnimationRepository().GetAsset("Standard Run");


	// Sphere IK control object
	auto& sphereGameObject = AddGameObject("Sphere");

	sphereGameObject.AddComponent<IkTargetController>();

	auto* sphereRenderer = sphereGameObject.AddComponent<NightOwl::MeshRenderer>();
	sphereRenderer->CloneRenderer(sphere->renderer);

	auto& yBotGameObject = AddGameObject("Y Bot");

	yBotGameObject.AddComponent<NightOwl::CatmullRomSpline>();
	yBotGameObject.AddComponent<SplineDebugger>();

	auto* renderer = yBotGameObject.AddComponent<NightOwl::MeshRenderer>();
	// Make sure mesh gets a copy
	renderer->CloneRenderer(model->renderer);
	
	NightOwl::GameObject& skeleton = AddCloneOfGameObject(model->skeleton[0]);
	skeleton.GetTransform()->SetParent(yBotGameObject.GetTransform());

	yBotGameObject.GetTransform()->Scale(NightOwl::Vec3F(2.0f), NightOwl::Space::World);

	// Setup ik chain
	auto* chain = yBotGameObject.AddComponent<NightOwl::ChainIK>();
	// chain->AddJointToChain(FindWithName("mixamorig_LeftShoulder")->GetTransform());
	chain->AddJointToChain(FindWithName("mixamorig_LeftArm")->GetTransform());
	chain->AddJointToChain(FindWithName("mixamorig_LeftForeArm")->GetTransform());
	chain->AddJointToChain(FindWithName("mixamorig_LeftHand")->GetTransform());
	chain->AddJointToChain(FindWithName("mixamorig_LeftHandThumb1")->GetTransform());
	chain->AddJointToChain(FindWithName("mixamorig_LeftHandThumb2")->GetTransform());
	chain->AddJointToChain(FindWithName("mixamorig_LeftHandThumb3")->GetTransform());
	// chain->AddJointToChain(FindWithName("mixamorig_LeftHandThumb4")->GetTransform());
	chain->SetTarget(sphereGameObject.GetTransform());

	// Setup Blend tree
	simple1DBlendTree = std::make_shared<NightOwl::BlendTree>();
	simple1DBlendTree->SetName("simple1DBlendTree");
	simple1DBlendTree->AddBlendTreeNode({ idle,  0.0f, 1.0f });
	simple1DBlendTree->AddBlendTreeNode({ walk,   0.15f, 1.0f });
	simple1DBlendTree->AddBlendTreeNode({ run, 1.0f, 1.0f });
	simple1DBlendTree->SetBlendParameterXName("velocity");

	auto* animator = yBotGameObject.AddComponent<NightOwl::Animator>();
	// animator->AddMotion(shoveAnimation);
	// animator->AddMotion(startWalkingAnimation);
	// animator->AddMotion(drunkWalkingTurn);
	// animator->AddMotion(runningSlideAnimation);
	// animator->AddMotion(simple1DBlendTree.get());
	// animator->SetCurrentMotion("simple1DBlendTree");
	// animator->SetFloat(simple1DBlendTree->GetBlendParameterXName(), 0.0f);
	animator->SetSkeleton(skeleton.GetTransform());
	// animator->Play();
	
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
