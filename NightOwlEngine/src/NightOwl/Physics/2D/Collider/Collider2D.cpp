#include "NightOwlPch.h"

#include "Collider2D.h"

namespace NightOwl
{
	Vec2F Collider2D::GetOffsetFromCenterOfObject() const
	{
		return offsetFromCenterOfObject;
	}

	void Collider2D::SetOffsetFromCenterOfObject(const Vec2F& offsetFromCenterOfObject)
	{
		this->offsetFromCenterOfObject = offsetFromCenterOfObject;
	}

	START_REFLECTION(Collider2D)
		CLASS_MEMBER_REFLECTION(rigidBody2D)
	END_REFLECTION(Collider2D)
}
