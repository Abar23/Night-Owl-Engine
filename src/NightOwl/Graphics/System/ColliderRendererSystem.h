#pragma once

#include "NightOwl/Physics/2D/Collider/Collider2D.h"
#include "NightOwl/Core/Memory/WeakPointer.h"
#include <vector>
#include <set>

namespace NightOwl::Graphics
{
	class ColliderRendererSystem
	{
	public:
		void Update();

		void AddCollider(const Core::WeakPointer<Physics::Collider2D>& collider);

		void RemoveCollider(const Core::WeakPointer<Physics::Collider2D>& collider);

	private:
		std::vector<Core::WeakPointer<Physics::Collider2D>> Colliders;
	};
}

