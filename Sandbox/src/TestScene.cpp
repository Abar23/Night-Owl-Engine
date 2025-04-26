#include "TestScene.h"

#include "Behaviors/CameraController.h"
#include "Behaviors/IkTargetController.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/Light.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/SkinnedMeshRenderer.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Graphics.h"
#include "NightOwl/Graphics/Materials/Material.h"
#include <random>

#include "NightOwl/Graphics/Types/TextureFilterMode.h"
#include "NightOwl/Graphics/Types/TextureWrapMode.h"

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
	// assetManager->LoadModel("./assets/Dragons/Asian Dragon.stl");
	// assetManager->LoadModel("./assets/Dragons/Syrax Head.stl");
	assetManager->LoadModel("./assets/Squirrel/Squirrel.dae");
	assetManager->LoadTexture2D("./assets/HdrSkybox/Alexs_Apt_2k_Irradiance.hdr");
	NightOwl::ITexture2D* skybox = assetManager->LoadTexture2D("./assets/HdrSkybox/Alexs_Apt_2k.hdr");
	skybox->SetMaxMipMapLevel(12);
	skybox->SetWrapModeU(NightOwl::TextureWrapMode::Repeat);
	skybox->SetWrapModeV(NightOwl::TextureWrapMode::Repeat);
	skybox->SetFilterMode(NightOwl::TextureFilterMode::Trilinear);

	// Get models
	//const NightOwl::Model* plane = assetManager->GetModelRepository().GetAsset("plane");
	const NightOwl::Model* squirrel = assetManager->GetModelRepository().GetAsset("Squirrel");

	// Setup squirrel model
	auto& squirrelObject = AddGameObject("Squirrel");
	auto* rendererComponent = squirrelObject.AddComponent<NightOwl::MeshRenderer>();
	rendererComponent->CloneRenderer(squirrel->renderer);

	// // Sphere IK control object
	// auto& planeGameObject = AddGameObject("Target");
	// planeGameObject.AddComponent<IkTargetController>();
	// rendererComponent = planeGameObject.AddComponent<NightOwl::MeshRenderer>();
	// rendererComponent->CloneRenderer(plane->renderer);
	// rendererComponent->GetMaterial()->SetVec4F({ 0.5, 0.5, 0.5, 1.0 }, "diffuseColor");

	// Camera
	NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	auto* cameraComponent = mainCameraGameObject.AddComponent<NightOwl::Camera>();
	cameraComponent->SetFarClippingPlane(20000.0f);
	mainCameraGameObject.AddComponent<CameraController>();

	// Create a random engine
	std::default_random_engine generator;

	// Create a uniform distribution for floating-point values between 0 and 1
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	constexpr int numberOfLights = 1;
	constexpr float xStartingPosition = -10.0f;
	constexpr float zStartingPosition = -10.0f;
	constexpr float lightSpacing = -1.0f * xStartingPosition / numberOfLights;
	for (int i = 0; i < numberOfLights; ++i)
	{
		for (int lightIndex = 0; lightIndex < numberOfLights; ++lightIndex)
		{
			auto& lightTestObject = AddGameObject("Light");
			auto* lightComponent = lightTestObject.AddComponent<NightOwl::Light>();
			lightComponent->SetColor({ distribution(generator), distribution(generator), distribution(generator) });
			lightComponent->SetRange(1000.0f);
			lightComponent->SetIntensity(100.0f);
			lightTestObject.GetTransform()->SetPosition({ xStartingPosition + lightSpacing * i, 0.5f, zStartingPosition + lightSpacing * lightIndex });
		}
	}

	// Create global directional light
	auto& globalLightGameObject = AddGameObject("Global Light");
	auto* globalLightComponent = globalLightGameObject.AddComponent<NightOwl::Light>();
	globalLightComponent->SetShadows(NightOwl::LightShadows::Soft);
	globalLightComponent->SetType(NightOwl::LightType::Directional);
	globalLightComponent->SetIntensity(10.0f);
	NightOwl::Light::SetGlobalLight(globalLightComponent);
	globalLightGameObject.GetTransform()->Rotate({ -90.0f, 0.0f, 0.0f }, NightOwl::Space::World);
	globalLightGameObject.GetTransform()->SetPosition({ 0.0f, 80.0f, 0.0f });
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
