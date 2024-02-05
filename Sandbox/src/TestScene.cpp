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
	NightOwl::Model* sphere = assetManager->GetModelRepository().GetAsset("sphere");
	NightOwl::Model* plane = assetManager->GetModelRepository().GetAsset("plane");

	// Sphere IK control object
	auto& sphereGameObject = AddGameObject("Target");
	sphereGameObject.AddComponent<IkTargetController>();
	auto* sphereRenderer = sphereGameObject.AddComponent<NightOwl::MeshRenderer>();
	sphereRenderer->CloneRenderer(plane->renderer);
	// cloth plane
	// auto& clothPlaneGameObject = AddGameObject("cloth plane");
	// clothPlaneGameObject.AddComponent<NightOwl::MeshRenderer>();
	// clothPlaneGameObject.AddComponent<ImGuiInterface>();
	// auto* planarClothComponent = clothPlaneGameObject.AddComponent<NightOwl::PlanarCloth>();
	// planarClothComponent->ConstructClothWithDimension(33);
	// planarClothComponent->SetSphereCollider(sphereGameObject.GetTransform(), 0.52f);

	// Camera
	NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	mainCameraGameObject.AddComponent<NightOwl::Camera>();
	mainCameraGameObject.AddComponent<CameraController>();

	// Infinite floor plane
	// auto& infinitePlane = AddGameObject("Infinite Plane");
	// infinitePlane.AddComponent<NightOwl::MeshRenderer>();
	// infinitePlane.AddComponent<InfinitePlane>();

	// Create a random engine
	std::default_random_engine generator;

	// Create a uniform distribution for floating-point values between 0 and 1
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	float xStartingPosition = -20.0f;
	float zStartingPosition = -20.0f;
	float lightSpacing = 1.0f;
	for (int i = 0; i < 32; ++i)
	{
		for (int lightIndex = 0; lightIndex < 32; ++lightIndex)
		{
			auto& lightTestObject = AddGameObject("Light");
			auto* lightComponent = lightTestObject.AddComponent<NightOwl::Light>();
			lightComponent->SetColor({ distribution(generator), distribution(generator), distribution(generator) });
			// auto* lightRenderer = lightTestObject.AddComponent<NightOwl::MeshRenderer>();
			// lightRenderer->CloneRenderer(sphere->renderer);
			lightTestObject.GetTransform()->Scale({ 0.1f, 0.1f, 0.1f }, NightOwl::Space::World);
			lightTestObject.GetTransform()->SetPosition({ xStartingPosition + lightSpacing * i, 0.0f, zStartingPosition + lightSpacing * lightIndex });
		}
	}


	// auto& lightTestObject = AddGameObject("Light");
	// auto* lightComponent = lightTestObject.AddComponent<NightOwl::Light>();
	// lightComponent->SetColor({ distribution(generator), distribution(generator), distribution(generator) });
	// lightTestObject.GetTransform()->Scale({ 0.1f, 0.1f, 0.1f }, NightOwl::Space::World);
	// lightTestObject.GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
	//
	// auto& lightTestObject2 = AddGameObject("Light");
	// auto* lightComponent2 = lightTestObject2.AddComponent<NightOwl::Light>();
	// lightComponent2->SetColor({ distribution(generator), distribution(generator), distribution(generator) });
	// lightTestObject2.GetTransform()->Scale({ 0.1f, 0.1f, 0.1f }, NightOwl::Space::World);
	// lightTestObject2.GetTransform()->SetPosition({ 1.0f, 0.0f, 0.0f });

	//NightOwl::Graphics::GetContext()->SetClearColor(NightOwl::Vec4F(0.2f, 0.2f, 0.2f, 1.0f));
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
