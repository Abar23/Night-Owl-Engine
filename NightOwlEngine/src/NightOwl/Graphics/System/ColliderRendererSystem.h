#pragma once

#include "NightOwl/Physics/2D/Collider/Collider2D.h"
#include <vector>
#include <set>

namespace NightOwl
{
	class ColliderRendererSystem
	{
	public:
		void Update();

		void AddCollider(const Collider2D* collider);

		void RemoveCollider(const Collider2D* collider);

	private:
		std::vector<Collider2D*> colliders;
	};
}

