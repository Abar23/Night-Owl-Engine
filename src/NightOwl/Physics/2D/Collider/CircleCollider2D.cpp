#include "CircleCollider2D.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl::Physics
{
	CircleCollider2D::CircleCollider2D(const Math::Vec2F& offsetFromCenterOfObject, float radius)
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

	Math::Vec2F CircleCollider2D::GetCenterOfCircle() const
	{
		return offsetFromCenterOfObject + rigidBody2D->GetGameObject().GetTransform()->GetPosition().xy;
	}
}
