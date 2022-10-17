#include "Transform.h"
#include "NightOwl/Core/Utitlity/Assert.h"

// NEED TO CLEAN UP LOGIC, KINDA NASTY
namespace NightOwl::Component
{
	Transform::Transform()
		: Component(ComponentType::Transform),
		  localModelMatrix(1.0f),
	      worldMatrix(1.0f),
		  parentLocalMatrix(1.0f),
		  localScale(1.0f),
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
			worldScale += scale;

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
			localEulerAngles += eulers;

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldEulerAngles += eulers;

			RestrictEulerAngles(worldEulerAngles);

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
		parent = parentTransform;
		parent->SetChild(*this);
		
		for (const auto& childTransform : parentTransform->children)
		{
			childTransform->PropagateParentLocalTransform(parent->GetLocalModelMatrix());
		}
	}

	Transform* Transform::RemoveParent()
	{
		Transform* parentBeingRemoved = parent;

		int indexOfChildToRemove;
		for (indexOfChildToRemove = 0; indexOfChildToRemove < parent->children.size(); indexOfChildToRemove++)
		{
			if (this == parent->children[indexOfChildToRemove]) break;
		}
		parent->children.erase(parent->children.begin() + indexOfChildToRemove);

		parent = nullptr;

		parentLocalMatrix = Math::Mat4F::Identity();

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

			const Math::Mat4F rotationMatrix = Math::Mat4F::MakeRotationX(localEulerAngles.x) *
											   Math::Mat4F::MakeRotationY(localEulerAngles.y) *
											   Math::Mat4F::MakeRotationZ(localEulerAngles.z);


			const Math::Mat4F scaleMatrix = Math::Mat4F::MakeScale(localScale);

			localModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

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
		if (isWorldDirty)
		{
			const Math::Mat4F translationMatrix = Math::Mat4F::MakeTranslation(worldPosition);

			const Math::Mat4F rotationMatrix = Math::Mat4F::MakeRotationX(worldEulerAngles.x) *
											   Math::Mat4F::MakeRotationY(worldEulerAngles.y) *
											   Math::Mat4F::MakeRotationZ(worldEulerAngles.z);


			const Math::Mat4F scaleMatrix = Math::Mat4F::MakeScale(worldScale);

			if(parent != nullptr)
			{
				Math::Mat4F parentChildCombined = parentLocalMatrix * GetLocalModelMatrix();
				const Math::Mat4F parentChildCombinedTranslation = Math::Mat4F::MakeTranslation(parentChildCombined.GetTranslation());
				worldMatrix = parentChildCombinedTranslation * translationMatrix * rotationMatrix * scaleMatrix * parentChildCombinedTranslation.GetInverse() * parentChildCombined;
			}
			else
			{
				worldMatrix =  GetLocalModelMatrix() * worldMatrix;
			}

			isWorldDirty = false;
		}
		else if(isLocalDirty)
		{
			if (parent != nullptr)
			{
				worldMatrix = parentLocalMatrix * GetLocalModelMatrix();
			}
			else
			{
				worldMatrix = GetLocalModelMatrix();
			}
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

	void Transform::RestrictEulerAngles(Math::Vec3F& eulerAngles)
	{
		for (float& angle : eulerAngles.data)
		{
			if (angle > Math::THREE_HUNDRED_SIXTY_DEGREES)
			{
				angle -= Math::THREE_HUNDRED_SIXTY_DEGREES;
			}

			if (angle < -Math::THREE_HUNDRED_SIXTY_DEGREES) 
			{
				angle += Math::THREE_HUNDRED_SIXTY_DEGREES;
			}
		}
	}

	Math::Vec3F Transform::GetWorldScale()
	{
		GetWorldMatrix();

		const Math::Vec3F worldXScale{ worldMatrix(0, 0), worldMatrix(1, 0), worldMatrix(2, 0) };
		const Math::Vec3F worldYScale{ worldMatrix(0, 1), worldMatrix(1, 1), worldMatrix(2, 1) };
		const Math::Vec3F worldZScale{ worldMatrix(0, 2), worldMatrix(1, 2), worldMatrix(2, 2) };

		return {worldXScale.Magnitude(), worldZScale.Magnitude(), worldYScale.Magnitude() };
	}

	void Transform::SetWorldScale(const Math::Vec3F& worldScale)
	{
		this->worldScale = worldScale;
	}

	// BROKEN - CONVERT TO QUATERNION FOR ROTATION
	Math::Vec3F Transform::GetWorldEulerAngles()
	{
		GetWorldMatrix();

		return worldEulerAngles;
	}

	void Transform::SetWorldEulerAngles(const Math::Vec3F& worldEulerAngles)
	{
		this->worldEulerAngles = worldEulerAngles;
	}

	Math::Vec3F Transform::GetWorldPosition() const
	{
		return worldMatrix.GetTranslation();
	}

	void Transform::SetWorldPosition(const Math::Vec3F& worldPosition)
	{
		this->worldPosition = worldPosition;
	}

	bool Transform::HasParent()
	{
		return parent != nullptr;
	}

	bool Transform::HasChildren()
	{
		return !children.empty();
	}
}
