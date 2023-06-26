#pragma once

#include "IApplication.h"
#include "SceneManager.h"
#include "NightOwl/Audio/AudioSystem.h"
#include "NightOwl/Behavior/OwlBehaviorManager.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Graphics/System/MeshRendererSystem.h"
#include "NightOwl/Physics/2D/PhysicsEngine2D.h"

namespace NightOwl
{
	class NightOwlEngine
	{
	public:
		NightOwlEngine();

		void Init();

		void Update();

		void Shutdown();

		void SetApplication(std::shared_ptr<IApplication> application)
		{
			this->application = application;
		}

	private:
		std::shared_ptr<IApplication> application;

		// All non-singleton engine subsystems
		//std::shared_ptr<ColliderRendererSystem> colliderRendererSystem;

		std::shared_ptr<OwlBehaviorManager> owlBehaviorManager;

		std::shared_ptr<MeshRendererSystem> meshRendererSystem;

		//std::shared_ptr<ArchetypeSystem> archetypeSystem;

		//std::shared_ptr<Animation::AnimatorSystem> animatorSystem;

		std::shared_ptr<PhysicsEngine2D> physicsEngine2D;

		std::shared_ptr<AudioSystem> audioSystem;

		std::shared_ptr<SceneManager> sceneManager;

		std::shared_ptr<AssetManager> assetManger;
	};
}
