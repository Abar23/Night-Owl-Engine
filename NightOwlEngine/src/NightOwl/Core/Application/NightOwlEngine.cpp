#include "NightOwlPch.h"

#include "NightOwlEngine.h"
#include "NightOwlEngineConstants.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Core/Locator/AudioSystemLocator.h"
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
		WindowApi::CreateWindow("Dodge Brawl", 900, 1600);

		#ifdef DEBUG
		Utility::LoggerManager::Init();
		#endif

		Input::Init();

		Time::Init();

		Time::SetFixedTime(FIXED_UPDATE_TIME_STEP);

		owlBehaviorManager = std::make_shared<OwlBehaviorManager>();
		meshRendererSystem = std::make_shared<MeshRendererSystem>();
		physicsEngine2D = std::make_shared<PhysicsEngine2D>();
		audioSystem = std::make_shared<AudioSystem>();
		sceneManager = std::make_shared<SceneManager>();
		assetManger = std::make_shared<AssetManager>();

		OwlBehaviorManagerLocator::Provide(owlBehaviorManager.get());
		MeshRendererSystemLocator::Provide(meshRendererSystem.get());
		PhysicsEngine2DLocator::Provide(physicsEngine2D.get());
		SceneManagerLocator::Provide(sceneManager.get());
		AssetManagerLocator::Provide(assetManger.get());
		AudioSystemLocator::Provide(audioSystem.get());
		
		#ifdef DEBUG
		debugTools = std::make_shared<DebugTools::DebugTools>();
		DebugToolsLocator::Provide(debugTools.get());
		#endif

		application->Init();
	}

	void NightOwlEngine::Update()
	{
		while (!WindowApi::GetWindow()->ShouldWindowClose())
		{
			Time::Update();

			application->Update();

			if (sceneManager->ShouldLoadNextScene())
			{
				sceneManager->LoadNextScene();

				Time::Reset();
			}

			while (Time::ShouldExecuteFixedUpdate())
			{
				Time::UpdateFixedTime();

				owlBehaviorManager->FixedUpdate();

				physicsEngine2D->Update();

				sceneManager->Update();

				audioSystem->Update();
			}

			if (Time::ShouldRenderFrame())
			{
				RenderApi::GetContext()->ClearColor();
				RenderApi::GetContext()->ClearBuffer();

				owlBehaviorManager->Update();

				#ifdef DEBUG
				colliderRendererSystem->Update();

				DebugToolsLocator::GetDebugTools()->RunDebugTools();
				#endif

				sceneManager->Update();

				meshRendererSystem->Update();

				Input::Update();

				WindowApi::GetWindow()->Update();
			}

			sceneManager->DestroyMarkedGameObjects();
		}
	}

	void NightOwlEngine::Shutdown()
	{
		application->Shutdown();

		sceneManager->Shutdown();

		audioSystem->Shutdown();

		#ifdef DEBUG
		debugTools->Shutdown();
		#endif

		WindowApi::GetWindow()->Shutdown();

		#ifdef DEBUG
		Utility::LoggerManager::Shutdown();
		#endif
	}
}
