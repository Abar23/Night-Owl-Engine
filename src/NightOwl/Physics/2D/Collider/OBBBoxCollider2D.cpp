#include "OBBBoxCollider2D.h"

#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl::Physics
{
	OBBBoxCollider2D::OBBBoxCollider2D(const Math::Vec2F& offsetFromCenterOfObject,
		const Math::Vec2F& halfWidthsOfBoundingBox, const Math::QuatF& orientationOffsetFromObject)
			:	offsetFromCenterOfObject(offsetFromCenterOfObject),
				halfWidths(halfWidthsOfBoundingBox),
				orientationOffsetFromObject(orientationOffsetFromObject)
	{ }

	Math::Vec2F OBBBoxCollider2D::GetCenterOfBoundingBox() const
	{
		return offsetFromCenterOfObject + rigidBody2D->GetGameObject().GetTransform()->GetPosition().xy;
	}

	void OBBBoxCollider2D::SetCenterOfBoundingBox(const Math::Vec2F& offsetFromCenterOfObject)
	{
		this->offsetFromCenterOfObject = offsetFromCenterOfObject;
	}

	Math::Vec2F OBBBoxCollider2D::GetHalfWidths() const
	{
		return halfWidths;
	}

	void OBBBoxCollider2D::SetHalfWidths(const Math::Vec2F& halfWidths)
	{
		this->halfWidths = halfWidths;
	}

	Math::QuatF OBBBoxCollider2D::GetOrientation() const
	{
		return orientationOffsetFromObject * rigidBody2D->GetGameObject().GetTransform()->GetRotation();
	}

	void OBBBoxCollider2D::SetOrientation(const Math::QuatF& orientationOffsetFromObject)
	{
		this->orientationOffsetFromObject = orientationOffsetFromObject;
	}
}
