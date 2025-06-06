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
		  parentLocalVecQuatMat(1.0f),
		  inverseOfOriginalParentLocalVecQuatMat(1.0f),
		  root(this),
		  parent(nullptr),
		  isLocalDirty(false),
		  isWorldDirty(false)
	{ }

	void Transform::LookAt(const Vec3F& targetPoint)
	{
		const Vec3F& targetDirection = targetPoint - GetPosition();

		const QuatF lookAtQuaternion = QuatF::LookAt(targetDirection.GetNormalize());

		SetRotation(lookAtQuaternion);
	}

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

	void Transform::Rotate(const QuatF& rotation, Space space)
	{
		if (parent == nullptr || space == Space::Local)
		{
			localVecQuatMat.quaternion = localVecQuatMat.quaternion * rotation;

			localVecQuatMat.quaternion.Renormalize();

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldOffsetVecQuatMat.quaternion = worldOffsetVecQuatMat.quaternion * rotation;

			worldOffsetVecQuatMat.quaternion.Renormalize();

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
			localVecQuatMat.quaternion =  localVecQuatMat.quaternion * QuatF::MakeRotationFromEulers(eulers);

			localVecQuatMat.quaternion.Renormalize();

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldOffsetVecQuatMat.quaternion = worldOffsetVecQuatMat.quaternion * QuatF::MakeRotationFromEulers(eulers);

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

	const QuatF Transform::GetLocalRotation()
	{
		return localVecQuatMat.quaternion;
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
				inverseOfOriginalParentLocalVecQuatMat = parent->localVecQuatMat.GetInverse();
			}

			parent->GetWorldMatrix();
			for (const auto& childTransform : parentTransform->children)
			{
				childTransform->PropagateParentTransform(parent->worldVecQuatMat);
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
			GetWorldMatrix(); // ensure worldVecQuatMat is up-to-date
			localVecQuatMat = worldVecQuatMat;
			isLocalDirty = true;

			// Reset world data since parent has been lost. In this case Local = World
			worldOffsetVecQuatMat = VecQuatMatF::Identity();
			parentLocalVecQuatMat = VecQuatMatF::Identity();
			inverseOfOriginalParentLocalVecQuatMat = VecQuatMatF::Identity();

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
				VecQuatMatF parentChildCombined = parentLocalVecQuatMat * inverseOfOriginalParentLocalVecQuatMat * localVecQuatMat;

				VecQuatMatF parentChildCombinedTranslation;
				parentChildCombinedTranslation.SetTranslation(parentChildCombined.GetTranslation());

				worldVecQuatMat = parentChildCombinedTranslation * worldOffsetVecQuatMat * parentChildCombinedTranslation.GetInverse() * parentChildCombined;

				worldMatrix = worldVecQuatMat.GetMatrix();
			}
			else
			{
				worldVecQuatMat = localVecQuatMat * worldOffsetVecQuatMat;
				worldMatrix = worldVecQuatMat.GetMatrix();
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

	void Transform::PropagateParentTransform(const VecQuatMatF& parentLocalTransform)
	{
		this->parentLocalVecQuatMat = parentLocalTransform;

		SetWorldDirtyFlag();

		for (const auto& childTransform : this->children)
		{
			GetWorldMatrix();
			childTransform->PropagateParentTransform(this->worldVecQuatMat);
		}
	}

	void Transform::Remove()
	{

	}

	QuatF Transform::GetRotation()
	{
		GetWorldMatrix();
		return worldVecQuatMat.quaternion;
	}

	void Transform::SetRotation(const QuatF& newRotation)
	{
		if(parent == nullptr)
		{
			localVecQuatMat.quaternion = newRotation.GetNormalize();
			isLocalDirty = true;
			return;
		}

		worldOffsetVecQuatMat.quaternion = newRotation.GetNormalize();
		isWorldDirty = true;
	}

	Vec3F Transform::GetPosition()
	{
		GetWorldMatrix();
		return worldVecQuatMat.GetTranslation();
	}

	void Transform::SetPosition(const Vec3F& worldPosition)
	{
		if(parent == nullptr)
		{
			localVecQuatMat.vector = worldPosition;
			isLocalDirty = true;
			return;
		}

		// Add to the local position the world offset of the desired position coming in
		const Vec3F worldOffset = worldPosition - (GetPosition() - worldOffsetVecQuatMat.vector);

		// if (worldOffset.Magnitude() < EPSILON)
		// {
		// 	return;
		// }

		// Apply rotation to the offset so that it is aligned with the local position axes
		worldOffsetVecQuatMat.vector = worldOffset;//+= GetWorldMatrix().GetRotationMatrix() * localVecQuatMat.quaternion.GetRotationMatrix().GetInverse() * worldOffset;

		isWorldDirty = true;
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
		return GetRotation() * Vec3F::Forward();
	}

	Vec3F Transform::GetRight()
	{
		return GetRotation() * Vec3F::Right();
	}

	Vec3F Transform::GetUp()
	{
		return GetRotation() * Vec3F::Up();
	}

	void Transform::Clone(const Transform& transformToClone, Scene* currentScene /* = nullptr */, Transform* parent /* = nullptr */)
	{
		localModelMatrix = transformToClone.localModelMatrix;
		worldMatrix = transformToClone.worldMatrix;
		parentLocalVecQuatMat = transformToClone.parentLocalVecQuatMat;
		inverseOfOriginalParentLocalVecQuatMat = transformToClone.inverseOfOriginalParentLocalVecQuatMat;
		localVecQuatMat = transformToClone.localVecQuatMat;
		worldOffsetVecQuatMat = transformToClone.worldOffsetVecQuatMat;
		root = transformToClone.root;
		this->parent = parent;

		isLocalDirty = transformToClone.isLocalDirty;
		isWorldDirty = transformToClone.isWorldDirty;

		for (auto& transform : transformToClone.children)
		{
			const auto clonedChild = transform->GetGameObject().Clone(currentScene ? currentScene : nullptr, this);
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
