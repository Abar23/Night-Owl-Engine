#include "NightOwlPch.h"

#include "ColliderRendererSystem.h"

#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"

#include "NightOwl/Core/Locator/DebugToolsLocator.h"
#include "NightOwl/Core/Profiler/StopWatch.h"

namespace NightOwl::Graphics
{
	void ColliderRendererSystem::Update()
	{
		START_STOPWATCH("Collider System Update");
		for (auto& collider : Colliders)
		{
			Graphics::RenderApi::GetContext()->EnableWireFrame(true);

			if (collider->GetRigidBody()->GetGameObject().IsActive())
			{
				collider->Draw();

				//draw velocity and acceleration lines.
			}

			Graphics::RenderApi::GetContext()->EnableWireFrame(false);
		}
		STOP_STOPWATCH;
	}

	void ColliderRendererSystem::AddCollider(const Core::WeakPointer<Physics::Collider2D>& collider)
	{
		Colliders.push_back(collider);
	}

	void ColliderRendererSystem::RemoveCollider(const Core::WeakPointer<Physics::Collider2D>& collider)
	{
		int Collider2DIndex = 0;
		for (const auto& renderer : Colliders)
		{
			if (renderer == collider)
			{
				break;
			}
			Collider2DIndex++;
		}

		if (Collider2DIndex != Colliders.size())
		{
			Colliders.erase(Colliders.begin() + Collider2DIndex);
		}
	}	
}

