#pragma once

#include "Collider2D.h"
#include "NightOwl/Math/Vec2.h"

namespace NightOwl::Physics
{
	class CircleCollider2D : public Collider2D
	{
	public:
		CircleCollider2D() = delete;

		CircleCollider2D(const Math::Vec2F& offsetFromCenterOfObject, float radius);

		void SetRadius(float radius);

		float GetRadius() const;

		Math::Vec2F GetCenterOfCircle() const;

	private:
		Math::Vec2F offsetFromCenterOfObject;

		float radius;
	};
}
