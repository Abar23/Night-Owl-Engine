#include "NightOwlPch.h"

#include "OBBBoxCollider2D.h"

#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	OBBBoxCollider2D::OBBBoxCollider2D(const Vec2F& offsetFromCenterOfObject,
		const Vec2F& halfWidthsOfBoundingBox, const QuatF& orientationOffsetFromObject)
			:	offsetFromCenterOfObject(offsetFromCenterOfObject),
				halfWidths(halfWidthsOfBoundingBox),
				orientationOffsetFromObject(orientationOffsetFromObject)
	{ }

	Vec2F OBBBoxCollider2D::GetCenterOfBoundingBox() const
	{
		return offsetFromCenterOfObject + rigidBody2D->GetGameObject().GetTransform()->GetPosition().xy;
	}

	void OBBBoxCollider2D::SetCenterOfBoundingBox(const Vec2F& offsetFromCenterOfObject)
	{
		this->offsetFromCenterOfObject = offsetFromCenterOfObject;
	}

	Vec2F OBBBoxCollider2D::GetHalfWidths() const
	{
		return halfWidths;
	}

	void OBBBoxCollider2D::SetHalfWidths(const Vec2F& halfWidths)
	{
		this->halfWidths = halfWidths;
	}

	QuatF OBBBoxCollider2D::GetOrientation() const
	{
		return orientationOffsetFromObject * rigidBody2D->GetGameObject().GetTransform()->GetRotation();
	}

	void OBBBoxCollider2D::SetOrientation(const QuatF& orientationOffsetFromObject)
	{
		this->orientationOffsetFromObject = orientationOffsetFromObject;
	}

	START_REFLECTION(OBBBoxCollider2D)
	//CLASS_MEMBER_REFLECTION(offsetFromCenterOfObject)
	//CLASS_MEMBER_REFLECTION(halfWidths)
	//CLASS_MEMBER_REFLECTION(orientationOffsetFromObject)
	END_REFLECTION(OBBBoxCollider2D)
}
