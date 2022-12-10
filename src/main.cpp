#include "NightOwl/Behavior/OwlBehaviorManager.h"
#include "NightOwl/Core/Locator/OwlBehaviorManagerLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/Math/Math.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Window/WindowApi.h"
#include "NightOwl/Input/Input.h"
#include "NightOwl/Physics/2D/PhysicsEngine2D.h"
#include "NightOwl/Core/Locator/PhysicsEngine2DLocator.h"
#include "BrickBreak/BrickBreak.h"
#include "NightOwl/Core/Locator/MeshRenderSystemLocator.h"
#include "NightOwl/Graphics/System/MeshRendererSystem.h"

int main()
{
	// Standup necessary systems
	NightOwl::Utility::LoggerManager::Init();
	NightOwl::Window::WindowApi::CreateWindow("Night Owl Engine Demo", 600, 800);
	NightOwl::Input::Input::Init();
	NightOwl::Core::Time::Init();
	NightOwl::Behavior::OwlBehaviorManager manager;
	NightOwl::Graphics::MeshRendererSystem renderSystem;
	NightOwl::Physics::PhysicsEngine2D engine;
	NightOwl::Core::OwlBehaviorManagerLocator::Provide(&manager);
	NightOwl::Core::MeshRendererSystemLocator::Provide(&renderSystem);
	NightOwl::Core::PhysicsEngine2DLocator::Provide(&engine);

	BrickBreak::BrickBreak brick;
	brick.Init();
	brick.Run();

	manager.InitBehaviors();

	while(!NightOwl::Window::WindowApi::GetWindow()->ShouldWindowClose())
	{
		NightOwl::Graphics::RenderApi::GetContext()->ClearColor(NightOwl::Math::Vec4F(0.5, 0.4, 0.3, 1.0));
		NightOwl::Graphics::RenderApi::GetContext()->ClearBuffer();

		brick.Run();

		engine.Update();

		manager.Update();

		NightOwl::Input::Input::Update();

		renderSystem.Update();

		NightOwl::Window::WindowApi::GetWindow()->Update();
		NightOwl::Core::Time::Update();
	}

	return 0;
}
