#include "Transform.h"
#include "NightOwl/Core/Utitlity/Assert.h"

namespace NightOwl::Component
{
	Transform::Transform(GameObject::GameObject& gameObject)
		: Component(&gameObject, ComponentType::Transform),
		  localModelMatrix(Math::Mat4F::Identity()),
		  localScale(1.0f),
		  worldMatrix(Math::Mat4F::Identity()),
		  worldScale(1.0f),
		  root(this),
		  parent(nullptr),
		  isLocalDirty(false),
		  isWorldDirty(false)
	{

	}
	void Transform::Scale(float scaleX, float scaleY, float scaleZ, Space space)
	{
		const Math::Vec3F scale(scaleX, scaleY, scaleZ);

		(space == Space::Local) ? localScale += scale : worldScale += scale;

		(space == Space::Local) ? SetLocalDirtyFlag() : SetWorldDirtyFlag();
	}

	void Transform::Scale(const Math::Vec3F& scale, Space space)
	{
		(space == Space::Local) ? localScale += scale : worldScale += worldScale;

		(space == Space::Local) ? SetLocalDirtyFlag() : SetWorldDirtyFlag();
	}

	const Math::Vec3F& Transform::GetLocalScale()
	{
		return localScale;
	}

	void Transform::SetLocalScale(float scaleX, float scaleY, float scaleZ)
	{
		localScale = Math::Vec3F(scaleX, scaleY, scaleZ);
		SetLocalDirtyFlag();
	}

	void Transform::SetLocalScale(const Math::Vec3F& scale)
	{
		localScale = scale;
		SetLocalDirtyFlag();
	}

	const Math::Vec3F& Transform::GetLocalEulerAngles()
	{
		return localEulerAngles;
	}

	void Transform::SetLocalEulerAngles(float angleX, float angleY, float angleZ)
	{
		localEulerAngles = Math::Vec3F(angleX, angleY, angleZ);
		SetLocalDirtyFlag();
	}

	void Transform::SetLocalEulerAngles(const Math::Vec3F& eulers)
	{
		localEulerAngles = eulers;
		SetLocalDirtyFlag();
	}

	const Math::Vec3F& Transform::GetLocalPosition()
	{
		return localPosition;
	}

	void Transform::SetLocalPosition(float positionX, float positionY, float positionZ)
	{
		localPosition = Math::Vec3F(positionX, positionY, positionZ);
		SetLocalDirtyFlag();
	}

	void Transform::SetLocalPosition(const Math::Vec3F& position)
	{
		localPosition = position;
		SetLocalDirtyFlag();
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ, Space space)
	{
		const Math::Vec3F eulers(angleX, angleY, angleZ);

		(space == Space::Local) ? localEulerAngles += eulers : worldEulerAngles += eulers;

		(space == Space::Local) ? SetLocalDirtyFlag() : SetWorldDirtyFlag();
	}

	void Transform::Rotate(const Math::Vec3F& eulers, Space space)
	{
		(space == Space::Local) ? localEulerAngles += eulers : worldEulerAngles += eulers;

		(space == Space::Local) ? SetLocalDirtyFlag() : SetWorldDirtyFlag();
	}

	void Transform::Translate(float positionX, float positionY, float positionZ, Space space)
	{
		const Math::Vec3F translation(positionX, positionY, positionZ);

		(space == Space::Local) ? localPosition += translation : worldPosition += translation;

		(space == Space::Local) ? SetLocalDirtyFlag() : SetWorldDirtyFlag();
	}

	void Transform::Translate(const Math::Vec3F& translation, Space space)
	{
		(space == Space::Local) ? localPosition += translation : worldPosition += translation;

		(space == Space::Local) ? SetLocalDirtyFlag() : SetWorldDirtyFlag();
	}

	int Transform::GetNumberOfChildren()
	{
		return children.size();
	}

	const Transform& Transform::GetParent()
	{
		return *parent;
	}

	void Transform::SetParent(Transform* transform)
	{
		parent = transform;
	}

	void Transform::SetChild(Transform* transform)
	{
		children.push_back(transform);
	}

	const Math::Mat4F& Transform::GetLocalModelMatrix()
	{
		if (isLocalDirty)
		{
			const Math::Mat4F translationMatrix = Math::Mat4F::MakeTranslation(localPosition);

			const Math::Mat4F rotationMatrix = Math::Mat4F::MakeRotationX(localEulerAngles.x) *
											   Math::Mat4F::MakeRotationY(localEulerAngles.y) *
											   Math::Mat4F::MakeRotationZ(localEulerAngles.z);


			const Math::Mat4F scaleMatrix = Math::Mat4F::MakeScale(localScale);

			localModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

			isLocalDirty = false;
		}

		return localModelMatrix;
	}

	const Math::Mat4F& Transform::GetWorldMatrix()
	{
		if (isWorldDirty)
		{
			const Math::Mat4F translationMatrix = Math::Mat4F::MakeTranslation(worldPosition);

			const Math::Mat4F rotationMatrix = Math::Mat4F::MakeRotationX(worldEulerAngles.x) *
											   Math::Mat4F::MakeRotationY(worldEulerAngles.y) *
											   Math::Mat4F::MakeRotationZ(worldEulerAngles.z);


			const Math::Mat4F scaleMatrix = Math::Mat4F::MakeScale(worldScale);

			worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

			isWorldDirty = false;
		}

		return worldMatrix;
	}

	Transform* Transform::GetChildAtIndex(int index) const
	{
		ENGINE_ASSERT(index < children.size(), std::format("Transform does not have child at index {0}", index));

		return children[index];
	}

	void Transform::SetLocalDirtyFlag()
	{
		isLocalDirty = true;
	}

	void Transform::SetWorldDirtyFlag()
	{
		isWorldDirty = true;
	}

	Math::Vec3F Transform::GetWorldScale() const
	{
		return worldScale;
	}

	void Transform::SetWorldScale(const Math::Vec3F& worldScale)
	{
		this->worldScale = worldScale;
	}

	Math::Vec3F Transform::GetWorldEulerAngles() const
	{
		return worldEulerAngles;
	}

	void Transform::SetWorldEulerAngles(const Math::Vec3F& worldEulerAngles)
	{
		this->worldEulerAngles = worldEulerAngles;
	}

	Math::Vec3F Transform::GetWorldPosition() const
	{
		return worldPosition;
	}

	void Transform::SetWorldPosition(const Math::Vec3F& worldPosition)
	{
		this->worldPosition = worldPosition;
	}
}
