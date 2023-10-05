#pragma once

#include "Collider2D.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include "NightOwl/Math/Vec2.h"

namespace NightOwl
{
	class RigidBody2D;

	class AABBBoxCollider2D : public Collider2D
	{
	public:
		AABBBoxCollider2D() = delete;

		AABBBoxCollider2D(const Vec2F& offsetFromCenterOfObject, const Vec2F& halfWidthsOfBoundingBox);

		Vec2F GetCenterOfBoundingBox() const;

		void SetCenterOfBoundingBox(const Vec2F& offsetFromCenterOfObject);

		Vec2F GetHalfWidths() const;

		void SetHalfWidths(const Vec2F& halfWidths);

		REFLECT()

	private:
		Vec2F offsetFromCenterOfObject;

		Vec2F halfWidths;
	};
}
