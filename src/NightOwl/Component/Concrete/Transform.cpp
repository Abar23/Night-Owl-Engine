#include "Transform.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/GameObject/GameObject.h"

// NEED TO CLEAN UP LOGIC, KINDA NASTY!!!!!!!!!!
namespace NightOwl::Component
{
	Transform::Transform()
		: Component(ComponentType::Transform),
		  localModelMatrix(1.0f),
	      worldMatrix(1.0f),
		  parentLocalMatrix(1.0f),
		  inverseOfOriginalParentLocalModelMatrix(1.0f),
		  localScale(1.0f),
		  worldScaleOffset(1.0f),
		  root(this),
		  parent(nullptr),
		  isLocalDirty(false),
		  isWorldDirty(false)
	{
	}

	void Transform::Scale(float scaleX, float scaleY, float scaleZ, Space space)
	{
		const Math::Vec3F scale(scaleX, scaleY, scaleZ);

		Scale(scale, space);
	}

	void Transform::Scale(const Math::Vec3F& scale, Space space)
	{
		if (parent == nullptr || space == Space::Local)
		{
			localScale += scale;

			SetLocalDirtyFlag();
		}
		else if (space == Space::World)
		{
			worldScaleOffset += scale;

			SetWorldDirtyFlag();
		}
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ, Space space)
	{
		const Math::Vec3F eulers(angleX, angleY, angleZ);

		Rotate(eulers, space);
	}

	void Transform::Rotate(const Math::Vec3F& eulers, Space space)
	{
		if (parent == nullptr || space == Space::Local)
		{
			localRotation = Math::QuatF::MakeRotationFromEulers(eulers) * localRotation;

			localRotation.Renormalize();

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldRotationOffset = Math::QuatF::MakeRotationFromEulers(eulers) * worldRotationOffset;

			SetWorldDirtyFlag();
		}
	}

	void Transform::Translate(float positionX, float positionY, float positionZ, Space space)
	{
		const Math::Vec3F translation(positionX, positionY, positionZ);

		Translate(translation, space);
	}

	void Transform::Translate(const Math::Vec3F& translation, Space space)
	{
		if(parent == nullptr || space == Space::Local)
		{
			localPosition += translation;

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldPosition += translation;

			SetWorldDirtyFlag();
		}
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
		return localRotation.GetEulerAngles();
	}

