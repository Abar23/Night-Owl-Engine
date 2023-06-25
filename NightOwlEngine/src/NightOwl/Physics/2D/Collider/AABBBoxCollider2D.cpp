#include "NightOwlPch.h"

#include "AABBBoxCollider2D.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	AABBBoxCollider2D::AABBBoxCollider2D(const Vec2F& offsetFromCenterOfObject, const Vec2F& halfWidthsOfBoundingBox)
		:	offsetFromCenterOfObject(offsetFromCenterOfObject),
			halfWidths(halfWidthsOfBoundingBox)
	{ }

	Vec2F AABBBoxCollider2D::GetCenterOfBoundingBox() const
	{
		return offsetFromCenterOfObject + rigidBody2D->GetGameObject().GetTransform()->GetPosition().xy;
	}

	void AABBBoxCollider2D::SetCenterOfBoundingBox(const Vec2F& offsetFromCenterOfObject)
	{
		this->offsetFromCenterOfObject = offsetFromCenterOfObject;
	}

	Vec2F AABBBoxCollider2D::GetHalfWidths() const
	{
		return halfWidths;
	}

	void AABBBoxCollider2D::SetHalfWidths(const Vec2F& halfWidths)
	{
		this->halfWidths = halfWidths;
	}

	START_REFLECTION(AABBBoxCollider2D)
	//CLASS_MEMBER_REFLECTION(offsetFromCenterOfObject)
	//CLASS_MEMBER_REFLECTION(halfWidths)
	END_REFLECTION(AABBBoxCollider2D)
}
