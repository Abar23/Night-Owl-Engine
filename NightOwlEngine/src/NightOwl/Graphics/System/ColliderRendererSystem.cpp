#include "NightOwlPch.h"

#include "ColliderRendererSystem.h"

#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"

namespace NightOwl
{
	void ColliderRendererSystem::Update()
	{
		for (auto& collider : colliders)
		{
			RenderApi::GetContext()->EnableWireFrame(true);

			if (collider->GetRigidBody()->GetGameObject().IsActive())
			{
				collider->Draw();

				//draw velocity and acceleration lines.
			}

			RenderApi::GetContext()->EnableWireFrame(false);
		}
	}

	void ColliderRendererSystem::AddCollider(const Collider2D* collider)
	{
		colliders.push_back(collider);
	}

	void ColliderRendererSystem::RemoveCollider(const Collider2D* collider)
	{
		int Collider2DIndex = 0;
		for (const auto renderer : colliders)
		{
			if (renderer == collider)
			{
				break;
			}
			Collider2DIndex++;
		}

		if (Collider2DIndex != colliders.size())
		{
			colliders.erase(colliders.begin() + Collider2DIndex);
		}
	}	
}

