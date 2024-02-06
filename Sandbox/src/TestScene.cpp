#include "TestScene.h"

#include <random>

#include "Behaviors/CameraController.h"
#include "Behaviors/IkTargetController.h"
#include "Behaviors/ImGuiInterface.h"
#include "Behaviors/InfinitePlane.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/Light.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/PlanarCloth.h"
#include "NightOwl/Component/Concrete/SkinnedMeshRenderer.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Graphics.h"

class InfinitePlane;

TestScene::TestScene()
	: Scene("TestScene")
{
}

void TestScene::Init()
{
	auto* assetManager = NightOwl::AssetManagerLocator::Get();

	// Load assets for the scene
	assetManager->LoadShaders("./assets/Shaders");
	assetManager->LoadModel("./assets/Sphere/sphere.obj");
	assetManager->LoadModel("./assets/Plane/plane.obj");

	// Get models
	NightOwl::Model* plane = assetManager->GetModelRepository().GetAsset("plane");

	// Sphere IK control object
	auto& sphereGameObject = AddGameObject("Target");
	sphereGameObject.AddComponent<IkTargetController>();
	auto* sphereRenderer = sphereGameObject.AddComponent<NightOwl::MeshRenderer>();
	sphereRenderer->CloneRenderer(plane->renderer);

	// Camera
	NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	mainCameraGameObject.AddComponent<NightOwl::Camera>();
	mainCameraGameObject.AddComponent<CameraController>();

	// Create a random engine
	std::default_random_engine generator;

	// Create a uniform distribution for floating-point values between 0 and 1
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	constexpr float xStartingPosition = -20.0f;
	constexpr float zStartingPosition = -20.0f;
	for (int i = 0; i < 50; ++i)
	{
		for (int lightIndex = 0; lightIndex < 50; ++lightIndex)
		{
			constexpr float lightSpacing = 0.5f;
			auto& lightTestObject = AddGameObject("Light");
			auto* lightComponent = lightTestObject.AddComponent<NightOwl::Light>();
			lightComponent->SetColor({ distribution(generator), distribution(generator), distribution(generator) });
			lightTestObject.GetTransform()->Scale({ 0.0f, 0.1f, 0.1f }, NightOwl::Space::World);
			lightTestObject.GetTransform()->SetPosition({ xStartingPosition + lightSpacing * i, 0.0f, zStartingPosition + lightSpacing * lightIndex });
		}
	}
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