	void Transform::SetLocalEulerAngles(float angleX, float angleY, float angleZ)
	{
		localRotation = Math::QuatF::MakeRotationFromEulers(Math::Vec3F(angleX, angleY, angleZ));

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalEulerAngles(const Math::Vec3F& eulers)
	{
		localRotation = Math::QuatF::MakeRotationFromEulers(eulers);

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

	int Transform::GetNumberOfChildren()
	{
		return children.size();
	}

	const Transform& Transform::GetParent()
	{
		return *parent;
	}

	void Transform::SetParent(Transform* parentTransform)
	{
		if(parentTransform != nullptr && parentTransform != parent)
		{
			parent = parentTransform;
			parent->SetChild(*this);
			inverseOfOriginalParentLocalModelMatrix = parent->GetWorldMatrix().GetInverse();
			for (const auto& childTransform : parentTransform->children)
			{
				childTransform->PropagateParentLocalTransform(parent->GetLocalModelMatrix());
			}
		}

		// What to do when parent exists and parenting to new object
	}

	Transform* Transform::RemoveParent()
	{
		Transform* parentBeingRemoved = nullptr;

		if(parent != nullptr)
		{
			parentBeingRemoved = parent;

			// Remove this object from parent children list and null out parent pointer
			int indexOfChildToRemove;
			for (indexOfChildToRemove = 0; indexOfChildToRemove < parent->children.size(); indexOfChildToRemove++)
			{
				if (this == parent->children[indexOfChildToRemove]) break;
			}
			parent->children.erase(parent->children.begin() + indexOfChildToRemove);
			parent = nullptr;

			// Since parent is being removed, save off the current rotation, scale, and position that as been given to the object
 			localPosition = worldMatrix.GetTranslation();
			localScale = worldMatrix.GetScale();
			localRotation.SetOrthogonalRotationMatrix(worldMatrix.GetRotationMatrix());
			localRotation.Normalize();
			isLocalDirty = true;

			// Reset world data since parent has been lost. In this case Local = World
			worldPosition = Math::Vec3F();
			worldScaleOffset = Math::Vec3F(1.0);
			worldRotationOffset = Math::QuatF();
			parentLocalMatrix = Math::Mat4F::Identity();
			inverseOfOriginalParentLocalModelMatrix = Math::Mat4F::Identity();
		}
		return parentBeingRemoved;
	}

	void Transform::SetChild(Transform& childTransform)
	{
		children.push_back(&childTransform);
	}

	const Math::Mat4F& Transform::GetLocalModelMatrix(bool overrideDirtyFlag)
	{
		if (isLocalDirty || overrideDirtyFlag)
		{
			const Math::Mat4F translationMatrix = Math::Mat4F::MakeTranslation(localPosition);

			const Math::Mat4F scaleMatrix = Math::Mat4F::MakeScale(localScale);

			localModelMatrix = translationMatrix * localRotation.GetRotationMatrix() * scaleMatrix;

			// CLean up tomorrow
			if(parent == nullptr)
			{
				worldMatrix = localModelMatrix;
			}

			isLocalDirty = false;
		}

		return localModelMatrix;
	}

	const Math::Mat4F& Transform::GetWorldMatrix()
	{
		if (isWorldDirty || isLocalDirty)
		{
			const Math::Mat4F translationMatrix = Math::Mat4F::MakeTranslation(worldPosition);

			const Math::Mat4F scaleMatrix = Math::Mat4F::MakeScale(worldScaleOffset);

			if(parent != nullptr)
			{
				const Math::Mat4F parentChildCombined = parentLocalMatrix * inverseOfOriginalParentLocalModelMatrix * GetLocalModelMatrix();

				const Math::Mat4F parentChildCombinedTranslation = Math::Mat4F::MakeTranslation(parentChildCombined.GetTranslation());

				worldMatrix = parentChildCombinedTranslation * translationMatrix * worldRotationOffset.GetRotationMatrix() * scaleMatrix * parentChildCombinedTranslation.GetInverse() * parentChildCombined;
			}
			else
			{
				// Clean up tomorrow!
				worldMatrix =  GetLocalModelMatrix() * translationMatrix * worldRotationOffset.GetRotationMatrix() * scaleMatrix;
			}

			isWorldDirty = false;
			isLocalDirty = false;
		}

		return worldMatrix;
	}

	Transform* Transform::GetChildAtIndex(int index) const
	{
		ENGINE_ASSERT(index < children.size(), "Transform does not have child at index {0}", index);

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

	void Transform::SetFlagBasedOnSpace(Space space)
	{
		(space == Space::Local) ? SetLocalDirtyFlag() : SetWorldDirtyFlag();
	}

	void Transform::PropagateParentLocalTransform(const Math::Mat4F& parentLocalTransform)
	{
		this->parentLocalMatrix = parentLocalTransform;

		for (const auto& childTransform : this->children)
		{
			childTransform->PropagateParentLocalTransform(parentLocalTransform * this->GetLocalModelMatrix(true));
		}

		SetWorldDirtyFlag();
	}

	Math::QuatF Transform::GetRotation() const
	{
		Math::QuatF currentWorldRotation;
		currentWorldRotation.SetNonOrthogonalRotationMatrix(worldMatrix.GetRotationMatrix());
		return currentWorldRotation.Normalize();
	}

	void Transform::SetRotation(const Math::QuatF& newRotation)
	{
		Math::QuatF currentWorldRotation;
		currentWorldRotation.SetOrthogonalRotationMatrix(worldMatrix.GetRotationMatrix());
		// Apply current world rotation offset to the inverse of the total rotations on the object, then apply desired rotation.
		// Need to multiply by the world rotation offset since it will have the inverse of all rotation in it once this function has run.
		this->worldRotationOffset = newRotation * currentWorldRotation.Inverse() * worldRotationOffset;
		this->worldRotationOffset.Normalize();
		isWorldDirty = true;
	}

	Math::Vec3F Transform::GetPosition() const
	{
		return worldMatrix.GetTranslation();
	}

	void Transform::SetPosition(const Math::Vec3F& worldPosition)
	{
		if(parent != nullptr)
		{
			// Add to the local position the world offset of the desired position coming in
			Math::Vec3F worldOffset = worldPosition - GetPosition();
			// Apply rotation to the offset so that it is aligned with the local position axes
			localPosition += GetWorldMatrix().GetRotationMatrix() * localModelMatrix.GetRotationMatrix().Inverted() * worldOffset;
		}
		else
		{
			localPosition = worldPosition;
		}
		isLocalDirty = true;
	}

	bool Transform::HasParent() const
	{
		return parent != nullptr;
	}

	bool Transform::HasChildren() const
	{
		return !children.empty();
	}
}
