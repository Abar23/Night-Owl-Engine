#include "NightOwlPch.h"

#include "NightOwlEngine.h"
#include "NightOwlEngineConstants.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AnimatorSystemLocator.h"
#include "NightOwl/Core/Locator/TileSystemLocator.h"
#include "NightOwl/Core/Locator/ArchetypeSystemLocator.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Core/Locator/AudioSystemLocator.h"
#include "NightOwl/Core/Locator/ColliderRendererSystemLocator.h"
#include "NightOwl/Core/Locator/DebugToolsLocator.h"
#include "NightOwl/Core/Locator/MeshRenderSystemLocator.h"
#include "NightOwl/Core/Locator/OwlBehaviorManagerLocator.h"
#include "NightOwl/Core/Locator/PhysicsEngine2DLocator.h"
#include "NightOwl/Core/Locator/SceneManagerLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Input/Input.h"
#include "NightOwl/Window/WindowApi.h"

namespace NightOwl::Core
{
	NightOwlEngine::NightOwlEngine()
		:	application(nullptr)
	{ }

	void NightOwlEngine::Init()
	{
		ENGINE_ASSERT(application != nullptr, "Night Owl Engine application can't be null");

		// Standup necessary systems
		Window::WindowApi::CreateWindow("Dodge Brawl", 900, 1600);

		#ifdef DEBUG
		Utility::LoggerManager::Init();
		#endif

		Input::Input::Init();

		Time::Init();

		Time::SetFixedTime(FIXED_UPDATE_TIME_STEP);

		colliderRendererSystem = std::make_shared<Graphics::ColliderRendererSystem>();
		owlBehaviorManager = std::make_shared<Behavior::OwlBehaviorManager>();
		meshRendererSystem = std::make_shared<Graphics::MeshRendererSystem>();
		archetypeSystem = std::make_shared<Archetype::ArchetypeSystem>();
		animatorSystem = std::make_shared<Animation::AnimatorSystem>();
		tileSystem = std::make_shared <Tile::TileSystem>();
		physicsEngine2D = std::make_shared<Physics::PhysicsEngine2D>();
		audioSystem = std::make_shared<Audio::AudioSystem>();
		sceneManager = std::make_shared<SceneManager>();
		assetManger = std::make_shared<AssetManager>();

		ColliderRendererSystemLocator::Provide(colliderRendererSystem.get());
		OwlBehaviorManagerLocator::Provide(owlBehaviorManager.get());
		MeshRendererSystemLocator::Provide(meshRendererSystem.get());
		PhysicsEngine2DLocator::Provide(physicsEngine2D.get());
		ArchetypeSystemLocator::Provide(archetypeSystem.get());
		AnimatorSystemLocator::Provide(animatorSystem.get());
		TileSystemLocator::Provide(tileSystem.get());
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
		START_STOPWATCH("Game Loop Update");
		while (!Window::WindowApi::GetWindow()->ShouldWindowClose())
		{
			Time::Update();

			application->Update();

			if (sceneManager->ShouldLoadNextScene())
			{
				sceneManager->LoadNextScene();

				tileSystem->GenerateTiles();

				Time::Reset();
			}

			while (Time::ShouldExecuteFixedUpdate())
			{
				Time::UpdateFixedTime();

				owlBehaviorManager->FixedUpdate();

				physicsEngine2D->Update();

				sceneManager->Update();

				audioSystem->Update();

				animatorSystem->FixedUpdate();
			}

			if (Time::ShouldRenderFrame())
			{
				Graphics::RenderApi::GetContext()->ClearColor();
				Graphics::RenderApi::GetContext()->ClearBuffer();

				owlBehaviorManager->Update();

				#ifdef DEBUG
				colliderRendererSystem->Update();

				DebugToolsLocator::GetDebugTools()->RunDebugTools();
				#endif

				sceneManager->Update();

				animatorSystem->Update();

				meshRendererSystem->Update();

				Input::Input::Update();

				Window::WindowApi::GetWindow()->Update();

				STOP_AND_START_STOPWATCH;
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

		Window::WindowApi::GetWindow()->Shutdown();

		#ifdef DEBUG
		Utility::LoggerManager::Shutdown();
		#endif
	}
}
