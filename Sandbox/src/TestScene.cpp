#include "TestScene.h"

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

	// Get models
	NightOwl::Model* sphere = assetManager->GetModelRepository().GetAsset("sphere");

	// Sphere IK control object
	auto& sphereGameObject = AddGameObject("Target");
	sphereGameObject.AddComponent<IkTargetController>();
	auto* sphereRenderer = sphereGameObject.AddComponent<NightOwl::MeshRenderer>();
	sphereRenderer->CloneRenderer(sphere->renderer);

	// cloth plane
	auto& clothPlaneGameObject = AddGameObject("cloth plane");
	clothPlaneGameObject.AddComponent<NightOwl::MeshRenderer>();
	clothPlaneGameObject.AddComponent<ImGuiInterface>();
	auto* planarClothComponent = clothPlaneGameObject.AddComponent<NightOwl::PlanarCloth>();
	planarClothComponent->ConstructClothWithDimension(33);
	planarClothComponent->SetSphereCollider(sphereGameObject.GetTransform(), 0.52f);

	// Camera
	NightOwl::GameObject& mainCameraGameObject = AddGameObject("Main Camera");
	mainCameraGameObject.AddComponent<NightOwl::Camera>();
	mainCameraGameObject.AddComponent<CameraController>();

	// Infinite floor plane
	auto& infinitePlane = AddGameObject("Infinite Plane");
	infinitePlane.AddComponent<NightOwl::MeshRenderer>();
	infinitePlane.AddComponent<InfinitePlane>();

	auto& lightTestObject = AddGameObject("Light");
	lightTestObject.GetTransform()->Translate({ 0.5f, 0.5f, 0.5f }, NightOwl::Space::World);
	auto* lightComponent = lightTestObject.AddComponent<NightOwl::Light>();
	lightComponent->SetColor({ 0.0f, 0.0f, 1.0f });

	auto& lightTestObject2 = AddGameObject("Light");
	lightTestObject2.GetTransform()->Translate({ 0.5f, 0.5f, 0.5f }, NightOwl::Space::World);
	auto* lightComponent2 = lightTestObject2.AddComponent<NightOwl::Light>();
	lightComponent2->SetColor({ 0.0f, 0.0f, 1.0f });

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
