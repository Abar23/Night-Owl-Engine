#include "NightOwlPch.h"

#include "CircleCollider2D.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	CircleCollider2D::CircleCollider2D(const Vec2F& offsetFromCenterOfObject, float radius)
		:	offsetFromCenterOfObject(offsetFromCenterOfObject),
			radius(radius)
	{ }

	void CircleCollider2D::SetRadius(float radius)
	{
		this->radius = radius;
	}

	float CircleCollider2D::GetRadius() const
	{
		return radius;
	}

	Vec2F CircleCollider2D::GetCenterOfCircle() const
	{
		return offsetFromCenterOfObject + rigidBody2D->GetGameObject().GetTransform()->GetPosition().xy;
	}

	START_REFLECTION(CircleCollider2D)
	//CLASS_MEMBER_REFLECTION(offsetFromCenterOfObject)
	//CLASS_MEMBER_REFLECTION(radius)
	END_REFLECTION(CircleCollider2D)
}
