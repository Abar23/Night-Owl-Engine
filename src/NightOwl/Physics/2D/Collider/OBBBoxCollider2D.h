#pragma once

#include "Collider2D.h"
#include "NightOwl/Math/Quaternion.h"
#include "NightOwl/Math/Vec2.h"
#include "NightOwl/Core/Reflection/Reflection.h"

namespace NightOwl::Physics
{
	class OBBBoxCollider2D : public Collider2D
	{
	public:
		OBBBoxCollider2D() = delete;

		OBBBoxCollider2D(const Math::Vec2F& offsetFromCenterOfObject, const Math::Vec2F& halfWidthsOfBoundingBox, const Math::QuatF& orientationOffsetFromObject);

		Math::Vec2F GetCenterOfBoundingBox() const;

		void SetCenterOfBoundingBox(const Math::Vec2F& offsetFromCenterOfObject);

		Math::Vec2F GetHalfWidths() const;

		void SetHalfWidths(const Math::Vec2F& halfWidths);

		Math::QuatF GetOrientation() const;

		void SetOrientation(const Math::QuatF& orientationOffsetFromObject);

		REFLECT()

	private:
		Math::Vec2F offsetFromCenterOfObject;

		Math::Vec2F halfWidths;

		Math::QuatF orientationOffsetFromObject;
	};
}
