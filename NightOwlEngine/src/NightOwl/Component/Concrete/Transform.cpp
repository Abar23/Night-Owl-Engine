#include <NightOwlPch.h>

#include "Transform.h"

#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/GameObject/GameObject.h"

// NEED TO CLEAN UP LOGIC, KINDA NASTY!!!!!!!!!!
namespace NightOwl
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
	{ }

	void Transform::Scale(float scaleX, float scaleY, float scaleZ, Space space)
	{
		const Vec3F scale(scaleX, scaleY, scaleZ);

		Scale(scale, space);
	}

	void Transform::Scale(const Vec3F& scale, Space space)
	{
		if (parent == nullptr || space == Space::Local)
		{
			localScale = scale;

			SetLocalDirtyFlag();
		}
		else if (space == Space::World)
		{
			worldScaleOffset = scale;

			SetWorldDirtyFlag();
		}
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ, Space space)
	{
		const Vec3F eulers(angleX, angleY, angleZ);

		Rotate(eulers, space);
	}

	void Transform::Rotate(const Vec3F& eulers, Space space)
	{
		if (parent == nullptr || space == Space::Local)
		{
			localRotation = QuatF::MakeRotationFromEulers(eulers) * localRotation;

			localRotation.Renormalize();

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldRotationOffset = QuatF::MakeRotationFromEulers(eulers) * worldRotationOffset;

			SetWorldDirtyFlag();
		}
	}

	void Transform::Translate(float positionX, float positionY, float positionZ, Space space)
	{
		const Vec3F translation(positionX, positionY, positionZ);

		Translate(translation, space);
	}

	void Transform::Translate(const Vec3F& translation, Space space)
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

	const Vec3F& Transform::GetLocalScale()
	{
		return localScale;
	}

	void Transform::SetLocalScale(float scaleX, float scaleY, float scaleZ)
	{
		localScale = Vec3F(scaleX, scaleY, scaleZ);

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalScale(const Vec3F& scale)
	{
		localScale = scale;

		SetLocalDirtyFlag();
	}

	const Vec3F Transform::GetLocalEulerAngles()
	{
		return localRotation.GetEulerAngles();
	}

	void Transform::SetLocalRotation(const QuatF& rotation)
	{
		localRotation = rotation;

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalEulerAngles(float angleX, float angleY, float angleZ)
	{
		localRotation = QuatF::MakeRotationFromEulers(Vec3F(angleX, angleY, angleZ));

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalEulerAngles(const Vec3F& eulers)
	{
		localRotation = QuatF::MakeRotationFromEulers(eulers);

		SetLocalDirtyFlag();
	}

	const Vec3F& Transform::GetLocalPosition()
	{
		return localPosition;
	}

	void Transform::SetLocalPosition(float positionX, float positionY, float positionZ)
	{
		localPosition = Vec3F(positionX, positionY, positionZ);

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalPosition(const Vec3F& position)
	{
		localPosition = position;

		SetLocalDirtyFlag();
	}

	int Transform::GetNumberOfChildren()
	{
		return children.size();
	}

	Transform& Transform::GetParent()
	{
		return *parent;
	}

	void Transform::SetParent(Transform* parentTransform)
	{
		if(parentTransform != nullptr && parentTransform != parent)
		{
			parent = parentTransform;
			parent->SetChild(this);
			inverseOfOriginalParentLocalModelMatrix = parent->GetWorldMatrix().GetInverse();
			for (const auto& childTransform : parentTransform->children)
			{
				childTransform->PropagateParentLocalTransform(parent->GetLocalModelMatrix());
			}

			if (gameObject->GetScene() != nullptr)
			{
				gameObject->GetScene()->SetDirtyFlag();
			}
		}
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
			worldPosition = Vec3F();
			worldScaleOffset = Vec3F(1.0);
			worldRotationOffset = QuatF();
			parentLocalMatrix = Mat4F::Identity();
			inverseOfOriginalParentLocalModelMatrix = Mat4F::Identity();

			gameObject->GetScene()->SetDirtyFlag();
		}

		return parentBeingRemoved;
	}

	void Transform::SetChild(Transform* childTransform)
	{
		children.push_back(childTransform);
	}

	bool Transform::IsChildOf(Transform* parent)
	{
		return this->parent == parent;
	}

	const Mat4F& Transform::GetLocalModelMatrix(bool overrideDirtyFlag)
	{
		if (isLocalDirty || overrideDirtyFlag)
		{
			const Mat4F translationMatrix = Mat4F::MakeTranslation(localPosition);

			const Mat4F scaleMatrix = Mat4F::MakeScale(localScale);

			localModelMatrix = translationMatrix * localRotation.GetRotationMatrix() * scaleMatrix;

			// CLean up tomorrow
			if(parent == nullptr)
			{
				worldMatrix = localModelMatrix;
			}

		}

		return localModelMatrix;
	}

	const Mat4F& Transform::GetWorldMatrix()
	{
		if (isWorldDirty || isLocalDirty)
		{
			const Mat4F translationMatrix = Mat4F::MakeTranslation(worldPosition);

			const Mat4F scaleMatrix = Mat4F::MakeScale(worldScaleOffset);

			if(parent != nullptr)
			{
				const Mat4F parentChildCombined = parentLocalMatrix * inverseOfOriginalParentLocalModelMatrix * GetLocalModelMatrix();

				const Mat4F parentChildCombinedTranslation = Mat4F::MakeTranslation(parentChildCombined.GetTranslation());

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

	void Transform::PropagateParentLocalTransform(const Mat4F& parentLocalTransform)
	{
		this->parentLocalMatrix = parentLocalTransform;

		for (const auto& childTransform : this->children)
		{
			childTransform->PropagateParentLocalTransform(parentLocalTransform * this->GetLocalModelMatrix(true));
		}

		SetWorldDirtyFlag();
	}

	void Transform::Remove()
	{

	}

	QuatF Transform::GetRotation()
	{
		QuatF currentWorldRotation;
		currentWorldRotation.SetNonOrthogonalRotationMatrix(GetWorldMatrix().GetRotationMatrix());
		return currentWorldRotation.Normalize();
	}

	void Transform::SetRotation(const QuatF& newRotation)
	{
		if(parent == nullptr)
		{
			this->worldRotationOffset = newRotation;
			this->worldRotationOffset.Normalize();
			isWorldDirty = true;
			return;
		}

		localRotation = newRotation.GetNormalize();
		isLocalDirty = true;
	}

	Vec3F Transform::GetPosition()
	{
		return GetWorldMatrix().GetTranslation();
	}

	void Transform::SetPosition(const Vec3F& worldPosition)
	{
		if(parent != nullptr)
		{
			// Add to the local position the world offset of the desired position coming in
			Vec3F worldOffset = worldPosition - GetPosition();
			// Apply rotation to the offset so that it is aligned with the local position axes
			localPosition += GetWorldMatrix().GetRotationMatrix() * localModelMatrix.GetRotationMatrix().GetInverse() * worldOffset;
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

	Vec3F Transform::GetForward()
	{
		return GetRotation().GetRotationMatrix().GetColumn(2);
	}

	Vec3F Transform::GetRight()
	{
		return GetRotation().GetRotationMatrix().GetColumn(0);
	}

	Vec3F Transform::GetUp()
	{
		return GetRotation().GetRotationMatrix().GetColumn(1);
	}

	void Transform::Clone(const Transform& tranformToClone, Scene* currentScene /* = nullptr */)
	{
		localModelMatrix = tranformToClone.localModelMatrix;
		worldMatrix = tranformToClone.worldMatrix;
		parentLocalMatrix = tranformToClone.parentLocalMatrix;
		inverseOfOriginalParentLocalModelMatrix = tranformToClone.inverseOfOriginalParentLocalModelMatrix;
		localScale = tranformToClone.localScale;
		localRotation = tranformToClone.localRotation;
		localPosition = tranformToClone.localPosition;
		worldScaleOffset = tranformToClone.worldScaleOffset;
		worldRotationOffset = tranformToClone.worldRotationOffset;
		worldPosition = tranformToClone.worldPosition;
		root = tranformToClone.root;
		parent = tranformToClone.parent;

		isLocalDirty = tranformToClone.isLocalDirty;
		isWorldDirty = tranformToClone.isWorldDirty;

		// TODO: Figure out if this is needed
		// if(parent != nullptr)
		// {
		// 	parent->SetChild(this);
		// }

		for (auto& transform : tranformToClone.children)
		{
			auto clonedChild = transform->GetGameObject().Clone(currentScene ? currentScene : nullptr);
			children.push_back(clonedChild->GetTransform());
		}
	}

	START_REFLECTION(Transform)
	CLASS_MEMBER_REFLECTION(localScale)
	CLASS_MEMBER_REFLECTION(localRotation)
	CLASS_MEMBER_REFLECTION(localPosition)
	CLASS_MEMBER_REFLECTION(worldScaleOffset)
	CLASS_MEMBER_REFLECTION(worldRotationOffset)
	CLASS_MEMBER_REFLECTION(worldPosition)
	END_REFLECTION(Transform)
}
