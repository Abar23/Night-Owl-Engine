#pragma once

namespace NightOwl
{
	class AnimatorSystem;

	class AudioSystem;

	class AssetManager;

	class ClothSimSystem;

	class DebugSystem;

	class LightSystem;

	class MeshRendererSystem;

	class OwlBehaviorManager;

	class PhysicsEngine2D;

	class SceneManager;

	template <typename SystemType>
    class Locator
    {
    public:
        static SystemType* Get()
        {
            return systemInstance;
        }

        static void Provide(SystemType* systemInstance)
        {
            Locator::systemInstance = systemInstance;
        }

    private:
        inline static SystemType* systemInstance{ nullptr };
    };

    using AnimatorSystemLocator = Locator<AnimatorSystem>;

	using AudioSystemLocator = Locator<AudioSystem>;

	using AssetManagerLocator = Locator<AssetManager>;

	using ClothSimSystemLocator = Locator<ClothSimSystem>;

	using DebugSystemLocator = Locator<DebugSystem>;

	using LightSystemLocator = Locator<LightSystem>;

	using MeshRendererSystemLocator = Locator<MeshRendererSystem>;

	using OwlBehaviorManagerLocator = Locator<OwlBehaviorManager>;

	using PhysicsEngine2DLocator = Locator<PhysicsEngine2D>;

	using SceneManagerLocator = Locator<SceneManager>;
}
