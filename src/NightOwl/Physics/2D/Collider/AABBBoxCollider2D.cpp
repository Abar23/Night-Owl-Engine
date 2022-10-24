#include "AABBBoxCollider2D.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl::Physics
{
	AABBBoxCollider2D::AABBBoxCollider2D(const Math::Vec2F& offsetFromCenterOfObject, const Math::Vec2F& halfWidthsOfBoundingBox)
		:	offsetFromCenterOfObject(offsetFromCenterOfObject),
			halfWidths(halfWidthsOfBoundingBox)
	{}

	Math::Vec2F AABBBoxCollider2D::GetCenterOfBoundingBox() const
	{
		return offsetFromCenterOfObject + rigidBody2D->GetGameObject().GetTransform()->GetPosition().xy;
	}

	void AABBBoxCollider2D::SetCenterOfBoundingBox(const Math::Vec2F& offsetFromCenterOfObject)
	{
		this->offsetFromCenterOfObject = offsetFromCenterOfObject;
	}

	Math::Vec2F AABBBoxCollider2D::GetHalfWidths() const
	{
		return halfWidths;
	}

	void AABBBoxCollider2D::SetHalfWidths(const Math::Vec2F& halfWidths)
	{
		this->halfWidths = halfWidths;
	}
}
