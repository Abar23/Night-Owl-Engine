#pragma once

#include "Collider2D.h"
#include "NightOwl/Math/Vec2.h"

namespace NightOwl::Physics
{
	class RigidBody2D;

	class AABBBoxCollider2D : public Collider2D
	{
	public:
		AABBBoxCollider2D() = delete;

		AABBBoxCollider2D(const Math::Vec2F& offsetFromCenterOfObject, const Math::Vec2F& halfWidthsOfBoundingBox);

		Math::Vec2F GetCenterOfBoundingBox() const;

		void SetCenterOfBoundingBox(const Math::Vec2F& offsetFromCenterOfObject);

		Math::Vec2F GetHalfWidths() const;

		void SetHalfWidths(const Math::Vec2F& halfWidths);

	private:
		Math::Vec2F offsetFromCenterOfObject;

		Math::Vec2F halfWidths;
	};
}
