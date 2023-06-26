#include "Application.h"
#include <NightOwl/Window/WindowApi.h>
#include <NightOwl/Core/Locator/SceneManagerLocator.h>
#include <NightOwl/Input/Input.h>

namespace Sandbox
{
	Application::Application()
		:	sceneNumber(0),
			shouldDisableCursor(true)
	{
	}

	void Application::Init()
	{
		
	}

	void Application::Update()
	{
		if (NightOwl::Input::IsKeyPressed(NightOwl::KeyCode::F11))
		{
			ToggleCursorAndFullScreen();
		}
	}

	void Application::Shutdown()
	{

	}

	void Application::ToggleCursorAndFullScreen()
	{
		NightOwl::WindowApi::GetWindow()->ToggleFullScreen();

		if (shouldDisableCursor)
		{
			#ifdef RELEASE
			NightOwl::WindowApi::GetWindow()->DisableCursor();
			#endif
		}
		else
		{
			NightOwl::WindowApi::GetWindow()->HideCursor();
		}

		shouldDisableCursor = !shouldDisableCursor;
	}
}

std::shared_ptr<NightOwl::IApplication> NightOwl::CreateApplication()
{
	return  std::make_shared<Sandbox::Application>();
}
