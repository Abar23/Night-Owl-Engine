#include "NightOwlPch.h"

#include "NightOwlEngine.h"
#include "IApplication.h"
#include "NightOwlEngineConstants.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AnimatorSystemLocator.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Core/Locator/AudioSystemLocator.h"
#include "NightOwl/Core/Locator/DebugSystemLocator.h"
#include "NightOwl/Core/Locator/MeshRenderSystemLocator.h"
#include "NightOwl/Core/Locator/OwlBehaviorManagerLocator.h"
#include "NightOwl/Core/Locator/PhysicsEngine2DLocator.h"
#include "NightOwl/Core/Locator/SceneManagerLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Input/Input.h"
#include "NightOwl/Window/WindowApi.h"

namespace NightOwl
{
	NightOwlEngine::NightOwlEngine()
		:	application(nullptr)
	{ }

	void NightOwlEngine::Init()
	{
		ENGINE_ASSERT(application != nullptr, "Night Owl Engine application can't be null");

		// Standup necessary systems
		WindowApi::CreateWindow("Animation Project 1", 900, 1600);
		
		#ifdef DEBUG
		Utility::LoggerManager::Init();
		#endif

		Input::Init();

		Time::Init();

		Time::SetFixedTime(FIXED_UPDATE_TIME_STEP);

		OwlBehaviorManagerLocator::Provide(&owlBehaviorManager);
		MeshRendererSystemLocator::Provide(&meshRendererSystem);
		AnimatorSystemLocator::Provide(&animatorSystem);
		PhysicsEngine2DLocator::Provide(&physicsEngine2D);
		SceneManagerLocator::Provide(&sceneManager);
		AssetManagerLocator::Provide(&assetManger);
		AudioSystemLocator::Provide(&audioSystem);
		DebugSystemLocator::Provide(&debugSystem);

		assetManger.LoadEngineAssets();

		// Depends on engine assets being loaded in
		debugSystem.Init();

		// Initialize application
		application->Init();
	}

	void NightOwlEngine::Update()
	{
		while (!WindowApi::GetWindow()->ShouldWindowClose())
		{
			Time::Update();

			application->Update();

			if (sceneManager.ShouldLoadNextScene())
			{
				sceneManager.LoadNextScene();

				Time::Reset();
			}

			while (Time::ShouldExecuteFixedUpdate())
			{
				Time::UpdateFixedTime();
			
				owlBehaviorManager.FixedUpdate();
			
				physicsEngine2D.Update();
			
				sceneManager.Update();
			
				audioSystem.Update();
			
				animatorSystem.FixedUpdate();
			}

			if (Time::ShouldRenderFrame())
			{
				RenderApi::GetContext()->ClearColor();
				RenderApi::GetContext()->ClearBuffer();

				owlBehaviorManager.Update();

				animatorSystem.Update();

				sceneManager.Update();

				meshRendererSystem.Update();

				debugSystem.Update();

				Input::Update();

				WindowApi::GetWindow()->Update();
			}

			sceneManager.DestroyMarkedGameObjects();
		}
	}

	void NightOwlEngine::Shutdown()
	{
		application->Shutdown();

		sceneManager.Shutdown();

		audioSystem.Shutdown();

		// clears engine and scene assets
		assetManger.ClearAll();

		WindowApi::GetWindow()->Shutdown();

		#ifdef DEBUG
		Utility::LoggerManager::Shutdown();
		#endif
	}
}
