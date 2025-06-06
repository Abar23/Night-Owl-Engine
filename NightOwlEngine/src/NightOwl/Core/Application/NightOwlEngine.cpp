#include "NightOwlPch.h"

#include "NightOwlEngine.h"
#include "IApplication.h"
#include "NightOwlEngineConstants.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/Graphics/Graphics.h"
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
		WindowApi::CreateWindow("Animation Project 4", 900, 1600);

		Graphics::Initialize();

		lightSystem.Initialize();

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
		ClothSimSystemLocator::Provide(&clothSimSystem);
		LightSystemLocator::Provide(&lightSystem);

		assetManger.LoadEngineAssets();

		Graphics::SetupRenderPipelineAssets();

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
			
				clothSimSystem.FixedUpdate();
			
				animatorSystem.FixedUpdate();
			
				sceneManager.Update();
			
				audioSystem.Update();
			}

			if (Time::ShouldRenderFrame())
			{
				Graphics::GetContext()->ClearColor();
				Graphics::GetContext()->ClearBuffer();

				owlBehaviorManager.Update();

				animatorSystem.Update();

				owlBehaviorManager.LateUpdate();

				sceneManager.Update();

				clothSimSystem.Update();

				Graphics::Render();

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

		lightSystem.ShutDown();

		debugSystem.Shutdown();

		Graphics::Shutdown();

		WindowApi::GetWindow()->Shutdown();

		#ifdef DEBUG
		Utility::LoggerManager::Shutdown();
		#endif
	}
}
