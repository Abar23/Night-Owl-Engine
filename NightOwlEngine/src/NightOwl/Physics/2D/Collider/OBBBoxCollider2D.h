#pragma once

#include "Collider2D.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include "NightOwl/Math/Quaternion.h"
#include "NightOwl/Math/Vec2.h"

namespace NightOwl
{
	class OBBBoxCollider2D : public Collider2D
	{
	public:
		OBBBoxCollider2D() = delete;

		OBBBoxCollider2D(const Vec2F& offsetFromCenterOfObject, const Vec2F& halfWidthsOfBoundingBox, const QuatF& orientationOffsetFromObject);

		Vec2F GetCenterOfBoundingBox() const;

		void SetCenterOfBoundingBox(const Vec2F& offsetFromCenterOfObject);

		Vec2F GetHalfWidths() const;

		void SetHalfWidths(const Vec2F& halfWidths);

		QuatF GetOrientation() const;

		void SetOrientation(const QuatF& orientationOffsetFromObject);

		REFLECT()

	private:
		Vec2F offsetFromCenterOfObject;

		Vec2F halfWidths;

		QuatF orientationOffsetFromObject;
	};
}
