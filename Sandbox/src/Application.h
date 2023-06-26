#pragma once

#include "NightOwl/Core/Application/IApplication.h"

namespace Sandbox
{
	class Application : public NightOwl::IApplication
	{
	public:
		Application();
		
		void Init() override;

		void Update() override;

		void Shutdown() override;

		void ToggleCursorAndFullScreen();

	private:
		int sceneNumber;

		bool shouldDisableCursor;
	};
}
