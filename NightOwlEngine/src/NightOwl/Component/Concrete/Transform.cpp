#include <NightOwlPch.h>

#include "Transform.h"

#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	Transform::Transform()
		: Component(ComponentType::Transform),
		  localModelMatrix(1.0f),
	      worldMatrix(1.0f),
		  parentLocalMatrix(1.0f),
		  inverseOfOriginalParentLocalModelMatrix(1.0f),
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
			localVecQuatMat.SetScale(scale);

			SetLocalDirtyFlag();
		}
		else if (space == Space::World)
		{
			worldOffsetVecQuatMat.SetScale(scale);

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
			localVecQuatMat.quaternion = QuatF::MakeRotationFromEulers(eulers) * localVecQuatMat.quaternion;

			localVecQuatMat.quaternion.Renormalize();

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldOffsetVecQuatMat.quaternion = QuatF::MakeRotationFromEulers(eulers) * worldOffsetVecQuatMat.quaternion;

			worldOffsetVecQuatMat.quaternion.Renormalize();

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
			localVecQuatMat.vector += translation;

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldOffsetVecQuatMat.vector += translation;

			SetWorldDirtyFlag();
		}
	}

	const Vec3F& Transform::GetLocalScale()
	{
		return localVecQuatMat.GetScale();
	}

	void Transform::SetLocalScale(float scaleX, float scaleY, float scaleZ)
	{
		localVecQuatMat.SetScale({ scaleX, scaleY, scaleZ });

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalScale(const Vec3F& scale)
	{
		localVecQuatMat.SetScale(scale);

		SetLocalDirtyFlag();
	}

	const Vec3F Transform::GetLocalEulerAngles()
	{
		return localVecQuatMat.GetRotation().GetEulerAngles();
	}

	void Transform::SetLocalRotation(const QuatF& rotation)
	{
		localVecQuatMat.quaternion = rotation;

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalEulerAngles(float angleX, float angleY, float angleZ)
	{
		localVecQuatMat.quaternion = QuatF::MakeRotationFromEulers(Vec3F(angleX, angleY, angleZ));

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalEulerAngles(const Vec3F& eulers)
	{
		localVecQuatMat.quaternion = QuatF::MakeRotationFromEulers(eulers);

		SetLocalDirtyFlag();
	}

	const Vec3F& Transform::GetLocalPosition()
	{
		return localVecQuatMat.GetTranslation();
	}

	void Transform::SetLocalPosition(float positionX, float positionY, float positionZ)
	{
		localVecQuatMat.SetTranslation({ positionX, positionY, positionZ } );

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalPosition(const Vec3F& position)
	{
		localVecQuatMat.vector = position;

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

	void Transform::SetParent(Transform* parentTransform, bool shouldSetParentInverse /* = true */)
	{
		if(parentTransform != nullptr && parentTransform != parent)
		{
			parent = parentTransform;
			parent->SetChild(this);

			if (shouldSetParentInverse)
			{
				inverseOfOriginalParentLocalModelMatrix = parent->worldVecQuatMat.GetInverse();
			}

			for (const auto& childTransform : parentTransform->children)
			{
				childTransform->PropagateParentLocalTransform(parent->localVecQuatMat);
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
			localVecQuatMat.vector = worldMatrix.GetTranslation();
			localVecQuatMat.SetScale(worldMatrix.GetScale());
			localVecQuatMat.quaternion.SetNonOrthogonalRotationMatrix(worldMatrix.GetRotationMatrix());
			localVecQuatMat.quaternion.Normalize();
			isLocalDirty = true;

			// Reset world data since parent has been lost. In this case Local = World
			worldOffsetVecQuatMat = VecQuatMatF();
			parentLocalMatrix = VecQuatMatF();
			inverseOfOriginalParentLocalModelMatrix = VecQuatMatF();

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
			localModelMatrix = localVecQuatMat.GetMatrix();

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
			if(parent != nullptr)
			{
				VecQuatMatF parentChildCombined = parentLocalMatrix * inverseOfOriginalParentLocalModelMatrix * localVecQuatMat;

				VecQuatMatF parentChildCombinedTranslation;
				parentChildCombinedTranslation.SetTranslation(parentChildCombined.GetTranslation());

				worldVecQuatMat = parentChildCombinedTranslation * worldOffsetVecQuatMat * parentChildCombinedTranslation.GetInverse() * parentChildCombined;
				worldMatrix = worldVecQuatMat.GetMatrix();
			}
			else
			{
				finalVecQuatMat = localVecQuatMat * worldOffsetVecQuatMat;
				worldMatrix = finalVecQuatMat.GetMatrix();
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

	void Transform::PropagateParentLocalTransform(const VecQuatMatF& parentLocalTransform)
	{
		this->parentLocalMatrix = parentLocalTransform;

		for (const auto& childTransform : this->children)
		{
			childTransform->PropagateParentLocalTransform(parentLocalTransform * this->localVecQuatMat);
		}

		SetWorldDirtyFlag();
	}

	void Transform::Remove()
	{

	}

	QuatF Transform::GetRotation()
	{
		return finalVecQuatMat.quaternion;
	}

	void Transform::SetRotation(const QuatF& newRotation)
	{
		if(parent == nullptr)
		{
			localVecQuatMat.quaternion = newRotation.GetNormalize();
			isLocalDirty = true;
		}

		worldOffsetVecQuatMat.quaternion = newRotation;

		isWorldDirty = true;
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
			localVecQuatMat.vector += GetWorldMatrix().GetRotationMatrix() * localModelMatrix.GetRotationMatrix().GetInverse() * worldOffset;
		}
		else
		{
			localVecQuatMat.vector = worldPosition;
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

	void Transform::Clone(const Transform& transformToClone, Scene* currentScene /* = nullptr */)
	{
		localModelMatrix = transformToClone.localModelMatrix;
		worldMatrix = transformToClone.worldMatrix;
		parentLocalMatrix = transformToClone.parentLocalMatrix;
		inverseOfOriginalParentLocalModelMatrix = transformToClone.inverseOfOriginalParentLocalModelMatrix;
		localVecQuatMat = transformToClone.localVecQuatMat;
		worldOffsetVecQuatMat = transformToClone.worldOffsetVecQuatMat;
		root = transformToClone.root;
		parent = transformToClone.parent;

		isLocalDirty = transformToClone.isLocalDirty;
		isWorldDirty = transformToClone.isWorldDirty;

		// TODO: Figure out if this is needed
		// if(parent != nullptr)
		// {
		// 	parent->SetChild(this);
		// }

		for (auto& transform : transformToClone.children)
		{
			const auto clonedChild = transform->GetGameObject().Clone(currentScene ? currentScene : nullptr);
			children.push_back(clonedChild->GetTransform());
		}
	}

	START_REFLECTION(Transform)
	// CLASS_MEMBER_REFLECTION(localScale)
	// CLASS_MEMBER_REFLECTION(localRotation)
	// CLASS_MEMBER_REFLECTION(localPosition)
	// CLASS_MEMBER_REFLECTION(worldScaleOffset)
	// CLASS_MEMBER_REFLECTION(worldRotationOffset)
	// CLASS_MEMBER_REFLECTION(worldPosition)
	END_REFLECTION(Transform)
}
