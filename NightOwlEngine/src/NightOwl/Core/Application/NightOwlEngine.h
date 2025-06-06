#pragma once

#include "SceneManager.h"
#include "NightOwl/Animation/3D/System/AnimatorSystem.h"
#include "NightOwl/Audio/AudioSystem.h"
#include "NightOwl/Behavior/OwlBehaviorManager.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"
#include "NightOwl/Graphics/System/LightSystem.h"
#include "NightOwl/Graphics/System/MeshRendererSystem.h"
#include "NightOwl/Physics/ClothSimSystem.h"
#include "NightOwl/Physics/2D/PhysicsEngine2D.h"

namespace NightOwl
{
	class IApplication;

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

		OwlBehaviorManager owlBehaviorManager;

		MeshRendererSystem meshRendererSystem;

		AnimatorSystem animatorSystem;

		ClothSimSystem clothSimSystem;

		//std::shared_ptr<ArchetypeSystem> archetypeSystem;

		PhysicsEngine2D physicsEngine2D;

		AudioSystem audioSystem;

		SceneManager sceneManager;

		AssetManager assetManger;

		DebugSystem debugSystem;

		LightSystem lightSystem;
	};
}
