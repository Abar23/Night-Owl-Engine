#pragma once

#include <vector>

namespace NightOwl
{
	class Collider2D;

	class ColliderRendererSystem
	{
	public:
		void Update();

		void AddCollider(Collider2D* collider);

		void RemoveCollider(const Collider2D* collider);

	private:
		std::vector<Collider2D*> colliders;
	};
}

